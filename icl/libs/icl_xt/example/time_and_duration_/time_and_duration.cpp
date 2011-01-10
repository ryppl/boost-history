/*-----------------------------------------------------------------------------+
Interval Container Library
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
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
void time_and_duration()
{
	// boost::posix_times are points in time related to the
	// Gregorian calendar. It has a time origin January-1-0, 00:00
	// which is NOT in the scope of its representable values.

	// The smallest representable point in time is
	cout << "Technical time origin: " << ptime(min_date_time) << endl;
	// which is also the identity element on ptime
	cout << "Also Null value      : " << icl::identity_element<ptime>::value() << endl;
	// Note, that identity_element<ptime> is added to boost posix_time by 
	// adaptor code in <boost/icl/ptime.hpp>
	
	posix_time::ptime origin = identity_element<ptime>::value();
	ptime next   = origin; ++next;
	cout << "Smallest next time   : " << next << endl;

	// Start decorating the Christmas tree
    ptime start_deco = time_from_string("2010-12-24 16:30");
	// Ahh! Christmas tree is decorated
    ptime stop_deco = time_from_string("2010-12-24 17:15");

    time_duration decoration_time = stop_deco - start_deco;

	//-----------------------------------------------------------------------------
	cout << "sizeof(int) = " << sizeof(int) << endl;
	cout << "sizeof(ptime) = " << sizeof(ptime) << endl;
	cout << "sizeof(time_duration) = " << sizeof(time_duration) << endl;

	//-----------------------------------------------------------------------------
	//JODO Make it a nice example
	ptime t_a = time_from_string("2010-12-24 19:30");
    ptime t_b = time_from_string("2010-12-24 20:30");
    time_duration a_2_b = t_b - t_a;
    cout << "Duration of [" << t_a << "," << t_b << ") = " 
         << a_2_b << endl;  

    interval<ptime>::type a_b 
        = interval<ptime>::right_open(t_a, t_b);
    cout << "Duration of " << a_b << " = " 
         << icl::size(a_b) << endl;

	icl::size(a_b);

	posix_time::time_duration half = a_2_b / 2;
    ptime t_m = t_a + half;

    cout << a_b << (icl::contains(a_b, t_m) ? 
                    " contains " : "does not contain ") 
         << t_m << endl;

	ptime day_begin = time_from_string("2010-12-24 00:00");
	time_duration d_a = t_a - day_begin;
	time_duration d_b = t_b - day_begin;
	time_duration d_m = d_b - d_a;

	interval<time_duration>::type rel_a_b 
        = interval<time_duration>::right_open(d_a, d_b);

	time_duration sz_a_b_1 = d_b - d_a;
	time_duration sz_a_b_2 = icl::size(rel_a_b);
	cout << "b-a         = " << sz_a_b_1 << endl;
	cout << "size([a,b)) = " << sz_a_b_2 << endl;
	cout << "size([a,b)) = " << (icl::size(rel_a_b)) << endl;

	cout << rel_a_b << " " << (icl::size(rel_a_b)) << endl;
}

void time_durations()
{
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
    //time_and_duration();
	time_durations();
    return 0;
}

// Program output:
/*-----------------------------------------------------------------------------
>>Interval Container Library: Sample time_and_duration.cpp <<
-------------------------------------------------------------
TODO Show prgram output.
-----------------------------------------------------------------------------*/
//]

