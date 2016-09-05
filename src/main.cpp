#include <Arduino.h>

int state = HIGH;

void setup(){
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, state = !state);
  delay(1000);
}
