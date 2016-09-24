//class responsible for per-led animations (usually fading and setting)

#ifndef LED_ANIM_H
#define LED_ANIM_H

// #include <iostream>

//fallback for when you're compiling without fastled.h
#include "crgb.h"
#include <stdint.h>



template <uint16_t MAX_LEDS>
class led_anim_c
{
    public:
        uint16_t used_leds=MAX_LEDS;

        //current color levels (this is compatible with FastLED)
        CRGB led_level[MAX_LEDS];

    private:

        //alternate color levels. (used for stuff like fading and blinking)
        CRGB led_alternate_level[MAX_LEDS];

        //led mode
        enum led_mode_t
        {
            MODE_STATIC,
            MODE_FADE_FAST,
            MODE_FADE_SLOW,
        };
        uint8_t led_mode[MAX_LEDS];

        //mode parameters
        uint8_t led_param1[MAX_LEDS];
        uint8_t led_param2[MAX_LEDS];

        uint16_t mirror_size;


    public:
        //constructor
        led_anim_c()
        {
            clear();
            mirror_size=0;

        }

        void set_mirror(uint16_t mirror_size)
        {
            if (mirror_size>=used_leds)
                this->mirror_size=0;
            else
                this->mirror_size=mirror_size;
        }

        //set led to a fixed value without effects
        void set(uint16_t led, CRGB level)
        {
            led_mode[led]=0;
            led_level[led]=level;

        }

        void set_used_leds(uint16_t used_leds)
        {
            this->used_leds=used_leds;
        }

        void clear(CRGB rgb=CRGB(0,0,0))
        {
            set_mirror(0);
            for (int i=0; i<used_leds;i++)
            {
                led_mode[i]=MODE_STATIC;
                led_level[i]=rgb;
            }
        }

        //fade from current to specified level, by stepsize
        void fade_to_fast(uint16_t led, CRGB level, uint8_t stepsize)
        {
            led_alternate_level[led]=level;
            led_mode[led]=MODE_FADE_FAST;
            led_param1[led]=stepsize;
        }

        //fade from specified level back to current to level, by stepsize
        void fade_from_fast(uint16_t led, CRGB level, uint8_t stepsize)
        {
            // already fading to something?
            if (led_mode[led]==MODE_FADE_FAST || led_mode[led]==MODE_FADE_SLOW)
            {
                //keep fading to it, but now from a different level
                led_level[led]=level;
                led_mode[led]=MODE_FADE_FAST;
                led_param1[led]=stepsize;
            }
            else
            {
                led_alternate_level[led]=led_level[led];
                led_level[led]=level;
                led_mode[led]=MODE_FADE_FAST;
                led_param1[led]=stepsize;
            }
        }



    private:
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


    public:
        //pre-execution steps (fading)
        void pre_step()
        {
            for (uint16_t led=0; led<used_leds && (mirror_size==0 || led<mirror_size); led++)
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

        //post execution step (mirroring)
        void post_step()
        {
            if (mirror_size)
            {
                //OPTIMIZE: with memcpy?
                for (uint16_t offset=mirror_size; offset<used_leds; offset=offset+mirror_size)
                {
                    for (uint16_t led=0; led<mirror_size && offset+led<used_leds; led++)
                    {
                        led_level[offset+led]=led_level[led];
                    }
                }
            }
        }

};


#endif
