//  suspendible_stopclock_example.cpp  ---------------------------------------------------//
//  Copyright 2009-2011 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt
//  See http://www.boost.org/libs/stopwatches for documentation.

#include <boost/thread.hpp>
#include <boost/stopwatches/stopclock.hpp>
#include <boost/stopwatches/suspendible_clock.hpp>
#include <cmath>

using namespace boost::chrono;
using namespace boost::stopwatches;
long double res;

int f1(long j)
{
    stopclock<suspendible_clock<high_resolution_clock> > a(9);   
    //stopclock<high_resolution_clock> b(9);   

    for ( int i = 0; i < j; ++i )
        res+=std::sqrt( res+123.456L+i );  // burn some time
    if (j!=0) f1(j-1);
    //boost::this_thread::sleep(boost::posix_time::milliseconds(10));
    return 0;
}
int main()
{
    //stopclock<> _(BOOST_CHRONO_TIME_FUNCTION_FORMAT);   
    //stopclock<> _(9);   
    stopclock<suspendible_clock<high_resolution_clock> > a(9);   
    //stopclock<high_resolution_clock> b(9);   

    for (int i =1; i<= 3; ++i)
        f1(i*100);
    std::cout<< res << std::endl;
    return 0;
}
