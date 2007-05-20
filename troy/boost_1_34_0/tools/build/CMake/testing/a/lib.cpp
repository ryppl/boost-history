#include <iostream>
#include <string>
#include <boost/preprocessor/stringize.hpp>
#ifndef A_COMPILE_FLAGS
#error A_COMPILE_FLAGS
#endif

#include <pthread.h>

// dont check (sticky)static/shared here, this gets compiled twice.
// tests check that things worked right via LINKTYPE

std::string a_linktype() { return BOOST_PP_STRINGIZE(LINKTYPE); }

void a_need_pthread()
{
    // this is never called, it just has to compile:
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    int type = 0;
    pthread_mutexattr_settype(&attr, type);
}

