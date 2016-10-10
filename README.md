# Led animation library for ESP8266

- Has a nice webgui to select and SIMULATE led animations.
- Create web animations in javascript in your browser and submit them to your ESP8266 that has an actual ledstrip.
- Our goal is to create a huge library with animations.


For a demonstration look at: https://rawgit.com/psy0rz/ledanim/master/web/public_html/index.html

In future version the control-tab will control certiain variables of the animation.

## HOWTO

### 1. hardware 

Build something like this:

![schematic](https://github.com/psy0rz/ledanim/blob/master/ledanim.png?2)
 
You can skip the transistor and just power your ATX supply by connecting green to ground. (or use a different 5v power supply) 
 
### 2. configure software

- install platformio-ide into Atom editor.

- with Atom open the directory ledanim/esp8266

- change platform.ini to match your board and port. (you can also do this via the gui)

- edit config.h and fill in some sensible values. 

### 3. wifi configuration

- create ledsim/esp8266/data/wifi.txt:
```
myssid
mypassword
``` 
 
- Upload SPIFFS

- Build and upload project

- Use serial monitor to see if the unit boots and connects to your wifi

- Goto http://ipadress

- ( You might need to refresh/reload the page a few times, the esp8266 webserver code doesnt handle parallel connections very well )

In later versions you can just use the online webbpage from github and add the ip's of your ESP8266.




