#include <Wire.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac;

int analogs=A0;
int PWM_Value=0;


void setup()
{
  dac.begin(0x63);

  pinMode(analogs,INPUT);
  
}

void loop()
{  
  PWM_Value= analogRead(analogs);
  PWM_Value=map(PWM_Value,0,1023,0,4095);
  dac.setVoltage(PWM_Value, false);
 
}
