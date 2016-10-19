
////////////////////////////////////////////////////////////////////
//example for: FastLED.addLeds< LPD8808, DATA_PIN, CLOCK_PIN, RGB >
//this will use the fastlib library
//look in the fastled documentation for your ledstrip and config
//uncomment the following lines, and comment the other examples:
// #define FASTLED_CONFIG LPD8806, 7, 5, GRB
// #define LED_COUNT 160
// #define FASTLED_ESP8266_NODEMCU_PIN_ORDER

///////////////////////////
//Config example for this strip: http://www.banggood.com/5M-90W-300SMD-WS2812B-LED-RGB-Colorful-Strip-Light-Waterproof-IP65-WhiteBlack-PCB-DC5V-p-1035641.html
//We need the neopixel library for this, since it supports DMA mode on the esp8266
//In case of the Espressive you need to connect the datapin to RX.
//Uncomment the following lines, and comment the other examples:
#define LED_COUNT 300
#define NEOPIXEL_CONFIG NeoGrbFeature, Neo800KbpsMethod


////////////////// other stuff
#define PIN_POWER_ON    D0
