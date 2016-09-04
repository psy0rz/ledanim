#ifndef UTIL_H
#define UTIL_H

#ifdef DEBUG_OUTPUT
#include <iostream>
#define DEBUG_LOG(s) std::cout << s << std::endl;
#else
#define DEBUG_LOG(s)
#endif


#endif
