# MAX6675-library
Arduino library for interfacing with MAX6675 thermocouple amplifier


I just added a hardware SPI mode. In this mode the SPI pins can be reused for your MAX6675 device as well as any other SPI gadget you got connected.

Use the single value contructor to access this mode:
```
MAX6675 aSensor(your_CS_pin);
```
