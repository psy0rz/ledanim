NOTE: If you want something much more advanced, for use with led-matrixes and a raspberry, keep an eye on https://github.com/psy0rz/ledder 

# Led animation library for ESP8266
                    
- Has a nice webgui to select and SIMULATE led animations.
- Create web animations in javascript in your browser and submit them to your ESP8266 that has an actual ledstrip.
- Our goal is to create a huge library with animations.


For a demonstration look at: http://ledanim.datux.nl/


## Supported ledstrips

- Most FastLED led-strips: https://github.com/FastLED/FastLED/wiki/Chipset-reference

- Most NEOpixelbus led-strips: https://github.com/Makuna/NeoPixelBus/wiki

For most SPI-controlled strips you should be fine with FastLED. However there are timing issues with serial-controlled strips like the WS2812b. In that case you should use the NeoPixelBus library. (it works excellent with the WS2812b)

NeoPixelBus uses all kind of hardware tricks (DMA, UARTS) to create smoother and faster led-updates. (but its less portable as a library) 


## HOWTO

### 1. hardware 

Build something like this:

![schematic](https://github.com/psy0rz/ledanim/blob/master/ledanim.png?2)
 
You can skip the transistor and just power your ATX supply by connecting green to ground. (or use a different 5v power supply) 
 
### 2. configure software

- install platformio-ide into Atom editor.

- with Atom open the directory ledanim/esp8266

- change platform.ini to match your board and port. (you can also do this via the gui)

- edit config.h and enable the correct ledstrip config. (and FastLed/NeopixelBus stuff)

### 3. wifi configuration

- create ledsim/esp8266/data/wifi.txt:
```
myssid
mypassword
``` 
 
- Upload SPIFFS

- Build and upload project



### 4. testing

- After powerup the first 3 leds should be blinking in order r, g, b with a fade effect. Check if the rgb-order is fine and the fading is smooth.

- Use serial monitor to see if the unit boots and connects to your wifi

- Goto http://ipadress

- ( You might need to refresh/reload the page a few times, the esp8266 webserver code doesnt handle parallel connections very well )

In later versions you can just use the online webbpage from github and add the ip's of your ESP8266.
