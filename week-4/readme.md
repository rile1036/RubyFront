This is a work based on "Automatic Water Level Control System"

By default, servo motor starts at position "3"

Switch 1:
It is developed to measure the temperature of water when switch 1 is pressed.
The temperature was implemented through the "dht11" library

Switch 2:
It is developed to measure the height of water when switch 2 is pressed.
When switch 2 is pressed, servo motor returns to the 12 position.
When the height of the water is over 42cm, servo motor returns to the 3 position, 
and "DFG" sound is output, LED is on.
