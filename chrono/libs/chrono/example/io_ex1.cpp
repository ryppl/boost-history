//  io_ex1.cpp  ----------------------------------------------------------//

//  Copyright 2010 Howard Hinnant
//  Copyright 2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

/*
This code was adapted by Vicente J. Botet Escriba from Hinnant's html documentation.
Many thanks to Howard for making his code available under the Boost license.

*/

#include <iostream>
#include <boost/chrono/chrono_io.hpp>

int main()
{
    using namespace std;
    using namespace boost;
    using namespace boost::chrono;

    cout << "milliseconds(3) + microseconds(10) = "
         <<  milliseconds(3) + microseconds(10) << '\n';

    cout << "hours(3) + minutes(10) = "
         <<  hours(3) + minutes(10) << '\n';

    typedef duration<long long, ratio<1, 2500000000> > ClockTick;
    cout << "ClockTick(3) + nanoseconds(10) = "
         <<  ClockTick(3) + nanoseconds(10) << '\n';

    cout << "\nSet cout to use short names:\n";
    cout << duration_short;

    cout << "milliseconds(3) + microseconds(10) = "
         <<  milliseconds(3) + microseconds(10) << '\n';

    cout << "hours(3) + minutes(10) = "
         <<  hours(3) + minutes(10) << '\n';

    cout << "ClockTick(3) + nanoseconds(10) = "
         <<  ClockTick(3) + nanoseconds(10) << '\n';

    cout << "\nsystem_clock::now() = " << system_clock::now() << '\n';
#ifdef BOOST_CHRONO_HAS_CLOCK_MONOTONIC
    cout << "monotonic_clock::now() = " << monotonic_clock::now() << '\n';
#endif    
    cout << "\nSet cout to use long names:\n" << duration_long
         << "high_resolution_clock::now() = " << high_resolution_clock::now() << '\n';
    return 0;
}

//~ milliseconds(3) + microseconds(10) = 3010 microseconds
//~ hours(3) + minutes(10) = 190 minutes
//~ ClockTick(3) + nanoseconds(10) = 56 [1/5000000000]seconds

//~ Set cout to use short names:
//~ milliseconds(3) + microseconds(10) = 3010 µs
//~ hours(3) + minutes(10) = 190 m
//~ ClockTick(3) + nanoseconds(10) = 56 [1/5000000000]s

//~ system_clock::now() = 1284923218301231 µs since Jan 1, 1970
//~ monotonic_clock::now() = 18588963676886 ns since boot

//~ Set cout to use long names:
//~ high_resolution_clock::now() = 18588963785548 nanoseconds since boot

//~ milliseconds(3) + microseconds(10) = 3010 microseconds
//~ hours(3) + minutes(10) = 190 minutes
//~ ClockTick(3) + nanoseconds(10) = 56 [1/5000000000]seconds

//~ Set cout to use short names:
//~ milliseconds(3) + microseconds(10) = 3010 Âµs
//~ hours(3) + minutes(10) = 190 m
//~ ClockTick(3) + nanoseconds(10) = 56 [1/5000000000]s

//~ system_clock::now() = 1285041215052875000 ns since Jan 1, 1970
//~ monotonic_clock::now() = 2793 ns since boot

//~ Set cout to use long names:
//~ high_resolution_clock::now() = 8660 nanoseconds since boot

