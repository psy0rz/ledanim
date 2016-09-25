#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "FS.h"

#include <MD5.h>


#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include <FastLED.h>
#include "strip_anim.hpp"



ESP8266WebServer server(80);


// void handleRoot() {
//     server.send(200, "text/plain", "hello from esp8266!");
// }
//

void wifi_config()
{
    File wifi_config_fh = SPIFFS.open("/wifi.txt", "r");
    if (!wifi_config_fh) {
        Serial.println("cannot load wifi config.");
    }
    else
    {
        String ssid=wifi_config_fh.readStringUntil('\n');
        String password=wifi_config_fh.readStringUntil('\n');

        Serial.print("Wifi connecting to: ");
        Serial.println(ssid.c_str());
        WiFi.begin(ssid.c_str(), password.c_str());
    }
}



String getContentType(String filename)
{
    if(server.hasArg("download")) return "application/octet-stream";
    else if(filename.endsWith(".htm")) return "text/html";
    else if(filename.endsWith(".html")) return "text/html";
    else if(filename.endsWith(".css")) return "text/css";
    else if(filename.endsWith(".js")) return "application/javascript";
    else if(filename.endsWith(".png")) return "image/png";
    else if(filename.endsWith(".gif")) return "image/gif";
    else if(filename.endsWith(".jpg")) return "image/jpeg";
    else if(filename.endsWith(".ico")) return "image/x-icon";
    else if(filename.endsWith(".xml")) return "text/xml";
    else if(filename.endsWith(".pdf")) return "application/x-pdf";
    else if(filename.endsWith(".zip")) return "application/x-zip";
    else if(filename.endsWith(".gz")) return "application/x-gzip";
    return "text/plain";
}

//hash to workaround 32 character path-limit
String hashed_gz_path(String & path)
{
    unsigned char* hash=MD5::make_hash(path.c_str());
    char *md5str = MD5::make_digest(hash, 13);
    String ret(md5str);
    ret="/"+ret+".gz";
    free(md5str);
    free(hash);
    return(ret);
}

bool handleFileRead(String path)
{
    if(path.endsWith("/"))
    {
        path += "index.html";
    }

    Serial.println("handleFileRead: " + path);

    String contentType = getContentType(path);

    path=hashed_gz_path(path);
    Serial.println("hashed file: " + path);

    if(SPIFFS.exists(path))
    {
        File file = SPIFFS.open(path, "r");
        size_t sent = server.streamFile(file, contentType);
        file.close();
        return true;
    }
    else
    {
        return false;
    }
}


#define LED_COUNT 160
strip_anim_c<LED_COUNT> strip_anim;

void setup(void){
    Serial.begin(115200);

    delay(100);
    Serial.println();
    Serial.println("LEDanim 1.1 booting...");

    // Serial.println(ESP.getSketchSize());
    // Serial.println(ESP.getFreeSketchSpace());


    //idle lights, testing rgb order
    commands_t commands={
        CMD_LED_NR_8           , 0,
        CMD_PEN_FADE_MODE      , FADE_TO_FAST,
        CMD_PEN_FADE_SPEED     , 5,

        //R
        CMD_PEN_COLOR          , 255 , 0 , 0 ,
        CMD_PEN_DRAW           ,
        CMD_DELAY_8            , 60,

        //G
        CMD_PEN_COLOR          , 0 , 255 , 0 ,
        CMD_PEN_DRAW           ,
        CMD_DELAY_8            , 60,

        //G
        CMD_PEN_COLOR          , 0 , 0, 255 ,
        CMD_PEN_DRAW           ,
        CMD_DELAY_8            , 60,

        //all off
        CMD_LED_NR_8           , 0,
        CMD_PEN_COLOR          , 0 , 0 , 0 ,
        CMD_PEN_DRAW           ,
        CMD_PEN_DRAW           ,
        CMD_PEN_DRAW           ,
        CMD_DELAY_8            , 120,

    };
    strip_anim.set_commands(commands);

    //data, clock
    FastLED.addLeds<LPD8806, 7,5, GRB >(strip_anim.led_anim.led_level, LED_COUNT);
    FastLED.setDither(DISABLE_DITHER);
    strip_anim.step();


    if (!SPIFFS.begin())
    Serial.println("SPIFFS: error while mounting");

    wifi_config();

    // server.on("/", handleRoot);

    // server.on("/inline", [](){
    //     server.send(200, "text/plain", "this works as well");
    // });

    // Dir dir = SPIFFS.openDir("/");
    // while (dir.next()) {
    //     Serial.println(dir.fileName());
    // }
    // FSInfo fs_info;
    // SPIFFS.info(fs_info);
    // Serial.println("max path");
    // Serial.println(fs_info.maxPathLength);


    server.onNotFound([](){
        if(!handleFileRead(server.uri()))
        server.send(404, "text/plain", "FileNotFound");
    });

    server.begin();


    // Port defaults to 8266
    // ArduinoOTA.setPort(8266);

    // Hostname defaults to esp8266-[ChipID]
    // ArduinoOTA.setHostname("myesp8266");


    // No authentication by default
    // ArduinoOTA.setPassword((const char *)"123");

    ArduinoOTA.onStart([]() {
        Serial.println("OTA: Start");
        SPIFFS.end(); //important
        strip_anim.led_anim.clear(CRGB(0,255,0)); //still safe
        FastLED.show();
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nOTA: End");
        //"dangerous": if you reset during flash you have to reflash via serial
        //so dont touch device until restart is complete
        strip_anim.led_anim.clear(CRGB(255,0,0));
        FastLED.show();
        Serial.println("\nOTA: DO NOT RESET OR POWER OFF UNTIL BOOT+FLASH IS COMPLETE.");
        delay(1000);
        ESP.reset();
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {

        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        //getting more "dangerous"
        strip_anim.led_anim.set((progress*LED_COUNT)/total, CRGB(255,255,0));
        FastLED.show();
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("OTA: Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("OTA: Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("OTA: Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("OTA: Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("OTA: End Failed");
    });
    ArduinoOTA.begin();


    Serial.println("boot complete");
}


unsigned long last_micros=0;
int last_wifi_status=-1;

void loop(void){
    // Serial.println("lup");
    server.handleClient();

    if (WiFi.status() != last_wifi_status)
    {
        last_wifi_status=WiFi.status();
        if (last_wifi_status==WL_CONNECTED)
        {
            Serial.print("Wifi connected IP address: ");
            Serial.print(WiFi.localIP());
            Serial.print(", strength: ");
            Serial.println(WiFi.RSSI());
        }
        else
        {
            Serial.print("Wifi disconnected, status:");
            Serial.println(last_wifi_status);
        }

    }

    ArduinoOTA.handle();

    strip_anim.step();

    //limit to 60 fps
    while (micros()-last_micros < 16666);
    // while (micros()-last_micros < 1000000);
    last_micros=micros();

    FastLED.show();


}
