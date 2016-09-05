#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266SSDP.h>
#include <WiFiManager.h>
#include <SPI.h>
#include <max6675.h>
#include <Ticker.h>

#define FILTER_LEN 10

/**
 * Thermocouple ports
 * D6 => SO (MISO)
 * D5 => SCK
 * D8 => CS
 *
 * mash_thermocouple CS => D8
 * element_thermocouple CS => D7
 */
Ticker temp_update;
MAX6675 mash_thermocouple;
MAX6675 element_thermocouple;
unsigned mash_temp_idx = 0;
unsigned elem_temp_idx= 0;
double mash_temp_filter[FILTER_LEN];
double elem_temp_filter[FILTER_LEN];
double mash_temp = -999.0;
double elem_temp = -999.0;

void update_temperature();
void get_temp();


WiFiManager wifiManager;
ESP8266WebServer http(80);

void setup(){
  Serial.begin(115200);
  delay(10);

  for(unsigned i = 0; i < FILTER_LEN; ++i) {
    mash_temp_filter[i] = -999.0;
    elem_temp_filter[i] = -999.0;
  }

  SPI.begin();
  mash_thermocouple.begin(D8);
  element_thermocouple.begin(D7);
  delay(500); // waiting for MAX6675 chips to stabalize
  temp_update.attach(0.5, update_temperature);

  http.on("/", HTTP_GET, get_temp);
  http.on("/index.html", HTTP_GET, get_temp);
  http.on("/description.xml", HTTP_GET, [](){
    SSDP.schema(http.client());
  });

  wifiManager.autoConnect("BrewControllerConfig");
  Serial.println();
  Serial.println("WiFi connected");
  http.begin();
  SSDP.setSchemaURL("description.xml");

  SSDP.setHTTPPort(80);
  SSDP.setName("Brew Controller");
  SSDP.setSerialNumber("633878639586");
  SSDP.setURL("index.html");
  SSDP.setModelName("Brew Controller V0.1");
  SSDP.setModelNumber("849862634386");
  SSDP.setModelURL("https://github.com/erifsx/brew_controller");
  SSDP.setManufacturer("Eric Marcoux");
  SSDP.setManufacturerURL("https://github.com/erifsx/brew_controller");
  SSDP.begin();
  Serial.print("Server started at ");
  Serial.print(WiFi.localIP());
  Serial.println(":80");
}

void get_temp() {
  String resp = "{\"temperature\":{\"mash\":";
         resp += mash_temp;
         resp += ", \"element\":";
         resp += elem_temp;
         resp += "}}";
  http.send(200, "application/json", resp);
}

void update_temperature() {
  mash_temp = mash_thermocouple.readFahrenheit();
  elem_temp = element_thermocouple.readFahrenheit();
}

void loop() {
  http.handleClient();
  delay(1);
}
