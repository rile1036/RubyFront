Smart Farm

1. Overview

The project name is Smart farm. it uses a particular module to measure temperature and brightness. when temperature and brightness is beyond a certain range, fan is ranning and red led is turned on. and All data that is monitored are sended from Raspberry Pi to the server every 10s.

2. Requiremnets
?Monitor temperature and lightness on every 1ms.
?Use an analogous data for a ligthness sensor.
?Build a server in cloud and install MySQL and Apache into it.
?Send sensor data from Raspberry Pi to the server every 10s.
?Turn on FAN when the temperature goes beyond 20 degrees (C) for 5 second.
?Turn on LED when the lightness goes below 10 and turn off it otherwise.
?Each functionality should be performed by an independant thread
?Use a signal mechanism when threads need to communicate each other.

2. Implementation

? Detect temperature
This project uses DHT22 Module, Fan Module When temperature that is measured by DHT22 Module is over 29กษ, Fan is operated 5 second. and then they are turned off. this process is repeated endlessly.

?monitoring thread

this thread is monitoring of sensor data. it monitor temperature and brightness on every 1ms. but It works differently in practice. because gettng the temperature data takes a long time.
if the brightness data that is monitered is lower than 10 degrees, send a signal to the led thread.

3. Demo Video

? https://www.youtube.com/watch?v=KkfrMA25FzY&feature=youtu.be
