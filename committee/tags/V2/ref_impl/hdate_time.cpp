// Copyright Howard Hinnant 2007. Distributed under the Boost
// Software License, Version 1.0. (see http://www.boost.org/LICENSE_1_0.txt)

// hdate_time.cpp

#define _POSIX_C_SOURCE 200112L
#include "hdate_time"
#include <unistd.h>
#include <time.h> //for clock_gettime and timespec
#include <sys/time.h> //for gettimeofday and timeval

namespace std {

system_time get_system_time() 
{
#if _POSIX_TIMERS >= 0
	if (sysconf(_SC_TIMERS) == -1) {
#endif
		timeval tv;
		gettimeofday(&tv, 0);
		return system_time(tv.tv_sec, tv.tv_usec * 1000LL);
#if _POSIX_TIMERS >= 0
	} else {
		timespec tv;
		clock_gettime(CLOCK_REALTIME, &tv);
		return system_time(tv.tv_sec, tv.tv_nsec);
	}
#endif
}

}  // std
