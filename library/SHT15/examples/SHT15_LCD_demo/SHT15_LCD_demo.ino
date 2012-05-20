/*
 Demonstration sketch for Hobbybotics SHT1x humidity/temperature sensor
 breakout V1.o.
 
 Reads humidity, temperature (Celsius) and temperature (Fahrenheit).  
 Displays results to LCD.
*/

#include <LCD.h>
#include <SHT15.h>
#include <Wire.h>

// create LCD object
LCD lcd;

// default pins for Wireless Thermostat board is SDA = 16, SCL = 17
SHT15 sensor;  

// degree symbol
const char degree = 223;

void setup()
{
  sensor.init();  // init SHT15 sensor
  //sensor.init(16, 17);  // init SHT15 sensor
  lcd.begin(20, 4);  // setup LCD number or columns and rows
     
  lcd.clear();
  
  lcd.print("--SHT15 Demo--");
    
  lcd.setCursor(0, 1);
  lcd.print("Temp C: ");
  
  lcd.setCursor(0, 2);
  lcd.print("Temp F: ");
  
  lcd.setCursor(0, 3);
  lcd.print("Humidity: ");
}

void loop()
{
  float temp_c;
  float temp_f;
  float humidity;

  // Read values from the sensor
  temp_c = sensor.measure(TEMPC);
  temp_f = sensor.measure(TEMPF);
  humidity = sensor.measure(HUMI);

  lcd.setCursor(10, 1);
  lcd.print(temp_c);
  lcd.print(degree);
  lcd.print("C");
  
  lcd.setCursor(10, 2);
  lcd.print(temp_f);
  lcd.print(degree);
  lcd.print("F");
  
  lcd.setCursor(10, 3);
  lcd.print(humidity);
  lcd.print("%");

  delay(1000);
}