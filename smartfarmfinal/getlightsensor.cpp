  1 #include "getlightsensor.h"
  2 // spi communication with Rpi and get sensor data
  3
  4 int read_mcp3208_adc(unsigned char adcChannel)
  5 {
  6   unsigned char buff[3];
  7   int adcValue = 0;
  8
  9   buff[0] = 0x06 | ((adcChannel & 0x07) >> 2);
 10   buff[1] = ((adcChannel & 0x07) << 6);
 11   buff[2] = 0x00;
 12
 13   digitalWrite(CS_MCP3208, 0);
 14   wiringPiSPIDataRW(SPI_CHANNEL, buff, 3);
 15
 16   buff[1] = 0x0f & buff[1];
 17   adcValue = (buff[1] << 8 ) | buff[2];
 18
 19   digitalWrite(CS_MCP3208, 1);
 20
 21   return adcValue;
 22 }
