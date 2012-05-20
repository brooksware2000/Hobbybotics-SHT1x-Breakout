/*-----------------------------------------------------------------------------------------------
 * File: SHT15.h
 * Function: SHT15 humidity/temperature library
 * Description: Reads humidity and temperature from SHT1x.
 * Created by Curtis Brooks, March 20, 2012.
 * Updated: 20 May 2012
 * Released into the public domain.
 * 
 * Notes: Modified SHT1x library by Jonathan Oxer 
 * <jon@oxer.com.au> / <www.practicalarduino.com>
 * ----------------------------------------------------------------------------------------------*/

#ifndef SHT15_h
#define SHT15_h

// Keep compatability with Arduino 1.0 and below
#if ARDUINO>=100
#include <Arduino.h>             // Arduino 1.0
#else
#include <WProgram.h>            // Arduino 0023 and below
#endif
 
                            // addr  command   r/w
#define MEASURE_TEMP  0x03  // 000    0001      1
#define MEASURE_HUMI  0x05  // 000    0010      1
#define STATUS_REG_W  0x06  // 000    0011      0
#define STATUS_REG_R  0x07  // 000    0011      1
#define RESET         0x1e  // 000    1111      0
 
enum {
  HUMI,
  TEMPC,
  TEMPF
};
 
class SHT15 {
  public:
    void init();
    void init(uint8_t SDA, uint8_t SCL);
    float measure(uint8_t type);
   
  private:
    int shiftIn(uint8_t numBits);
    void command(uint8_t cmd);
    void wait();
    int readByte16();
    void skipCRC();
    float rawTemp();
    float tempC();
    float tempF();
    float humidity();
    
    uint8_t _SDA;
    uint8_t _SCL;
};

#endif