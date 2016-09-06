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
    CMD_EOF=0,         //end of program
    CMD_UPDATE=1,
    CMD_DELAY_8=2,      //delay execution, but keep updating ledstrip. (doing fades and stuff)   8 bits delay, in steps
    CMD_DELAY_16=3,     //delay execution, but keep updating ledstrip. (doing fades and stuff)  16 bits delay. in steps

    CMD_REPEAT_BEGIN=4,
    CMD_REPEAT_BEGIN_RND=5,
    CMD_REPEAT_END=6,

    CMD_LED_NR_8=7,
    CMD_LED_NR_8_RND=8,
    CMD_LED_NR_16=9,
    CMD_LED_NR_16_RND=10,
    CMD_LED_SET_NEXT=11,
    CMD_PEN_COLOR=12,
    CMD_PEN_COLOR_RND=13,
    CMD_PEN_STEP=14,
    CMD_PEN_WIDTH=15,
    CMD_PEN_WIDTH_RND=16,
    CMD_PEN_DRAW=17,
    CMD_PEN_FADE_MODE=18,
    CMD_PEN_FADE_SPEED=19,
    CMD_PEN_FADE_SPEED_RND=20,
    CMD_PEN_CLONE_COUNT=21,
    CMD_PEN_CLONE_OFFSET=22,

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

        uint16_t repeat_count;
        uint16_t repeat_begin;

        //delay until next command
        uint16_t delay;

        //current led number
        uint16_t led;

        //current pen color
        CRGB pen_color;

        //current pen width
        uint16_t pen_width;

        //clone pen every X leds
        uint16_t pen_clone_offset;
        uint16_t pen_clone_count;


        //current pen_step
        int8_t pen_step;

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
            pen_step=1;
            pen_fade_mode=FADE_NONE;
            repeat_begin=0;
            repeat_count=0;
            pen_clone_offset=0;
            pen_clone_count=0;
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
                    //send output and continue execution. execution continues when step-time is complete.
                    case CMD_UPDATE:
                        DEBUG_LOG("CMD_UPDATE");
                        return;

                    //send output and delay execution. (fades keep going)
                    case CMD_DELAY_8:
                        delay=get_next8();
                        DEBUG_LOG("CMD_DELAY_8: " << delay);
                        return;

                    //16 bits
                    case CMD_DELAY_16:
                        delay=get_next16();
                        DEBUG_LOG("CMD_DELAY_16: " << delay);
                        return;



                    //end of program. update ledstrip and loop.
                    case CMD_EOF:
                        pc=0;
                        DEBUG_LOG("CMD_EOF");
                        return;

                    //start of a repeating loop
                    case CMD_REPEAT_BEGIN:
                        repeat_count=get_next16();
                        repeat_begin=pc;
                        break;

                    case CMD_REPEAT_BEGIN_RND:
                        min=get_next16();
                        max=get_next16();
                        repeat_count=get_random(min,max);
                        repeat_begin=pc;
                        break;

                    //end of repeating loop, jump to beginning until repeat_count is 0
                    case CMD_REPEAT_END:
                        if (repeat_count>0)
                        {
                            pc=repeat_begin;
                            repeat_count--;
                        }
                        break;

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

                    //set current led to specified color and goes to next led (direction will be pen_step)
                    case CMD_LED_SET_NEXT:
                        {
                            CRGB rgb;
                            rgb.r=get_next8();
                            rgb.g=get_next8();
                            rgb.b=get_next8();
                            DEBUG_LOG("CMD_LED_SET_NEXT (" << (int)rgb.r << " , " << (int)rgb.g << " , " << (int)rgb.b << ")  led=" << led);
                            led_anim.set(led, rgb);
                            led=(led+pen_step)%LED_COUNT;
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

                    //set current pen step size, can be negative to go to the left!
                    case CMD_PEN_STEP:
                        pen_step=get_next8();
                        DEBUG_LOG("CMD_PEN_STEP (" << (int)pen_step << ")");
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

                    //set current pen clone count and offset.
                    case CMD_PEN_CLONE_OFFSET:
                        pen_clone_offset=get_next16();
                        DEBUG_LOG("CMD_PEN_CLONE_SIZE (" << (int)pen_clone_offset << ")");
                        break;

                    case CMD_PEN_CLONE_COUNT:
                        pen_clone_count=get_next16();
                        DEBUG_LOG("CMD_PEN_CLONE_COUNT (" << (int)pen_clone_count << ")");
                        break;


                    //draw with the current pen-settings
                    case CMD_PEN_DRAW:
                        {
                            uint16_t tmp_led=led;
                            for (uint16_t clone_count=0; clone_count<pen_clone_count+1; clone_count++)
                            {
                                for (uint16_t width_count=0; width_count<pen_width; width_count++)
                                {
                                    switch (pen_fade_mode)
                                    {
                                        case FADE_NONE:
                                            led_anim.set(tmp_led, pen_color);
                                            break;
                                        case FADE_TO_FAST:
                                            led_anim.fade_to_fast(tmp_led, pen_color, pen_fade_speed);
                                            break;
                                        case FADE_FROM_FAST:
                                            led_anim.fade_from_fast(tmp_led, pen_color, pen_fade_speed);
                                            break;
                                        default:
                                            break;
                                    }
                                    tmp_led++;
                                    if (tmp_led>=LED_COUNT)
                                        tmp_led=0;
                                }
                                tmp_led=(led+pen_clone_offset*(clone_count+1))%LED_COUNT;
                            }
                        }
                        led=(led+pen_step)%LED_COUNT;
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
