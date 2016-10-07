
#define LED_COUNT 160

#define PIN_POWER_ON    D0

//look in the fastled documentation for your ledstrip and config
//we're using: FastLED.addLeds< LPD8808, DATA_PIN, CLOCK_PIN, RGB >
#define FASTLED_CONFIG LPD8806, 7, 5, GRB

//if you're using a nodemcu esp8266, like me
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
