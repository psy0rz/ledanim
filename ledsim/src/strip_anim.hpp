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
    CMD_LED_NR_16,
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
            DEBUG_LOG("commands:" << commands.size());
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

                    //set led number
                    case CMD_LED_NR_8:
                        led=get_next8();
                        DEBUG_LOG("CMD_LED_NR_8: " << led);
                        break;
                    //set led number
                    case CMD_LED_NR_16:
                        led=get_next16();
                        DEBUG_LOG("CMD_LED_NR_16: " << led);
                        break;

                    //set current led to specified color and goes to next led
                    case CMD_LED_SET_NEXT:
                        DEBUG_LOG("CMD_LED_SET_NEXT");
                        led_anim.set(led, CRGB(get_next8(), get_next8(), get_next8()));
                        led=led+1;
                        if (led>= LED_COUNT)
                            led=0;
                    default:
                        DEBUG_LOG("UNKNOWN command " << (int)command << " at pc " << pc);
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
