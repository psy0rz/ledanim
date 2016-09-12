#ifndef UTIL_H
#define UTIL_H


#ifdef DEBUG_OUTPUT
#include <iostream>
#define DEBUG_LOG(s) std::cout << s << std::endl;


#else
#define DEBUG_LOG(s)
#endif


//get random number in range min to max (inclusive)
#include <stdint.h>
uint16_t get_random(uint16_t min, uint16_t max)
{

#ifdef ARDUINO
        return(random(min,max+1));
#else
#include <stdlib.h>
        long int rnd=random();

        rnd=rnd % (max-min+1);
        return (rnd+min);
#endif
}


#endif
