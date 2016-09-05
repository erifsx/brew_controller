#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
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


WiFiManager wifiManager;
WiFiServer server(80);

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

  wifiManager.autoConnect("BrewControllerConfig");
  Serial.println();
  Serial.println("WiFi connected");
  server.begin();
  Serial.print("Server started at ");
  Serial.print(WiFi.localIP());
  Serial.println(":80");
}

void update_temperature() {
  mash_temp = mash_thermocouple.readFahrenheit();
  elem_temp = element_thermocouple.readFahrenheit();

  Serial.print("mash temp = ");
  Serial.println(mash_temp);
  Serial.print("element temp = ");
  Serial.println(elem_temp);
}

void loop() {
  WiFiClient client = server.available();
  delay(1);

  // wifi section
  if(client && client.available()) {
    String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n";
           s += "<html>\r\n";
           s += "Mash temp: ";
           s += mash_temp;
           s += "<br/>Element temp: ";
           s += elem_temp;
           s += "</html>\n";
    client.print(s);
    client.stop();
  }
  delay(10);
}
