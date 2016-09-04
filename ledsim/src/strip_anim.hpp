//class responsible for animations over the whole strip
//it has a simple programmaing language to program the animations

#ifndef STRIP_ANIM_H
#define STRIP_ANIM_H

#include <array>
#include <stdint.h>
#include <led_anim.hpp>
#include <vector>


enum led_commands_t
{
    CMD_EOF,         //end of program
    CMD_DELAY_8,      //delay execution, but keep updating ledstrip. (doing fades and stuff)   8 bits delay, in steps
    CMD_DELAY_16,     //delay execution, but keep updating ledstrip. (doing fades and stuff)  16 bits delay. in steps
    CMD_LED_NR_8,
    CMD_LED_SET_NEXT,
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

        //reset strip_anim and restart current program
        void reset()
        {
            pc=0;
            delay=0;
            led=0;
        }

        strip_anim_c()
        {
            commands.push_back(CMD_EOF);
            reset();
        }

        //get next command byte
        uint8_t get_next8()
        {
            uint8_t ret;
            ret=commands[pc];
            pc++;
            if (pc>=commands.size())
                pc=0;

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
            while(1)
            {
                switch (get_next8())
                {
                    //delay execution, but keep updating ledstrip. (doing fades and stuff)   8 bits delay, in steps
                    CMD_DELAY_8:
                        delay=get_next8();
                        return;
                    //delay execution, but keep updating ledstrip. (doing fades and stuff)  16 bits delay. in steps
                    CMD_DELAY_16:
                        delay=get_next16();
                        return;
                    //end of program. update ledstrip and loop.
                    CMD_EOF:
                        pc=0;
                        return;

                    //set led number
                    CMD_LED_NR_8:
                        led=get_next8();
                        break;
                    //set led number
                    CMD_LED_NR_16:
                        led=get_next16();
                        break;

                    //set current led to specified color and goes to next led
                    CMD_LED_SET_NEXT:
                        led_anim.set(led, CRGB(get_next8(), get_next8(), get_next8()));
                        led=led+1;
                        if (led>= LED_COUNT)
                            led=0;


                }
            }
        }

        void step()
        {
            led_anim.step();

            if (delay==0)
            {
                execute_commands();
            }
            else
            {
                delay--;
            }
        }
};





#endif
