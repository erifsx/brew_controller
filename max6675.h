// this library is public domain. enjoy!
// www.ladyada.net/learn/sensors/thermocouple
#ifndef MAX6675_LIBRARY_H
#define MAX6675_LIBRARY_H

#ifndef max6675_h
#define max6675_h

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class MAX6675 {
 public:
  /**
   * Initiate software SPI mode.
   */ 
  MAX6675(int8_t SCLK, int8_t CS, int8_t MISO);
  /**
   * Initiate hardware SPI mode.
   * Assumes you already setup spi (with SPI.begin())
   */ 
  MAX6675(int8_t CS);
  
  double readCelsius(void);
  double readFahrenheit(void);
  // For compatibility with older versions:
  double readFarenheit(void) { return readFahrenheit(); }
 private:
  int8_t sclk, miso, cs;
  uint8_t spiread(void);
  bool hwSPI = false;
};

#endif
