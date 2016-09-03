#ifndef LED_ANIM_H
#define LED_ANIM_H

#include <array>
#include <iostream>

//fallback for when you're compiling without fastled.h
#include "crgb.h"
#include <stdint.h>



template <uint16_t LED_COUNT>
class led_anim_c
{
    public:

        //current color levels (this is compatible with FastLED)
        CRGB led_level[LED_COUNT];

        //alternate color levels. (used for stuff like fading and blinking)
        CRGB led_alternate_level[LED_COUNT];

        //led mode
        enum led_mode_t
        {
            MODE_STATIC,
            MODE_FADE_FAST,
            MODE_FADE_SLOW,
        };
        uint8_t led_mode[LED_COUNT];

        //mode parameters
        uint8_t led_param1[LED_COUNT];
        uint8_t led_param2[LED_COUNT];


        //constructor
        led_anim_c()
        {
            for (uint16_t led=0; led<LED_COUNT; led++)
            {
                led_level[led].r=0;
                led_level[led].g=0;
                led_level[led].b=0;

                led_mode[led]=MODE_STATIC;
            }

        }


        //fades led_level to led_alternate_level by led_param1 steps on every call.
        void step_fade_fast(uint16_t led)
        {

            bool done=true;
            for (uint8_t color=0; color<3; color++)
            {
                //go up
                if (led_level[led][color] < led_alternate_level[led][color])
                {
                    if (led_alternate_level[led][color] - led_level[led][color] <= led_param1[led])
                    {
                        led_level[led][color]=led_alternate_level[led][color];
                    }
                    else
                    {
                        led_level[led][color]+=led_param1[led];
                    }
                    done=false;
                }
                //go down
                else if (led_level[led][color] > led_alternate_level[led][color])
                {
                    if (led_level[led][color] - led_alternate_level[led][color] <= led_param1[led])
                    {
                        led_level[led][color]=led_alternate_level[led][color];
                    }
                    else
                    {
                        led_level[led][color]-=led_param1[led];
                    }
                    done=false;
                }
            }

            if (done)
                led_mode[led]=MODE_STATIC;

                // std::cout << "led " << led << "rgb" << (int)led_level[led].r << (int)led_level[led].g << (int)led_level[led].b << "\n";

        }


        void step_fade_slow(uint16_t led)
        {

        }


        void step()
        {
            for (uint16_t led=0; led<LED_COUNT; led++)
            {
                switch (led_mode[led])
                {
                    case MODE_STATIC:
                        break;

                    case MODE_FADE_FAST:
                        step_fade_fast(led);
                        break;

                    case MODE_FADE_SLOW:
                        step_fade_slow(led);
                        break;

                    default:
                        break;
                }
            }
        }



};


#endif
