//class responsible for animations over the whole strip
//it has a simple programmaing language to program the animations

#ifndef STRIP_ANIM_H
#define STRIP_ANIM_H

#include <array>
#include <stdint.h>
#include <led_anim.hpp>
#include <vector>
#include "util.hpp"


enum led_commands_t
{
    CMD_EOF,         //end of program
    CMD_DELAY_8,      //delay execution, but keep updating ledstrip. (doing fades and stuff)   8 bits delay, in steps
    CMD_DELAY_16,     //delay execution, but keep updating ledstrip. (doing fades and stuff)  16 bits delay. in steps
    CMD_LED_NR_8,
    CMD_LED_NR_8_RND,
    CMD_LED_NR_16,
    CMD_LED_NR_16_RND,
    CMD_LED_SET_NEXT,
    CMD_PEN_COLOR,
    CMD_PEN_COLOR_RND,
    CMD_PEN_WIDTH,
    CMD_PEN_WIDTH_RND,
    CMD_PEN_DRAW,
    CMD_PEN_FADE_MODE,
    CMD_PEN_FADE_SPEED,
    CMD_PEN_FADE_SPEED_RND,
};


enum pen_fade_mode_t
{
    FADE_NONE,
    FADE_TO_FAST,
    FADE_TO_SLOW,
    FADE_FROM_FAST,
    FADE_FROM_SLOW,
};

template <uint16_t LED_COUNT>
class strip_anim_c
{
    public:
        led_anim_c<LED_COUNT> led_anim;

        //commands
        std::vector<uint8_t> commands;

        //current proram counter.
        uint16_t pc;

        //delay until next command
        uint16_t delay;

        //current led number
        uint16_t led;

        //current pen color
        CRGB pen_color;

        //current pen width
        uint16_t pen_width;

        //current pen_skip
        uint16_t pen_skip;

        //current pen fade mode and speed
        uint8_t pen_fade_mode;
        uint8_t pen_fade_speed;


        //reset strip_anim and restart current program
        void reset()
        {
            pc=0;
            delay=0;
            led=0;
            pen_color=CRGB(255,255,255);
            pen_width=1;
            pen_skip=0;
            pen_fade_mode=FADE_NONE;
        }

        strip_anim_c()
        {
            reset();
        }

        //get next command byte
        uint8_t get_next8()
        {
            uint8_t ret;

            if (pc>= commands.size())
                return(CMD_EOF);

            ret=commands[pc];
            pc++;

            // DEBUG_LOG("get_next8: " << ret);

            return(ret);
        }

        //get next command word
        uint16_t get_next16()
        {
            uint16_t ret;
            ret=((uint16_t)get_next8()) << 8;
            ret=ret+get_next8();
            return(ret);
        }


        void execute_commands()
        {
            uint8_t command;
            uint16_t min;
            uint16_t max;

            while(1)
            {
                command=get_next8();
                switch (command)
                {
                    //delay execution, but keep updating ledstrip. (doing fades and stuff)   8 bits delay, in steps
                    case CMD_DELAY_8:
                        delay=get_next8();
                        DEBUG_LOG("CMD_DELAY_8: " << delay);
                        return;

                    //delay execution, but keep updating ledstrip. (doing fades and stuff)  16 bits delay. in steps
                    case CMD_DELAY_16:
                        delay=get_next16();
                        DEBUG_LOG("CMD_DELAY_16: " << delay);
                        return;

                    //end of program. update ledstrip and loop.
                    case CMD_EOF:
                        pc=0;
                        DEBUG_LOG("CMD_EOF");
                        return;

                    //set led number, 8 bits.
                    case CMD_LED_NR_8:
                        led=get_next8();
                        if (led>=LED_COUNT)
                            led=LED_COUNT-1;
                        DEBUG_LOG("CMD_LED_NR_8: " << led);
                        break;

                    //set led number, 8 bits, random
                    case CMD_LED_NR_8_RND:
                        min=get_next8();
                        max=get_next8();
                        if (max>=LED_COUNT)
                            max=LED_COUNT;
                        led=get_random(min,max);
                        DEBUG_LOG("CMD_LED_NR_8_RND: " << led);
                        break;

                    //set led number, 16 bits
                    case CMD_LED_NR_16:
                        led=get_next16();
                        if (led>=LED_COUNT)
                            led=LED_COUNT-1;
                        DEBUG_LOG("CMD_LED_NR_16: " << led);
                        break;

                    //set led number, 16 bits, random
                    case CMD_LED_NR_16_RND:
                        min=get_next16();
                        max=get_next16();
                        if (max>=LED_COUNT)
                            max=LED_COUNT;
                        led=get_random(min,max);
                        DEBUG_LOG("CMD_LED_NR_16_RND: " << led);
                        break;

                    //set current led to specified color and goes to next led
                    case CMD_LED_SET_NEXT:
                        {
                            CRGB rgb;
                            rgb.r=get_next8();
                            rgb.g=get_next8();
                            rgb.b=get_next8();
                            DEBUG_LOG("CMD_LED_SET_NEXT (" << (int)rgb.r << " , " << (int)rgb.g << " , " << (int)rgb.b << ")  led=" << led);
                            led_anim.set(led, rgb);
                            led++;
                            if (led>= LED_COUNT)
                                led=0;
                            break;
                        }

                    //set current pen color
                    case CMD_PEN_COLOR:
                        pen_color.r=get_next8();
                        pen_color.g=get_next8();
                        pen_color.b=get_next8();
                        DEBUG_LOG("CMD_PEN_COLOR (" << (int)pen_color.r << " , " << (int)pen_color.g << " , " << (int)pen_color.b << ")");
                        break;

                    case CMD_PEN_COLOR_RND:
                        min=get_next8();
                        max=get_next8();
                        pen_color.r=get_random(min, max);

                        min=get_next8();
                        max=get_next8();
                        pen_color.g=get_random(min, max);

                        min=get_next8();
                        max=get_next8();
                        pen_color.b=get_random(min, max);
                        DEBUG_LOG("CMD_PEN_COLOR_RND (" << (int)pen_color.r << " , " << (int)pen_color.g << " , " << (int)pen_color.b << ")");
                        break;

                    //set current pen width
                    case CMD_PEN_WIDTH:
                        pen_width=get_next8();
                        DEBUG_LOG("CMD_PEN_WIDTH (" << (int)pen_width << ")");
                        break;

                    case CMD_PEN_WIDTH_RND:
                        min=get_next8();
                        max=get_next8();
                        pen_width=get_random(min,max);
                        DEBUG_LOG("CMD_PEN_WIDTH_RND (" << (int)pen_width << ")");
                        break;

                    //set current fade mode
                    case CMD_PEN_FADE_MODE:
                        pen_fade_mode=get_next8();
                        DEBUG_LOG("CMD_PEN_FADE_MODE (" << (int)pen_fade_mode << ")");
                        break;

                    //set current pen fade speed
                    case CMD_PEN_FADE_SPEED:
                        pen_fade_speed=get_next8();
                        DEBUG_LOG("CMD_PEN_FADE_SPEED (" << (int)pen_fade_speed << ")");
                        break;

                    case CMD_PEN_FADE_SPEED_RND:
                        min=get_next8();
                        max=get_next8();
                        pen_fade_speed=get_random(min,max);
                        DEBUG_LOG("CMD_PEN_FADE_SPEED_RND (" << (int)pen_fade_speed << ")");
                        break;



                    //draw with the current pen-settings
                    case CMD_PEN_DRAW:
                        for (uint16_t i=0; i<pen_width; i++)
                        {
                            switch (pen_fade_mode)
                            {
                                case FADE_NONE:
                                    led_anim.set(led, pen_color);
                                    break;
                                case FADE_TO_FAST:
                                    led_anim.fade_to_fast(led, pen_color, pen_fade_speed);
                                    break;
                                case FADE_FROM_FAST:
                                    led_anim.fade_from_fast(led, pen_color, pen_fade_speed);
                                    break;
                                default:
                                    break;
                            }
                            led++;
                            if (led>=LED_COUNT)
                                led=0;
                        }
                        led=(led+pen_skip)%LED_COUNT;
                        break;


                    default:
                        DEBUG_LOG("UNKNOWN command " << (int)command << " at pc " << pc-1);
                        delay=100;
                        return;

                }
            }
        }

        void step()
        {
            led_anim.step();

            if (delay==0)
            {
                DEBUG_LOG("start execute_commands");
                execute_commands();
                DEBUG_LOG("done execute_commands, delay=" << delay);
            }
            else
            {
                delay--;
            }
        }
};





#endif
