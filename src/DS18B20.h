#ifndef _DS18B20_H
#define _DS18B20_H

#include <OneWire.h>

namespace g3rb3n
{

  class DS18B20
  {
    public:
      struct Error{
        uint8_t code;
        String message;
      };

    private:
      OneWire* wire;
      
    public:
      DS18B20(uint8_t _pin);
      bool begin();
      float temperature();
      void temperature(float& temperature, Error& error);
  };
}

#endif
