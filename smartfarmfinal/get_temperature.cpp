  1 #include "get_temperature.h"
  2
  3 static int dht22_dat[5] = {0,0,0,0,0};
  4
  5 static uint8_t sizecvt(const int read){
  6   if(read > 255 || read < 0){
  7     printf("Invalid data from wiringPi library\n");
  8     exit(EXIT_FAILURE);
  9   }
 10   return (uint8_t)read;
 11 }
 12
 13 int read_temp(){
 14   uint8_t laststate = HIGH;
 15   uint8_t counter = 0;
 16   uint8_t j = 0, i;
 17   int ret_temp = 0;
 18   dht22_dat[0] = dht22_dat[1] = dht22_dat[2] = dht22_dat[3] = dht22_dat[4] =     0;
 19
 20   pinMode(DHTPIN, OUTPUT);
 21   digitalWrite(DHTPIN, HIGH);
 22   delay(10);
 23   digitalWrite(DHTPIN, LOW);
 24   delay(18);
 25   digitalWrite(DHTPIN, HIGH);
 26   delayMicroseconds(40);
 27   pinMode(DHTPIN, INPUT);
 28
 29   for(i=0; i<MAXTIMINGS; i++){
 30     counter = 0;
 31     while(sizecvt(digitalRead(DHTPIN)) == laststate){
 32       counter++;
 33       delayMicroseconds(1);
 34       if(counter == 255){
 35         break;
 36       }
 37     }
 38     laststate = sizecvt(digitalRead(DHTPIN));
 39
 40     if(counter == 255){
 41       break;
 42     }
 43     if((i >= 4) && (i%2 == 0)){
 44       dht22_dat[j/8] <<= 1;
 45       if(counter > 50){
 46         dht22_dat[j/8] |= 1;
 47       }
 48       j++;
 49     }
 50   }
 51
 52   if((j >= 40) && (dht22_dat[4] == ((dht22_dat[0] + dht22_dat[1] + dht22_dat    [2] + dht22_dat[3]) & 0xFF))){
 53     float t;
 54
 55     t = (float)(dht22_dat[2] & 0x7F)* 256 + (float)dht22_dat[3];
 56     t /= 10.0;
 57
 58     if((dht22_dat[2] & 0x80) != 0){
 59       t *= -1;
 60     }
 61     ret_temp = (int)t;
 62
 63     return ret_temp;
 64   }
 65   else{
 66     printf("Data not good, skip\n");
 67     return 0;
 68   }
 69 }
