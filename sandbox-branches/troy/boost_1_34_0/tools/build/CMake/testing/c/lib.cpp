#include <iostream>
#include <string>
#include <boost/preprocessor/stringize.hpp>

// we should see these
#ifndef C_COMPILE_FLAGS
#error C_COMPILE_FLAGS
#endif

#ifndef A_COMPILE_REQUIREMENTS
#error A_COMPILE_REQUIREMENTS
#endif

/// should NOT see these
#ifdef A_COMPILE_FLAGS
#error A_COMPILE_FLAGS
#endif

#ifdef B_COMPILE_FLAGS
#error B_COMPILE_FLAGS
#endif
#ifdef B_STICKY_COMPILE_FLAGS
#error B_STICKY_COMPILE_FLAGS
#endif

#include "a.hpp"

#include <time.h>

std::string c_linktype() { a_linktype(); return BOOST_PP_STRINGIZE(LINKTYPE); }

// this one causes a link requirement (-lrt) on linux
void f()
{
    // this is never called, it just has to compile:
    timespec tp;
    int res = clock_gettime(CLOCK_REALTIME, &tp);
    (void) &res;
}


void foo()
{
  
}
