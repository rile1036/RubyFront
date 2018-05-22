  1 #ifndef GETLIGHTSENSOR
  2 #include <stdio.h>
  3 #include <string.h>
  4 #include <errno.h>
  5 #include <wiringPi.h>
  6 #include <wiringPiSPI.h>
  7
  8 #define CS_MCP3208 8
  9 #define SPI_CHANNEL 0
 10 #define SPI_SPEED 1000000
 11
 12 int read_mcp3208_adc(unsigned char adcChannel);
 13
 14 #endif
