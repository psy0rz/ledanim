#ifndef UTIL_H
#define UTIL_H

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

#ifdef DEBUG_OUTPUT
#include <iostream>
#define DEBUG_LOG(s) std::cout << s << std::endl;


#else
#define DEBUG_LOG(s)
#endif


/////////////////get random number in range min to max (inclusive)
#include <stdint.h>
uint16_t get_random(uint16_t min, uint16_t max)
{

#ifdef ARDUINO
        return(random(min,max+1));
#endif

#ifdef EMSCRIPTEN
        return((emscripten_random()*(max-min))+min);

#endif

}


///////////////get clock time in ms
#ifdef ARDUINO
#define get_millis millis
#endif

#ifdef EMSCRIPTEN
#define get_millis emscripten_get_now
#endif


#endif
