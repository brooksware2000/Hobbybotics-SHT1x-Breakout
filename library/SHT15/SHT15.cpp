/*-----------------------------------------------------------------------------------------------
 * File: SHT15.cpp
 * Function: SHT15 humidity/temperature library
 * Description: Reads humidity and temperature from SHT1x.
 * Created by Curtis Brooks, March 20, 2012.
 * Updated: 20 May 2012
 * Released into the public domain.
 * 
 * Notes: Modified SHT1x library by Jonathan Oxer 
 * <jon@oxer.com.au> / <www.practicalarduino.com>
 * ----------------------------------------------------------------------------------------------*/

// Add necessary include files
#include "SHT15.h"
 
/*-----------------------------------------------------------------------------------------------
 * Public Methods
 * ----------------------------------------------------------------------------------------------*/

void SHT15::init() {
  _SDA = 16;
  _SCL = 17;
}
 
void SHT15::init(uint8_t SDA, uint8_t SCL) {
  _SDA = SDA;
  _SCL = SCL;
}

float SHT15::measure(uint8_t type) {
  float value;
  
  switch(type) {
    case HUMI : value = humidity(); break;
    case TEMPC : value = tempC(); break;
    case TEMPF : value = tempF(); break;
  }
  return value;
}

/*-----------------------------------------------------------------------------------------------
 * Private Methods
 * ----------------------------------------------------------------------------------------------*/
int SHT15:: shiftIn(uint8_t numBits) {
  int ret = 0;
  
  for (int i = 0; i < numBits; ++i) {
    digitalWrite(_SCL, HIGH);
    delay(10);
    ret = ret * 2 + digitalRead(_SDA);
    digitalWrite(_SCL, LOW);
  }
  return ret;
}

void SHT15::command(uint8_t cmd) {
  int ack;
  
  // start transmission
  pinMode(_SDA, OUTPUT);
  pinMode(_SCL, OUTPUT);
  
  digitalWrite(_SDA, HIGH);
  digitalWrite(_SCL, HIGH);
  digitalWrite(_SDA, LOW);
  digitalWrite(_SCL, LOW);
  digitalWrite(_SCL, HIGH);
  digitalWrite(_SDA, HIGH);
  digitalWrite(_SCL, LOW);
  
  // The command: 3 MSB bits are the address and must be 000. Last 5 bits are the command
  shiftOut(_SDA, _SCL, MSBFIRST, cmd);
  
  // verify we get the correct ack
  digitalWrite(_SCL, HIGH);
  pinMode(_SDA, INPUT);
  ack = digitalRead(_SDA);
  
  if (ack != LOW) {
    // error = "Ack Error 0";
  }
  
  digitalWrite(_SCL, LOW);
  ack = digitalRead(_SDA);
  
  if (ack != HIGH) {
    // error = "Ack Error 1";
  }
}

void SHT15::wait() {
  int ack;
  
  pinMode(_SDA, INPUT);
  
  for (int i = 0; i < 100; ++i) {
    delay(10);
    
    ack = digitalRead(_SDA);
    
    if(ack == LOW)
      break;
  }
  
  if(ack == HIGH) {
    // error = "Ack Error 2";
  }
}

int SHT15::readByte16() {
  int value;
  
  // get MSB
  pinMode(_SDA, INPUT);
  pinMode(_SCL, OUTPUT);
  
  value = shiftIn(8);
  value *= 256;
  
  // send required ack
  pinMode(_SDA, OUTPUT);
  
  digitalWrite(_SDA, HIGH);
  digitalWrite(_SDA, LOW);
  digitalWrite(_SCL, HIGH);
  digitalWrite(_SCL, LOW);
  
  // get LSB
  pinMode(_SDA, INPUT);
  value |= shiftIn(8);
  
  return value;
}

void SHT15::skipCRC() {
  // skip ack to end transmission
  pinMode(_SDA, OUTPUT);
  pinMode(_SCL, OUTPUT);
  
  digitalWrite(_SDA, HIGH);
  digitalWrite(_SCL, HIGH);
  digitalWrite(_SCL, LOW);
}

float SHT15::rawTemp() {
  int value;
  
  command(MEASURE_TEMP);
  wait();
  value = readByte16();
  skipCRC();
  return value;
}

float SHT15::tempC() {
  int value;          // raw value from sensor
  float temperature;  // temperature derived from raw value
  
  // conversion coefficients from SHT15 datasheet
  const float D1 = -40.0;  // for 14 Bit @ 5V
  const float D2 =   0.01; // for 14 Bit DEGC
  
  // get raw value from sensor
  value = rawTemp();
  
  // convert raw value to degrees Celsius
  temperature = (value * D2) + D1;
  
  return temperature;
}

float SHT15::tempF() {
  int value;        // raw value from sensor
  int temperature;  // temperature derived from raw value
  
  // conversion coefficients from SHT15 datasheet
  const float D1 = -40.0;   // for 14 Bit @ 5V
  const float D2 =   0.018; // for 14 Bit DEGF
  
  // get raw value from sensor
  value = rawTemp();
  
  // convert raw value to degrees Celsius
  temperature = (value * D2) + D1;
  
  return temperature;
}

float SHT15::humidity() {
  int value;                // raw humidity value from sensor
  float linearHumidity;     // humidity with linear correction applied
  float correctedHumidity;  // temperature corrected humidity
  float temperature;        // raw temperature value
  
  // conversion coefficents from SHT15 datasheet
  const float C1 = -4.0;       // for 12 Bit
  const float C2 =  0.0405;    // for 12 Bit
  const float C3 = -0.0000028; // for 12 Bit
  const float T1 =  0.01;      // for 14 Bit @ 5V
  const float T2 =  0.00008;   // for 14 Bit @ 5V
  
  // get raw value from sensor
  command(MEASURE_HUMI);
  wait();
  value = readByte16();
  skipCRC();
  
  // apply linear conversion to raw value
  linearHumidity = C1 + C2 * value + C3 * value * value;
  
  // get current temperature for humidity correction
  temperature = tempC();
  
  // correct humidity value for current temperature
  correctedHumidity = (temperature - 25.0 ) * (T1 + T2 * value) + linearHumidity;
  
  return correctedHumidity;
}
