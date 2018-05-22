#include <stdio.h>
  2 #include <stdlib.h>
  3 #include <stdint.h>
  4 #include <string.h>
  5 #include <signal.h>
  6 #include <errno.h>
  7 #include <sys/types.h>
  8 #include <unistd.h>
  9 #include <pthread.h>
 10 #include <wiringPi.h>
 11 #include <wiringPiSPI.h>
 12 #include <mysql/mysql.h>
 13 #include <time.h>
 14 #include <math.h>
 15
 16 #include "fanon.h"
 17 #include "get_temperature.h"
 18 #include "getlightsensor.h"
 19 #include "rgbtest.h"
 20
 21 #define MAX 100
 22
 23 #define DBHOST "localhost"
 24 #define DBUSER "root"
 25 #define DBPASS "root"
 26 #define DBNAME "iotfarm"
 27
 28 pthread_cond_t empty, fill, fanon, ledon = PTHREAD_COND_INITIALIZER;
 29 pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
 30
 31 MYSQL *connector;
 32 MYSQL_RES *result;
 33 MYSQL_ROW row;
 34
 35 int fill_ptr = 0;
 36 int use_ptr = 0;
 37 int count = 0;
 38 int isLed = 0;
 39 int isFan = 0;
 40
 41
 42 char query[1024];
 43 unsigned char adcChannel_light = 0;
 44
 45 typedef struct _buffer_data{
 46    int temp;
 47    int lightness;
 48 }buffer_data;
 49
 50 buffer_data buff[MAX];
 51 buffer_data tmp;
 52
 53 void put(int t, int l)
 54 {
 55    buff[fill_ptr].temp = t;
 56    buff[fill_ptr].lightness = l + 10;
 57    printf("temperature : %d\n", buff[fill_ptr].temp);
 58    printf("light : %u\n", buff[fill_ptr].lightness);
 59    fill_ptr = (fill_ptr + 1) % MAX;
 60    delay(500);
 61    count++;
 62 }
 63
 64 buffer_data get()
 65 {
 66    buffer_data g = buff[use_ptr];
 67    use_ptr = (use_ptr + 1) % MAX;
 68    count--;
 69    return g;
 70 }
 71
 72 void *led_func(void *arg)
 73 {
 74    while(1)
 75    {
 76        pthread_mutex_lock(&mutex);
 77       while(isLed==0){
 78          pthread_cond_wait(&ledon,&mutex);
 79       }
 80       digitalWrite(RED, 1);
 81       delay(5000);
 82       pthread_mutex_unlock(&mutex);
 83       isLed=0;
 84       digitalWrite(RED, 0);
 85    }
 86 }
 87
 88 void *fan_func(void *arg)
 89 {
 90    while(1)
 91    {
 92       pthread_mutex_lock(&mutex);
 93       while(isFan==0){
 94          pthread_cond_wait(&fanon,&mutex);
 95       }
 96       digitalWrite(FAN, 1);
 97       delay(5000);
 98       pthread_mutex_unlock(&mutex);
 99       isFan = 0;
100       digitalWrite(FAN, 0);
101    }
102 }
103
104 void *monitor_data(void *arg)
105 {
106     int temp, lightness;
107     while(1){
108       pthread_mutex_lock(&mutex);
109       while(count==MAX){
110          pthread_cond_wait(&empty,&mutex);
111       }
112
113       temp = read_temp();
114       lightness = read_mcp3208_adc(adcChannel_light);
115       put(temp,lightness);
116       if(temp >= 29){
117         fanon = 1; pthread_cond_signal(&fanon);
118       }
119       if(temp < 29) fanon = 0;
120       if(lightness >= 10){
121          ledon = 1; pthread_cond_signal(&ledon);
122       }
123       if(lightness < 10) ledon = 0;
124       pthread_cond_signal(&fill);
125       pthread_mutex_unlock(&mutex);
126
127       delay(100);
128    }
129 }
130 void *send_data(void *arg)
131 {
132    while(1){
133       pthread_mutex_lock(&mutex);
134       while(count == 0)
135       {
136          pthread_cond_wait(&fill,&mutex);
137       }
138       tmp = get();
139       sprintf(query,"insert into smart values (now(),%d %d)",0, 0);
140       pthread_mutex_unlock(&mutex);
141       delay(10000);
142    }
143 }
144 void sensor_init()
145 {
146    pinMode(FAN, OUTPUT);
147    pinMode(DHTPIN,OUTPUT);
148    pinMode(RED,OUTPUT);
149    pinMode(GREEN, OUTPUT);
150    pinMode(BLUE, OUTPUT);
151 }
152
153 int main(void)
154 {
155    pthread_t p1, p2, p3, p4;
156    if(wiringPiSetup() == -1){
157       fprintf(stdout,"Error Start : %s\n",strerror(errno));
158       return 1;
159    }
160    if(wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) == -1){
161       fprintf(stdout, "Failed : %s\n",strerror(errno));
162       return 1;
163    }
164    pinMode(CS_MCP3208, OUTPUT);
165    sensor_init();
166    printf("INIT!!\n");
167    connector = mysql_init(NULL);
168    if(!mysql_real_connect(connector, DBHOST,DBUSER,DBPASS, DBNAME, 3306,NULL    ,0))
169    {
170           fprintf(stderr,"%s\n",mysql_error(connector));
171           return 0;
172    }
173    printf("MySQL opened.\n");
174
175    pthread_create(&p1,NULL,monitor_data, (void*)"monitor");
176    printf("pthread[1] create\n");
177    pthread_create(&p2,NULL,send_data, (void*)"send");
178    printf("pthread[2] create\n");
179    pthread_create(&p3,NULL,led_func, (void*)"led_control");
180    printf("pthread[3] create\n");
181    pthread_create(&p4,NULL,fan_func, (void*) "fan_control");
182    printf("pthread[4] create\n");
183    pthread_join(p1, NULL);
184    pthread_join(p2, NULL);
185    pthread_join(p3, NULL);
186    pthread_join(p4, NULL);
187    mysql_close(connector);
188    return 0;
189 }

