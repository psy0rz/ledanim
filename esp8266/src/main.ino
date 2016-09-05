#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include <FastLED.h>
#include "strip_anim.hpp"

const char* ssid = "*****";
const char* password = "******";
MDNSResponder mdns;

ESP8266WebServer server(80);


void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

#define LED_COUNT 160

strip_anim_c<LED_COUNT> strip_anim;

void setup(void){


    //fire animation, we try to make the indirect light on the ground look like fire.
    strip_anim.commands={
        //glowing red background
        CMD_REPEAT_BEGIN       , 0,100,
        CMD_PEN_COLOR_RND      , 20,127 , 0,0 , 0,0 ,
        CMD_PEN_FADE_SPEED     , 1  ,
        CMD_LED_NR_16_RND      , 0,0 , 255,255 ,
        CMD_PEN_WIDTH          , 1,
        CMD_PEN_FADE_MODE      , FADE_TO_FAST ,
        CMD_PEN_DRAW           ,
        CMD_REPEAT_END         ,

        //bright yellow flames
        CMD_REPEAT_BEGIN       , 0,2,
        CMD_PEN_COLOR_RND      , 200,255 , 100,200 , 0,0 ,
        CMD_PEN_FADE_SPEED_RND , 10 , 30 ,
        CMD_LED_NR_16_RND      , 0,0 , 255,255 ,
        CMD_PEN_WIDTH_RND      , 1 , 3,
        CMD_PEN_FADE_MODE      , FADE_TO_FAST ,
        CMD_PEN_DRAW           ,
        CMD_REPEAT_END         ,
    };


    // //KITT knightrider radar
    // strip_anim.commands={
    //     CMD_PEN_COLOR          , 255 , 0 , 0 ,
    //     CMD_PEN_FADE_SPEED     , 5  ,
    //     CMD_PEN_FADE_MODE      , FADE_FROM_FAST ,
    //     CMD_PEN_CLONE_COUNT    , 0,3 ,
    //     CMD_PEN_CLONE_OFFSET   , 0,9 ,
    //
    //     //to the right
    //     CMD_PEN_STEP           , 1 ,
    //     CMD_REPEAT_BEGIN       , 0,6,
    //     CMD_PEN_DRAW           ,
    //     CMD_DELAY_8            , 8 ,
    //     CMD_REPEAT_END         ,
    //
    //     //to the left
    //     CMD_PEN_STEP           , (uint8_t)-1 ,
    //     CMD_REPEAT_BEGIN       , 0,6,
    //     CMD_PEN_DRAW           ,
    //     CMD_DELAY_8            , 8 ,
    //     CMD_REPEAT_END         ,
    // };
    //
    // //police lights
    // strip_anim.commands={
    //     CMD_PEN_WIDTH          , 10,
    //     CMD_PEN_CLONE_COUNT    , 0,5 ,
    //     CMD_PEN_CLONE_OFFSET   , 0,20,
    //
    //     ///////////////////////// left red
    //
    //
    //     //left red short
    //     CMD_LED_NR_8           , 0,
    //     CMD_PEN_COLOR          , 255 , 0 , 0 ,
    //     CMD_PEN_DRAW           ,
    //     CMD_DELAY_8            , 4,
    //
    //     //left off
    //     CMD_LED_NR_8           , 0,
    //     CMD_PEN_COLOR          , 0 , 0 , 0 ,
    //     CMD_PEN_DRAW           ,
    //     CMD_DELAY_8            , 2,
    //
    //
    //     //left red long
    //     CMD_LED_NR_8           , 0,
    //     CMD_PEN_COLOR          , 255 , 0 , 0 ,
    //     CMD_PEN_DRAW           ,
    //     CMD_DELAY_8            , 14,
    //
    //     //left off
    //     CMD_LED_NR_8           , 0,
    //     CMD_PEN_COLOR          , 0 , 0 , 0 ,
    //     CMD_PEN_DRAW           ,
    //
    //     ///////////////////////// right blue
    //
    //     //right blue short
    //     CMD_LED_NR_8           , 10,
    //     CMD_PEN_COLOR          , 0, 0 , 255 ,
    //     CMD_PEN_DRAW           ,
    //     CMD_DELAY_8            , 4,
    //
    //     //right off
    //     CMD_LED_NR_8           , 10,
    //     CMD_PEN_COLOR          , 0 , 0 , 0 ,
    //     CMD_PEN_DRAW           ,
    //     CMD_DELAY_8            , 2,
    //
    //
    //     //right blue long
    //     CMD_LED_NR_8           , 10,
    //     CMD_PEN_COLOR          , 0 , 0 , 255,
    //     CMD_PEN_DRAW           ,
    //     CMD_DELAY_8            , 14,
    //
    //     //right off
    //     CMD_LED_NR_8           , 10,
    //     CMD_PEN_COLOR          , 0 , 0 , 0 ,
    //     CMD_PEN_DRAW           ,
    //
    //
    // };

    //data, clock
    FastLED.addLeds<LPD8806, 7,5, GRB >(strip_anim.led_anim.led_level, LED_COUNT);
    strip_anim.step();

  Serial.begin(115200);
  return;

  WiFi.begin(ssid, password);
  Serial.println("");

  // // Wait for connection
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println("");
  // Serial.print("Connected to ");
  // Serial.println(ssid);
  // Serial.print("IP address: ");
  // Serial.println(WiFi.localIP());

  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}


unsigned long last_micros=0;
void loop(void){
  // Serial.println("lup");
  server.handleClient();
  strip_anim.step();

  //limit to 60 fps
  while (micros()-last_micros < 16666);
  // while (micros()-last_micros < 1000000);
  last_micros=micros();

  FastLED.show();

}
