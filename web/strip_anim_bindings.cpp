#include <emscripten/bind.h>

// #define DEBUG_OUTPUT
#include "strip_anim.hpp"
#include "crgb.h"
#include <stdint.h>

#define MAX_LEDS 200


EMSCRIPTEN_BINDINGS(strip_anim_bindings) {
    emscripten::constant("MAX_LEDS", MAX_LEDS);

    emscripten::value_object<CRGB>("CRGB")
        .field("r", &CRGB::r)
        .field("g", &CRGB::g)
        .field("b", &CRGB::b)
        ;

    emscripten::register_vector<uint8_t>("commands_t");


    emscripten::class_<strip_anim_c<MAX_LEDS>  >("strip_anim_c")
        .constructor<>()
        .function("step", &strip_anim_c<MAX_LEDS>::step)
        .function("get_led", &strip_anim_c<MAX_LEDS>::get_led)
        .function("set_commands", &strip_anim_c<MAX_LEDS>::set_commands)
        ;
}
