import RPi.GPIO as GPIO
import time
import Adafruit_DHT as dht
import pygame
h, t = dht.read_retry(dht.DHT11, 26)

GPIO.setmode(GPIO.BCM)

GPIO.setup(22, GPIO.IN)
GPIO.setup(23, GPIO.OUT)
GPIO.setup(24, GPIO.IN)

TRIG = 17
ECHO = 27

GPIO.setup(TRIG, GPIO.OUT)
GPIO.setup(ECHO, GPIO.IN)

GPIO.setup(4, GPIO.OUT)
motor = GPIO.PWM(4,50)

motor.start(3)

pygame.mixer.init()

music = pygame.mixer.Sound("DFJ.wav")

def m_distance():
	GPIO.output(TRIG, True)
	time.sleep(0.00001)
	GPIO.output(TRIG, False)

	while GPIO.input(ECHO) == 0:
		start = time.time()
	while GPIO.input(ECHO) == 1:
		end = time.time()
	pulse_duration = end - start
	distance = pulse_duration * 17150
	distance = 50 - round(distance, 2)
	return distance

if __name__ == '__main__':
	try:
		while True:
 			inputIO = GPIO.input(22)
			inputIO2 = GPIO.input(24)
			if inputIO == 0 :
				print "Current water temperature is ",h," degrees"
				time.sleep(1)
			if inputIO2 == 0 :
				distance = m_distance()
				print "it is now full to ", distance, "cm"
				motor.ChangeDutyCycle(13)
				if distance > 42 :
					print "Stop Water!!"
					motor.ChangeDutyCycle(3)
					GPIO.output(23, True)
					music.play()
					time.sleep(0.5)
				time.sleep(1)
			else:
				motor.ChangeDutyCycle(3)
				GPIO.output(23,False)
	except KeyboardInterrupt:
		pass
GPIO.cleanup()

