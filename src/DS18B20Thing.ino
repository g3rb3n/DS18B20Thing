#include <Thing.h>
#include <BlinkPattern.h>
#include <Wire.h>
#include <DS18B20.h>

using namespace g3rb3n;

Thing thing;
BlinkPattern led(BUILTIN_LED);
DS18B20 ds(D2);

BlinkPattern::Pattern<2> panic{{1,1},25};
BlinkPattern::Pattern<2> start{{1,9},25};
BlinkPattern::Pattern<2> normal{{1,39},25};

void setup() 
{
  Serial.begin(230400);
  Serial.println();
  Serial.println("Client:" + thing.clientId());

  led.setPattern(start);
  
  thing.onStateChange([](const String& msg){
    Serial.println(msg);
  });

  String sensor;
  sensor += "sensor/ds18b20/";
  sensor += thing.clientId();
  thing.addSensor(sensor, 5000, [](Value& value){
    Return<float> temperature = ds.temperature();
    if (!temperature.valid())
    {
      Serial.println(String("Error ") + temperature.code());
      led.setPattern(panic);
      return;
    }
    Serial.println(String("Read ") + (float)temperature);
    led.setPattern(normal);
    value = (float)temperature;
  });

  String display;
  display += "display/ds18b20/";
  display += thing.clientId();
  thing.addActuator(display, [](Value& value){
    //Serial.println("Got " +(String) value);
  });

  if (!ds.begin()) 
  {
    Serial.println("Could not find DS18B20 sensor");
    led.setPattern(panic);
  }
  
  thing.begin();
}

void loop()
{
  thing.handle();
  led.handle();
}
