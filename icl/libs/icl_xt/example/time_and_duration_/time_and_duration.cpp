/*-----------------------------------------------------------------------------+
Interval Container Library
Author: Joachim Faulhaber
Copyright (c) 2007-2011: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/

/** Example time_and_duration.cpp \file time_and_duration.cpp 
    \brief Times, durations and intervals. 


    \include time_and_duration_/time_and_duration.cpp
*/
//[example_time_and_duration
// The next line includes <boost/date_time/posix_time/posix_time.hpp>
// and a few lines of adapter code.
#include <boost/icl/ptime.hpp> 
#include <iostream>
#include <boost/icl/interval_map.hpp>

using namespace std;
using namespace boost;
using namespace boost::posix_time;
using namespace boost::icl;


/*JODO This is only halfbaked stuff and a collection of some ideas:

This example is intended to show that we can work with
interval<ptime>                               points in time
interval<ptime>::difference_type = duration   time durations as interval lengths
and
interval<duration> intervals of durations, as time object with a 0-element
that is not fixed.

While thinking about the date time stuff I realize that the fixed ptime and
relative duration aspects are only an interpretation with respect to the
parameters origin, resolution and time segmentation, which could be called
time system T=(o,r,s).

Basically we can always compute with integers!
(1) We receive a date/time information as input for a timesystem T=(o,r,s).
(2) We transform time data from T to integer values i.
(3) All computations are made on integer values i.
(4) Finally we can transform the compuation results back to the timesystem
    T=(o,r,s) or a different timesystem T'=(o',r',s'), if needed.

T ---f---> T   We assume, that every operation f:T->T on time types
|          |   can be performed on the equivalent integral type I,
g          g   such that the left diagram commutes.
|          |
V          V
I ---f---> I

Arithmetic on intervals: According to
ptime + duration    -> ptime
duration + ptime    -> ptime
duration + duration -> duration

we could define
interval<ptime> + interval<duration> -> interval<ptime>
etc.
*/

void limits_of_time()
{
    // boost::posix_times are points in time related to the
    // Gregorian calendar. It has a time origin January-1-0, 00:00
    // which is NOT in the scope of its representable values.

    cout << "----- Testing some traits of date_time::posix_time ----------\n";
    // The smallest representable point in time is
    cout << "Technical time origin: " << ptime(min_date_time) << endl;
    // which is also the identity element on ptime
    cout << "Also Null value      : " << icl::identity_element<ptime>::value() << endl;
    // Note, that identity_element<ptime> is added to boost posix_time by 
    // adaptor code in <boost/icl/ptime.hpp>
    
    posix_time::ptime origin = identity_element<ptime>::value();
    ptime next   = origin; ++next;
    cout << "Smallest next time   : " << next << endl;

    cout << "For most applications on time schedules, using resolutions\n"
         << "of minutes or seconds, an integer (4 bytes) would be ok.\n"
         << "boost::ptime being a 'univeral compromise' needs 8 bytes.\n";

    cout << "sizeof(int)           = " << sizeof(int) << endl;
    cout << "sizeof(ptime)         = " << sizeof(ptime) << endl;
    cout << "sizeof(time_duration) = " << sizeof(time_duration) << endl;

    cout << "-------------------------------------------------------------\n";
}

void time_and_duration()
{
    // Start decorating the Christmas tree
    ptime start_deco = time_from_string("2010-12-24 16:30");
    // Ahh! Christmas tree is decorated
    ptime stop_deco = time_from_string("2010-12-24 17:15");

    time_duration decoration_time = stop_deco - start_deco;

    //-----------------------------------------------------------------------------
    cout << "Duration of [" << start_deco << "," << stop_deco << ") = " 
         << decoration_time << endl;  

    interval<ptime>::type deco_2010 
        = interval<ptime>::right_open(start_deco, stop_deco);
    cout << "Duration of " << deco_2010 << " = " << icl::size(deco_2010) << endl;

    // To work with 'relative times' that do not have a fixed origin 
    // we can work with time_durations.

    ptime day_begin = time_from_string("2010-12-24 00:00");
    // It's deco time on Christmas.
    time_duration deco_start = start_deco - day_begin;
    time_duration deco_stop  = stop_deco  - day_begin;

    time_duration deco_mid = decoration_time / 2;

    ptime deco_mid_2010 = start_deco + deco_mid;

    cout << deco_2010 << (icl::contains(deco_2010, deco_mid_2010) ? 
                         " contains " : "does not contain ") 
         << deco_mid_2010 << endl;

    // We can work with intervals of time_durations
    interval<time_duration>::type deco_interval
        = interval<time_duration>::right_open(deco_start, deco_stop);

    cout << "----- We can work with intervals of time_durations: ---------\n";
    cout << "size(" << deco_interval << ") = " << icl::size(deco_interval) << endl;
    cout << "-------------------------------------------------------------\n";
}

void time_and_durations2()
{
    // Some convenient notations for time_durations
    time_duration start = hours(8) + minutes(30);
    time_duration stop  = hours(17) + minutes(15);

    icl::interval<time_duration>::type working_hours 
        = icl::interval<time_duration>::right_open(start, stop);

    cout << "working_hours: " << working_hours << endl;
}

int main()
{
    cout << ">>Interval Container Library: Sample time_and_duration.cpp <<\n";
    cout << "-------------------------------------------------------------\n";
    limits_of_time();
    time_and_duration();
    return 0;
}

// Program output:
/*-----------------------------------------------------------------------------
>>Interval Container Library: Sample time_and_duration.cpp <<
-------------------------------------------------------------
----- Testing some traits of date_time::posix_time ----------
Technical time origin: 1400-Jan-01 00:00:00
Also Null value      : 1400-Jan-01 00:00:00
Smallest next time   : 1400-Jan-01 00:00:00.000001
For most applications on time schedules, using resolutions
of minutes or seconds, an integer (4 bytes) would be ok.
boost::ptime being a 'univeral compromise' needs 8 bytes.
sizeof(int)           = 4
sizeof(ptime)         = 8
sizeof(time_duration) = 8
-------------------------------------------------------------
Duration of [2010-Dec-24 16:30:00,2010-Dec-24 17:15:00) = 00:45:00
Duration of [2010-Dec-24 16:30:00,2010-Dec-24 17:15:00) = 00:45:00
[2010-Dec-24 16:30:00,2010-Dec-24 17:15:00) contains 2010-Dec-24 16:52:30
----- We can work with intervals of time_durations: ---------
size([16:30:00,17:15:00)) = 00:45:00
-------------------------------------------------------------------------------*/
//]

