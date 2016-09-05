#include <Arduino.h>
#include <SPI.h>
#include <max6675.h>

/**
 * Thermocouple ports
 * D6 => SO (MISO)
 * D5 => SCK
 * D8 => CS
 */
MAX6675 thermocouple;

void setup(){
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  SPI.begin();
  thermocouple.begin(D8);
  delay(500); // waiting for MAX6675 chip to stabalize
}

void loop() {
  const double temp = thermocouple.readFahrenheit();
  delay(1000);
  Serial.printf("Temp = ");
  Serial.println(temp);
}
