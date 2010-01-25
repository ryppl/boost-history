//  test_system_clock.cpp  ----------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#include <boost/chrono/chrono.hpp>
#include <boost/chrono/suspendible_clock.hpp>
#include <boost/type_traits.hpp>

#include <iostream>

using namespace boost::chrono;

void test_system_clock()
{
    std::cout << "suspendible_clock<system_clock> test" << std::endl;
    suspendible_clock<system_clock>::duration delay = milliseconds(5);
    suspendible_clock<system_clock>::time_point start = suspendible_clock<system_clock>::now();
    //std::cout << "start: " << start.time_since_epoch().count() << " nanoseconds\n";
    //std::cout << "start: " << nanoseconds(start.time_since_epoch()).count() << " nanoseconds\n";
    //std::cout << "Clock::now: " << nanoseconds(system_clock::now().time_since_epoch()).count() << " nanoseconds\n";
    //std::cout << "now: " << nanoseconds(suspendible_clock<system_clock>::now().time_since_epoch()).count() << " nanoseconds\n";
    while (suspendible_clock<system_clock>::now() - start <= delay) {
    //std::cout << "delta: " << (suspendible_clock<system_clock>::now() - start).count() << " nanoseconds\n";
    //std::cout << "Clock::now: " << nanoseconds(system_clock::now().time_since_epoch()).count() << " nanoseconds\n";
    //std::cout << "now: " << nanoseconds(suspendible_clock<system_clock>::now().time_since_epoch()).count() << " nanoseconds\n";
        
    }
        
    suspendible_clock<system_clock>::time_point stop = suspendible_clock<system_clock>::now();
    suspendible_clock<system_clock>::duration elapsed = stop - start;
    std::cout << "paused " << nanoseconds(elapsed).count() << " nanoseconds\n";
    start = suspendible_clock<system_clock>::now();
    stop = suspendible_clock<system_clock>::now();
    std::cout << "suspendible_clock<system_clock> resolution estimate: " << nanoseconds(stop-start).count() << " nanoseconds\n";
    
    start = suspendible_clock<system_clock>::now();
    suspendible_clock<system_clock>::suspend();
    system_clock::duration delay3 = milliseconds(50);
    system_clock::time_point start3 = system_clock::now();
    while (system_clock::now() - start3 <= delay3)
        ;
    system_clock::time_point stop3 = system_clock::now();
    stop = suspendible_clock<system_clock>::now();
    std::cout << "system_clock 50ms: " << nanoseconds(stop3-start3).count() << " nanoseconds\n";
    std::cout << "suspendible_clock<system_clock> 50ms: " << nanoseconds(stop-start).count() << " nanoseconds\n";
    suspendible_clock<system_clock>::resume();   
    
}

void test_monotonic_clock()
{
#ifdef BOOST_CHRONO_HAS_CLOCK_MONOTONIC 
    std::cout << "monotonic_clock test" << std::endl;
    monotonic_clock::duration delay = milliseconds(5);
    monotonic_clock::time_point start = monotonic_clock::now();
    while (monotonic_clock::now() - start <= delay)
        ;
    monotonic_clock::time_point stop = monotonic_clock::now();
    monotonic_clock::duration elapsed = stop - start;
    std::cout << "paused " << nanoseconds(elapsed).count() << " nanoseconds\n";
    start = monotonic_clock::now();
    stop = monotonic_clock::now();
    std::cout << "monotonic_clock resolution estimate: " << nanoseconds(stop-start).count() << " nanoseconds\n";
#endif
}
void test_hi_resolution_clock()
{
    std::cout << "high_resolution_clock test" << std::endl;
    high_resolution_clock::duration delay = milliseconds(5);
    high_resolution_clock::time_point start = high_resolution_clock::now();
    while (high_resolution_clock::now() - start <= delay)
      ;
    high_resolution_clock::time_point stop = high_resolution_clock::now();
    high_resolution_clock::duration elapsed = stop - start;
    std::cout << "paused " << nanoseconds(elapsed).count() << " nanoseconds\n";
    start = high_resolution_clock::now();
    stop = high_resolution_clock::now();
    std::cout << "high_resolution_clock resolution estimate: " << nanoseconds(stop-start).count() << " nanoseconds\n";
}

//void test_mixed_clock()
//{
//    std::cout << "mixed clock test" << std::endl;
//    high_resolution_clock::time_point hstart = high_resolution_clock::now();
//    std::cout << "Add 5 milliseconds to a high_resolution_clock::time_point\n";
//    monotonic_clock::time_point mend = hstart + milliseconds(5);
//    bool b = hstart == mend;
//    system_clock::time_point sstart = system_clock::now();
//    std::cout << "Subtracting system_clock::time_point from monotonic_clock::time_point doesn't compile\n";
////  mend - sstart; // doesn't compile
//    std::cout << "subtract high_resolution_clock::time_point from monotonic_clock::time_point"
//            " and add that to a system_clock::time_point\n";
//    system_clock::time_point send = sstart + duration_cast<system_clock::duration>(mend - hstart);
//    std::cout << "subtract two system_clock::time_point's and output that in microseconds:\n";
//    microseconds ms = send - sstart;
//    std::cout << ms.count() << " microseconds\n";
//}
//
//void test_c_mapping()
//{
//    std::cout << "C map test\n";
//    using namespace boost::chrono;
//    system_clock::time_point t1 = system_clock::now();
//    std::time_t c_time = system_clock::to_time_t(t1);
//    std::tm* tmptr = std::localtime(&c_time);
//    std::cout << "It is now " << tmptr->tm_hour << ':' << tmptr->tm_min << ':' << tmptr->tm_sec << ' '
//              << tmptr->tm_year + 1900 << '-' << tmptr->tm_mon + 1 << '-' << tmptr->tm_mday << '\n';
//    c_time = std::mktime(tmptr);
//    system_clock::time_point t2 = system_clock::from_time_t(c_time);
//    microseconds ms = t1 - t2;
//    std::cout << "Round-tripping through the C interface truncated the precision by " << ms.count() << " microseconds\n";
//}


int main()
{
    test_system_clock();
    //test_monotonic_clock();
    //test_hi_resolution_clock();
    //test_mixed_clock();
    return 0;
}

