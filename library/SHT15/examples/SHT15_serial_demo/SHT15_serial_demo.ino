/*
 Demonstration sketch for Hobbybotics SHT1x humidity/temperature sensor
 breakout V1.o.
 
 Reads humidity, temperature (Celsius) and temperature (Fahrenheit).  
 Displays results to Serial Monitor.
*/

#include <SHT15.h>

// Default pins for Wireless Thermostat board is SDA = 16, SCL = 17
SHT15 sensor;

void setup()
{
   sensor.init(16, 17);  // init SHT15 sensor. Arduino pins A2 and A3
   Serial.begin(9600);
   Serial.println("--SHT15 Demo--");
}

void loop()
{
  int temp_c;
  int temp_f;
  int humidity;

  // Read values from the sensor
  temp_c = sensor.measure(TEMPC);
  temp_f = sensor.measure(TEMPF);
  humidity = sensor.measure(HUMI);

  // Print the values to the serial port
  Serial.print("Temperature: ");
  Serial.print(temp_c, DEC);
  Serial.print("C / ");
  Serial.print(temp_f, DEC);
  Serial.print("F. Humidity: ");
  Serial.print(humidity, DEC);
  Serial.println("%");

  delay(2000);
}