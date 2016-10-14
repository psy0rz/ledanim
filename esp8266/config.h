//look in the fastled documentation for your ledstrip and config

////////////////////////////////////////////////////////////////////
//example for: FastLED.addLeds< LPD8808, DATA_PIN, CLOCK_PIN, RGB >
#define FASTLED_CONFIG LPD8806, 7, 5, GRB
#define LED_COUNT 160

///////////////////////////
//config example for this strip: http://www.banggood.com/5M-90W-300SMD-WS2812B-LED-RGB-Colorful-Strip-Light-Waterproof-IP65-WhiteBlack-PCB-DC5V-p-1035641.html
//XXX: not working yet, need to tune fastled or use use neopixelbus DMA mode
// #define LED_COUNT 300
// #define FASTLED_CONFIG WS2812Controller800Khz, 6, GRB


////////////////// other stuff
#define PIN_POWER_ON    D0

//if you're using a nodemcu esp8266, like me
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
