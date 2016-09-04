#ifndef UTIL_H
#define UTIL_H

#ifdef DEBUG_OUTPUT
#include <iostream>
#define DEBUG_LOG(s) std::cout << s << std::endl;
#else
#define DEBUG_LOG(s)
#endif

#include <stdlib.h>
#include <stdint.h>
//get random number in range min to max (inclusive)
uint16_t get_random(uint16_t min, uint16_t max)
{
        long int rnd=random();

        rnd=rnd % (max-min+1);
        return (rnd+min);
}


#endif
