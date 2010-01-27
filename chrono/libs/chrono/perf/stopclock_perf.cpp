//  stopclock_perf.cpp  ---------------------------------------------------//

//  Copyright 2009 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

#include <boost/thread.hpp>
#include <boost/chrono/stopclock.hpp>
#include <boost/chrono/thread_clock.hpp>

#include <boost/chrono/suspendible_clock.hpp>
#include <boost/chrono/stopclock_accumulator.hpp>
#include <cmath>

using namespace boost::chrono;
double res;

template <typename Clock>
void f1(long j)
{
    for ( long i = 0; i < j; ++i )
        res+=std::sqrt( res+123.456L+i );  // burn some time
    if (j!=0) {
        f1<Clock>(j-1);
    }
    //boost::this_thread::sleep(boost::posix_time::milliseconds(10));
}

template <typename Clock>
void f2(long j)
{
    stopclock<Clock> b(9);   

    for ( long i = 0; i < j; ++i )
        res+=std::sqrt( res+123.456L+i );  // burn some time
    if (j!=0) {
        //std::cout << "f1("<<j-1<<")\n";
        f2<Clock>(j-1);
    }
    //boost::this_thread::sleep(boost::posix_time::milliseconds(10));
}

template <typename Clock>
void f3(long j)
{
    stopclock<suspendible_clock<Clock> > a(9);   

    for ( long i = 0; i < j; ++i )
        res+=std::sqrt( res+123.456L+i );  // burn some time
    if (j!=0) {
        //std::cout << "f1("<<j-1<<")\n";
        f3<Clock>(j-1);
    }
    //boost::this_thread::sleep(boost::posix_time::milliseconds(10));
}

template <typename Clock>
void f4(long j)
{
  static stopclock_accumulator<Clock> acc(9);
  typename stopclock_accumulator<Clock>::scoped_run c(acc);

    for ( long i = 0; i < j; ++i )
        res+=std::sqrt( res+123.456L+i );  // burn some time
    if (j!=0) {
        //std::cout << "f1("<<j-1<<")\n";
        f4<Clock>(j-1);
    }
    //boost::this_thread::sleep(boost::posix_time::milliseconds(10));
}

template <typename Clock, long SIZE>
void test_clock_size()
{
    std::cout << ">>> test_clock_size() " << SIZE << std::endl;
    //stopclock<> _(BOOST_CHRONO_TIME_FUNCTION_FORMAT);   
    //stopclock<> _(9);   
    //stopclock<suspendible_clock<high_resolution_clock> > a(9);   
    {
        res=0;
        stopclock<Clock> b(9, std::string(">>> NONE ")+  stopwatch_formatter::default_format());

        for (long i =1; i<= 3; ++i)
            f1<Clock>(i*SIZE);
        std::cout<< res << std::endl;
    }
    {
        res=0;
        stopclock<Clock> b(9, std::string(">>> HIGH ") + stopwatch_formatter::default_format());

        for (long i =1; i<= 3; ++i)
            f2<Clock>(i*SIZE);
        std::cout<< res << std::endl;
    }
    {
        res=0;
        stopclock<suspendible_clock<high_resolution_clock> > a(9, std::string(">>> SUSPEND ") + stopwatch_formatter::default_format());   

        for (long i =1; i<= 3; ++i)
            f3<Clock>(i*SIZE);
        std::cout<< res << std::endl;
    }
    {
        res=0;
        stopclock<Clock> b(9, std::string(">>> ACCU ") + stopwatch_formatter::default_format());

        for (long i =1; i<= 3; ++i)
            f4<Clock>(i*SIZE);
        std::cout<< res << std::endl;
    }
    {
        res=0;
        stopclock<Clock> b(9, std::string(">>> NONE ")+  stopwatch_formatter::default_format());

        for (long i =1; i<= 3; ++i)
            f1<Clock>(i*SIZE);
        std::cout<< res << std::endl;
    }
    {
        res=0;
        stopclock<Clock> b(9, std::string(">>> ACCU ") + stopwatch_formatter::default_format());

        for (long i =1; i<= 3; ++i)
            f4<Clock>(i*SIZE);
        std::cout<< res << std::endl;
    }
}

template <typename Clock>
void test_clock()
{
    std::cout << ">>> test_clock() " << std::endl;
    test_clock_size<Clock, 100>();
    test_clock_size<Clock, 200>();
    test_clock_size<Clock, 400>();
    test_clock_size<Clock, 800>();
    test_clock_size<Clock, 1600>();
    test_clock_size<Clock, 3200>();
}


int main() {
#ifdef BOOST_CHRONO_HAS_THREAD_CLOCK    
    std::cout << ">>> main()-thread_clock" << std::endl;
    test_clock<thread_clock>();
#endif    
    std::cout << ">>> main()-high_resolution_clock" << std::endl;
    test_clock<high_resolution_clock>();
#ifdef BOOST_CHRONO_HAS_THREAD_CLOCK    
    std::cout << ">>> main()-thread_clock" << std::endl;
    test_clock<thread_clock>();
#endif    

    return 0;

}
