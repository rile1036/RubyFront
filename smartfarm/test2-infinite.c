#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

#define MAXTIMINGS 85
#define PUMP 21

void sig_handler(int signo);

static int DHTPIN = 11;
static int dht22_dat[5] = {0,0,0,0,0};

static uint8_t sizecvt(const int read)
{
	if (read > 255 || read < 0)
	{
		printf("Invalid data from wiringPi library\n");
		exit(EXIT_FAILURE);
	}
	return (uint8_t)read;
}

static int read_dht22_dat()
{
	uint8_t laststate = HIGH;
	uint8_t counter = 0;
	uint8_t j = 0, i;

	dht22_dat[0] = dht22_dat[1] = dht22_dat[2] = dht22_dat[3] = dht22_dat[4] = 0;
	
	pinMode(PUMP, OUTPUT);
	pinMode(DHTPIN, OUTPUT);
	digitalWrite(DHTPIN, HIGH);
	delay(10);
	digitalWrite(DHTPIN, LOW);
	delay(18);
	digitalWrite(DHTPIN, HIGH);
	delayMicroseconds(40); 
	pinMode(DHTPIN, INPUT);

	for ( i=0; i< MAXTIMINGS; i++) {
		counter = 0;
		while (sizecvt(digitalRead(DHTPIN)) == laststate) {
			counter++;
			delayMicroseconds(1);
			if (counter == 255) {
				break;
			}
		}
		laststate = sizecvt(digitalRead(DHTPIN));

		if (counter == 255) break;
		if ((i >= 4) && (i%2 == 0)) {
			dht22_dat[j/8] <<= 1;
			if (counter > 50)
				dht22_dat[j/8] |= 1;
			j++;
		}
	}

	if ((j >= 40) && (dht22_dat[4] == ((dht22_dat[0] + dht22_dat[1] + dht22_dat[2] + dht22_dat[3]) & 0xFF)) ) {
        float t, h;
        h = (float)dht22_dat[0] * 256 + (float)dht22_dat[1];
        h /= 10;
        t = (float)(dht22_dat[2] & 0x7F)* 256 + (float)dht22_dat[3];
        t /= 10.0;
        if ((dht22_dat[2] & 0x80) != 0)  t *= -1;			
		while(1){
    		printf("Temperature = %2.f *C \n", t);
			delay(1000);
			if(t > 25){
				digitalWrite(PUMP, 1);
				delay(1000);
				digitalWrite(PUMP, 0);
			}
			digitalWrite(PUMP, 0);
		}
    	return 1;
	}
	else{
		printf("Data not good, skip\n");
		return 0;
	}
}

void sig_handler(int signo){
	digitalWrite(PUMP, 0);
	exit(0);
}

int main (void)
{
	signal(SIGINT, (void *)sig_handler);

	if (wiringPiSetup() == -1)
    exit(EXIT_FAILURE) ;

	if (setuid(getuid()) < 0){
		perror("Dropping privileges failed\n");
		exit(EXIT_FAILURE);
	}

	while (read_dht22_dat() == 0) {
		delay(1000); 
	}
	return 0 ;
}
