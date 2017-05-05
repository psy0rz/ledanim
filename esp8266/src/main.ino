#include "../config.h"

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "FS.h"

#include <MD5.h>


#ifdef FASTLED_CONFIG
#include <FastLED.h>
#endif

#ifdef NEOPIXEL_CONFIG
#include <NeoPixelBus.h>
NeoPixelBus<NEOPIXEL_CONFIG> * neopixel_strip=new NeoPixelBus<NEOPIXEL_CONFIG>(LED_COUNT);
#endif


#include "strip_anim.hpp"



ESP8266WebServer server(80);
strip_anim_c<LED_COUNT> strip_anim;



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

bool smooth=false;
void handle_set_commands()
{

    HTTPUpload& upload = server.upload();
      if (upload.status == UPLOAD_FILE_START)
      {
        // Serial.println("Upload started...");
        // Serial.println(upload.filename);
        // strip_anim.clear();
        strip_anim.add_commands_clear();
      } else if (upload.status == UPLOAD_FILE_WRITE){
        //if(uploadFile) uploadFile.write(upload.buf, upload.currentSize);
        // Serial.print("Upload processing bytes:");
        // Serial.println(upload.currentSize);
        strip_anim.add_commands(upload.buf, upload.currentSize);

      } else if (upload.status == UPLOAD_FILE_END)
      {
        // Serial.println("Upload done, total:");
        // Serial.print(upload.totalSize);
        strip_anim.add_commands_done();

        if (!smooth)
            strip_anim.add_commands_activate(false);
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

// //hash to workaround 32 character path-limit
// String hashed_gz_path(String & path)
// {
//     unsigned char* hash=MD5::make_hash(path.c_str());
//     char *md5str = MD5::make_digest(hash, 13);
//     String ret(md5str);
//     ret="/"+ret+".gz";
//     free(md5str);
//     free(hash);
//     return(ret);
// }

bool handleFileRead(String path)
{
    if(path.endsWith("/"))
    {
        path += "index.html";
    }

    // Serial.println("handleFileRead: " + path);

    String contentType = getContentType(path);

    // path=hashed_gz_path(path);
    // Serial.println("hashed file: " + path);

    if(SPIFFS.exists(path))
    {
        File file = SPIFFS.open(path, "r");
        server.sendHeader("Cache-Control","max-age=86400");
        size_t sent = server.streamFile(file, contentType);
        file.close();
        // server.client().stop();
        return true;
    }
    else
    {
        return false;
    }
}


void return_ok(const __FlashStringHelper* flashString)
{
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(200, "text/plain", String(flashString));
}

void return_ok()
{
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(200, "text/plain", "");
}

void return_error(const __FlashStringHelper* flashString)
{
    server.send(500, "text/plain", String(flashString));
}


//send current strip_anim values to hardware
void send_leds()
{
    #ifdef FASTLED_CONFIG
        //push current led_levels to actual hardware
        FastLED.show();
    #endif

    #ifdef NEOPIXEL_CONFIG
        //OPTIMIZE: too bad we need this extra buffer and copy. change strip_anim to direct pixel manipulation?
        //copy current led_levels to neopixel_strip, which will push it to hardware via DMA
        if (neopixel_strip!=NULL)
        {
            for (int l=0; l<LED_COUNT; l++)
            {
                neopixel_strip->SetPixelColor(l, RgbColor(
                    strip_anim.led_anim.led_level[l].r,
                    strip_anim.led_anim.led_level[l].g,
                    strip_anim.led_anim.led_level[l].b));
            }
            neopixel_strip->Show();
        }
    #endif
}

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
    strip_anim.set_commands(commands, false);

#ifdef FASTLED_CONFIG
    FastLED.addLeds< FASTLED_CONFIG >(strip_anim.led_anim.led_level, LED_COUNT);
    FastLED.setDither(DISABLE_DITHER);
#endif

#ifdef NEOPIXEL_CONFIG
    neopixel_strip->Begin();
#endif

    strip_anim.step();


    if (!SPIFFS.begin())
    Serial.println("SPIFFS: error while mounting");

    wifi_config();


    // load default animation from flash
    File fh=SPIFFS.open("commands.dat", "r+");
    if (fh)
    {
Serial.println("open ok");
        strip_anim.add_commands_clear();

        while(fh.available())
        {
          // Serial.println("byteee");
            uint8_t b;
            b=fh.read();
            strip_anim.add_commands(&b, 1);
        }
        fh.close();
        strip_anim.add_commands_activate(false);
Serial.println("cloost");
    }


    // server.on("/set_commands", handle_set_commands);
    server.on("/set_commands", HTTP_POST, [](){
        smooth=false;
        return_ok();
    }, handle_set_commands);

    server.on("/set_commands", HTTP_OPTIONS, [](){
        return_ok();
    });

    server.on("/set_commands_smooth", HTTP_POST, [](){
        smooth=true;
        return_ok();
    }, handle_set_commands);

    server.on("/set_commands_smooth", HTTP_OPTIONS, [](){
        return_ok();
    });

    //power on/off ATX supply
    pinMode(PIN_POWER_ON, OUTPUT);
    server.on("/off", HTTP_GET, [](){
        digitalWrite(PIN_POWER_ON, 0);
        return_ok();
    });

    server.on("/on", HTTP_GET, [](){
        digitalWrite(PIN_POWER_ON, 1);
        return_ok();
    });

    //save current command string to SPIFFS
    server.on("/save", HTTP_GET, [](){
        // server.send(404, "text/plain", "FileNotFound");
        File fh=SPIFFS.open("commands.dat", "w");
        if (!fh)
        {
            return_error(F("Error creating save file."));
        }
        else
        {
            commands_t::iterator i;
            i=strip_anim.commands.begin();
            while(i!=strip_anim.commands.end())
            {
                if (fh.write(*i)!=1)
                {
                    fh.close();
                    return_error(F("Error while saving."));
                    return;
                }
                i++;
            }
            fh.close();
            return_ok(F("Saved current animation as default."));
        }
    });

    server.onNotFound([](){
        if(!handleFileRead(server.uri()))
        {
            server.send(404, "text/plain", "FileNotFound");
            // server.client().stop();
        }
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
        send_leds();
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nOTA: End");
        //"dangerous": if you reset during flash you have to reflash via serial
        //so dont touch device until restart is complete
        strip_anim.led_anim.clear(CRGB(255,0,0));
        send_leds();
        Serial.println("\nOTA: DO NOT RESET OR POWER OFF UNTIL BOOT+FLASH IS COMPLETE.");
        delay(100);
#ifdef NEOPIXEL_CONFIG
        delete neopixel_strip; //imporatant, otherwise conflicts with flashing i think
        neopixel_strip=NULL;
#endif
        ESP.reset();
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {

        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        //getting more "dangerous"
        strip_anim.led_anim.set((progress*LED_COUNT)/total, CRGB(255,255,0));
        send_leds();
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

    //profiling
    // Serial.print("microseconds used: ");
    // Serial.println(micros()-last_micros);
    // with the last tests i did it was usually lower than 2000uS (for both strips)

    //we work at 60 fps
    while (micros()-last_micros < 16666);
    last_micros=micros();

    send_leds();


}
