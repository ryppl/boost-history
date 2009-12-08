//  mac/chrono.cpp  --------------------------------------------------------------//

//  Copyright Beman Dawes 2008

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


//----------------------------------------------------------------------------//
//                                 Mac                                        //
//----------------------------------------------------------------------------//

#include <sys/time.h> //for gettimeofday and timeval
#include <mach/mach_time.h>  // mach_absolute_time, mach_timebase_info_data_t

namespace boost
{
namespace chrono
{

// system_clock

// gettimeofday is the most precise "system time" available on this platform.
// It returns the number of microseconds since New Years 1970 in a struct called timeval
// which has a field for seconds and a field for microseconds.
//    Fill in the timeval and then convert that to the time_point
system_clock::time_point
system_clock::now()
{
    timeval tv;
    gettimeofday(&tv, 0);
    return time_point(seconds(tv.tv_sec) + microseconds(tv.tv_usec));
}

system_clock::time_point
system_clock::now(system::error_code & ec)
{
    timeval tv;
    gettimeofday(&tv, 0);
    ec.clear();
    return time_point(seconds(tv.tv_sec) + microseconds(tv.tv_usec));
}

// Take advantage of the fact that on this platform time_t is nothing but
//    an integral count of seconds since New Years 1970 (same epoch as timeval).
//    Just get the duration out of the time_point and truncate it to seconds.
time_t
system_clock::to_time_t(const time_point& t)
{
    return time_t(duration_cast<seconds>(t.time_since_epoch()).count());
}

// Just turn the time_t into a count of seconds and construct a time_point with it.
system_clock::time_point
system_clock::from_time_t(time_t t)
{
    return system_clock::time_point(seconds(t));
}

// monotonic_clock

// Note, in this implementation monotonic_clock and high_resolution_clock
//   are the same clock.  They are both based on mach_absolute_time().
//   mach_absolute_time() * MachInfo.numer / MachInfo.denom is the number of
//   nanoseconds since the computer booted up.  MachInfo.numer and MachInfo.denom
//   are run time constants supplied by the OS.  This clock has no relationship
//   to the Gregorian calendar.  It's main use is as a high resolution timer.

// MachInfo.numer / MachInfo.denom is often 1 on the latest equipment.  Specialize
//   for that case as an optimization.
static
monotonic_clock::rep
monotonic_simplified()
{
    return mach_absolute_time();
}

static
monotonic_clock::rep
monotonic_simplifie_ec(system::error_code & ec)
{
    return mach_absolute_time();
}


static
double
compute_monotonic_factor(kern_return_t& err)
{
    mach_timebase_info_data_t MachInfo;
    err = mach_timebase_info(&MachInfo);
    if( err != 0  ) {
	    return 0;
    }
    return static_cast<double>(MachInfo.numer) / MachInfo.denom;
}

static
monotonic_clock::rep
monotonic_full()
{
    static kern_return_t err;
    static const double factor = compute_monotonic_factor(err);
    if (err != 0) 
      boost::throw_exception(
        system::system_error( err, system::system_category, "chrono::monotonic_clock" ));	    
    return static_cast<monotonic_clock::rep>(mach_absolute_time() * factor);
}

static
monotonic_clock::rep
monotonic_full_ec(system::error_code & ec)
{
    static kern_return_t err;
    static const double factor = compute_monotonic_factor(err);
    if (err != 0) {
      ec.assign( errno, system::system_category );
      return monotonic_clock::rep();
    } 
    return static_cast<monotonic_clock::rep>(mach_absolute_time() * factor);
}

typedef monotonic_clock::rep (*FP)();
typedef monotonic_clock::rep (*FP_ec)(system::error_code &);


static
FP_ec
init_monotonic_clock(kern_return_t & err)
{
    mach_timebase_info_data_t MachInfo;
    err = mach_timebase_info(&MachInfo);
    if( err != 0  ) {
	return 0;
    }

    if (MachInfo.numer == MachInfo.denom)
        return &monotonic_simplified_ec;
    return &monotonic_full_ec;
}

static
FP_ec
init_monotonic_clock_ec(kern_return_t & err)
{
    mach_timebase_info_data_t MachInfo;
    err = mach_timebase_info(&MachInfo);
    if( err != 0  ) {
	return 0;
    }

    if (MachInfo.numer == MachInfo.denom)
        return &monotonic_simplified_ec;
    return &monotonic_full_ec;
}

monotonic_clock::time_point
monotonic_clock::now()
{
    static kern_return_t err;
    static FP fp = init_monotonic_clock(err);
    if( err != 0  ) 	boost::throw_exception(
        system::system_error( err, system::system_category, "chrono::monotonic_clock" ));	    
    return time_point(duration(fp()));
}

monotonic_clock::time_point
monotonic_clock::now(system::error_code & ec)
{
    static kern_return_t err;
    static FP_ec fp = init_monotonic_clock(err);
    if( err != 0  ) {
        ec.assign( err, system::system_category );
	return time_point();
    }
    return time_point(duration(fp(ec)));
}

}  // namespace chrono
}  // namespace boost
