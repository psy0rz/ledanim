//(C)2016 Edwin Eefting - edwin@datux.nl
//class responsible for animations over the whole strip
//it has a simple programmaing language to program the animations

#ifndef STRIP_ANIM_H
#define STRIP_ANIM_H

#include <array>
#include <stdint.h>
#include "led_anim.hpp"
#include <vector>
#include "util.hpp"


enum led_commands_t
{
    CMD_EOF=0,         //end of program
    CMD_UPDATE=1,
    CMD_DELAY_8=2,      //delay execution, but keep updating ledstrip. (doing fades and stuff)   8 bits delay, in steps
    CMD_DELAY_16=3,     //delay execution, but keep updating ledstrip. (doing fades and stuff)  16 bits delay. in steps
    CMD_INTERVAL_16=24, //delay execution, until next absolute time interval period (absolute timing, usefull for syncing to music)

    CMD_REPEAT_BEGIN=4,
    CMD_REPEAT_BEGIN_RND=5,
    CMD_REPEAT_END=6,

    CMD_LED_NR_8=7,
    CMD_LED_NR_8_RND=8,
    CMD_LED_NR_16=9,
    CMD_LED_NR_16_RND=10,
    CMD_LED_SET_NEXT=11,
    CMD_LED_MIRROR=23,

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
    FADE_NONE=0,
    FADE_TO_FAST=1,
    FADE_FROM_FAST=2,
    FADE_TO_SLOW=3,
    FADE_FROM_SLOW=4,
};

typedef std::vector<uint8_t> commands_t;

template <uint16_t MAX_LEDS>
class strip_anim_c
{
    public:
        led_anim_c<MAX_LEDS> led_anim;

    private:
        uint16_t used_leds=MAX_LEDS;

        bool stopped;

        //commands
        commands_t commands;
        commands_t commands_new;
        bool new_ready;


        //current proram counter.
        uint16_t pc;

        uint16_t repeat_count;
        uint16_t repeat_begin;

        //delay until next command, in steps (for on frame steps)
        uint16_t delay;
        //delay until systemclock reaches this time (for syncing with real-world time)
        unsigned long delay_until;

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

    public:
        CRGB & get_led(uint16_t led)
        {
            return (led_anim.led_level[led]);
        }


        //reset strip_anim and clear current program
        void reset(bool smooth=false)
        {
            stop();
            commands.clear();
            pc=0;
            delay=0;
            delay_until=0;
            led=0;
            pen_color=CRGB(255,255,255);
            pen_width=1;
            pen_step=1;
            pen_fade_mode=FADE_NONE;
            pen_fade_speed=1;
            repeat_begin=0;
            repeat_count=0;
            pen_clone_offset=0;
            pen_clone_count=0;
            if (smooth)
                led_anim.clear_smooth();
            else
                led_anim.clear();
        }

        void start()
        {
            stopped=false;
        }


        void stop()
        {
            stopped=true;
        }

        strip_anim_c()
        {
            reset();
        }

        void set_used_leds(uint16_t used_leds)
        {
            this->used_leds=used_leds;
        }

        void set_commands(commands_t commands, bool smooth)
        {
            if (smooth)
            {
                //finish current program and start at a nice point for smooter transition
                this->commands_new=commands;
                this->add_commands_done();
            }
            else
            {
                //abort current animation and start now
                reset();
                this->commands=commands;
                start();
            }
        }


        //stops program, resets and clears command buffer
        void clear()
        {
            reset();
            this->commands.clear();
        }
        //clear new-command buffer
        void add_commands_clear()
        {
            new_ready=false;
            commands_new.clear();
        }

        //indicate new commands are ready. it will be activated when pc==0 or when add_commands_activate is called.
        void add_commands_done()
        {
            new_ready=true;
        }

        void add_commands_activate(bool smooth)
        {
            reset(smooth);
            commands=commands_new;
            new_ready=false;
            commands_new.clear();
            start();
        }

        //add commands to new commands buffer, use add_commands_done to activate
        void add_commands(uint8_t * buf, int buf_size)
        {
            for (int i=0; i<buf_size; i++)
            {
                commands_new.push_back(buf[i]);
            }
        }

        //get next command byte
        inline uint8_t get_next8()
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



        //we could have made coded the interpreter more abstract, but that would probably lead to slower execution.
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
                    //end of program. update ledstrip and loop.
                    case CMD_EOF:
                        pc=0;
                        DEBUG_LOG("CMD_EOF");
                        return;

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

                    //delay until next interval
                    case CMD_INTERVAL_16:
                        delay_until=get_next16();
                        //calculate next absolute interval point
                        delay_until=((get_millis()/delay_until) * delay_until) + delay_until;
                        DEBUG_LOG("CMD_INTERVAL_16: " << delay_until);
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
                        if (led>=used_leds)
                            led=used_leds-1;
                        DEBUG_LOG("CMD_LED_NR_8: " << led);
                        break;

                    //set led number, 8 bits, random
                    case CMD_LED_NR_8_RND:
                        min=get_next8();
                        max=get_next8();
                        if (max>=used_leds)
                            max=used_leds-1;
                        led=get_random(min,max);
                        DEBUG_LOG("CMD_LED_NR_8_RND: " << led);
                        break;

                    //set led number, 16 bits
                    case CMD_LED_NR_16:
                        led=get_next16();
                        if (led>=used_leds)
                            led=used_leds-1;
                        DEBUG_LOG("CMD_LED_NR_16: " << led);
                        break;


                    case CMD_LED_MIRROR:
                        led_anim.set_mirror(get_next16());
                        DEBUG_LOG("CMD_LED_SET_MIRROR");
                        break;

                    //set led number, 16 bits, random
                    case CMD_LED_NR_16_RND:
                        min=get_next16();
                        max=get_next16();
                        if (max>=used_leds)
                            max=used_leds-1;
                        led=get_random(min,max);
                        DEBUG_LOG("CMD_LED_NR_16_RND: " << led);
                        break;

                    //set current led to specified color and goes to next led (direction will be pen_step)
                    //this is more lowlevel, and doesnt use most effects
                    //its a way to make complex graphics with only 4 bytes per pixel.
                    case CMD_LED_SET_NEXT:
                        {
                            CRGB rgb;
                            rgb.r=get_next8();
                            rgb.g=get_next8();
                            rgb.b=get_next8();
                            DEBUG_LOG("CMD_LED_SET_NEXT (" << (int)rgb.r << " , " << (int)rgb.g << " , " << (int)rgb.b << ")  led=" << led);
                            led_anim.set(led, rgb);
                            led=(led+pen_step)%used_leds;
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
                        DEBUG_LOG("CMD_PEN_CLONE_OFFSET (" << (int)pen_clone_offset << ")");
                        break;

                    case CMD_PEN_CLONE_COUNT:
                        pen_clone_count=get_next16();
                        DEBUG_LOG("CMD_PEN_CLONE_COUNT (" << (int)pen_clone_count << ")");
                        break;


                    //draw with the current pen-settings
                    case CMD_PEN_DRAW:
                        {
                            uint16_t tmp_led=led;
                            for (uint16_t clone_count=0; (clone_count<pen_clone_count+1) ; clone_count++)
                            {
                                for (uint16_t width_count=0; (width_count<pen_width) ; width_count++)
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
                                }
                                tmp_led=(led+pen_clone_offset*(clone_count+1))%used_leds;
                            }
                        }
                        led=(led+pen_step)%used_leds;
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
            //activate new commands?
            if (pc==0 && new_ready)
            {
                add_commands_activate(true);
            }

            if (stopped)
                return;

            led_anim.pre_step();

            if (delay==0 && (delay_until==0 || get_millis()>delay_until))
            {
                delay_until=0;
                DEBUG_LOG("start execute_commands");
                execute_commands();
                DEBUG_LOG("done execute_commands, delay=" << delay);
            }
            else
            {
                if (delay)
                    delay--;
            }

            led_anim.post_step();


        }
};





#endif
