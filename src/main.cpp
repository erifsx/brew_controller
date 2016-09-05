#include <Arduino.h>


void setup(){
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  delay(1000);
  Serial.printf("It works\n");
}
