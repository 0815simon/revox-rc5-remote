# revox-rc5-remote
Convert any RC5 signals to ITT via serial link using esp8266 (Wemos D1, NodeMCU)

# revox-rc5-remote
does the same, but adds a small webserver to control some functions

This is my very first public project because I needed a remote for my Revox B250 and B226. Because I did not find any I started hacking (trial and error style) and stopped when it worked for me. 
You can add your own commands using the Revox IR service manual depending on your needs. I used a Wemos D1 mini because of the small size and external antenna for WIFI, but the code should work with any other Arduino. And I know the code is not clean, could be more efficient, feel free to optimize it and share your code.

Notes for wire diagram Wemos --> serial link:
Connect D8 to DIN pin 3
GND to DIN 2

If you want to disable the internal IR-receiver using only signals via serial link shortcut pin DIN 1+2 and DIN 4+5. You can use the 5V from pin 5 to power the device. 

Eveything you do is on your own risk!
