#include <emscripten/bind.h>

#include "strip_anim.hpp"
#include "crgb.h"

#define MAX_LEDS 200


EMSCRIPTEN_BINDINGS(strip_anim_bindings) {
    emscripten::constant("MAX_LEDS", MAX_LEDS);

    emscripten::value_object<CRGB>("CRGB")
        .field("r", &CRGB::r)
        .field("g", &CRGB::g)
        .field("b", &CRGB::b)
        ;



    emscripten::class_<strip_anim_c<MAX_LEDS>  >("strip_anim_c")
        .constructor<>()
        .function("step", &strip_anim_c<MAX_LEDS>::step)
        .function("get_led", &strip_anim_c<MAX_LEDS>::get_led)
        ;

}
