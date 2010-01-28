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


namespace rep_none {
    template <typename Clock>
    void f1(long v) {
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
    }

    template <typename Clock>
    void f2(long v) {
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        f1<Clock>(v-1);
    }
    
    template <typename Clock>
    void f3(long v) {
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f4(long v) {
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f5(long v) {
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%4==0) f4<Clock>(v-1);
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f6(long v) {
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%5==0) f5<Clock>(v-1);
        if (v%4==0) f4<Clock>(v-1);
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f7(long v) {
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%6==0) f6<Clock>(v-1);
        if (v%5==0) f5<Clock>(v-1);
        if (v%4==0) f4<Clock>(v-1);
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f8(long v) {
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%7==0) f7<Clock>(v-1);
        if (v%6==0) f6<Clock>(v-1);
        if (v%5==0) f5<Clock>(v-1);
        if (v%4==0) f4<Clock>(v-1);
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f9(long v) {
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%8==0) f8<Clock>(v-1);
        if (v%7==0) f7<Clock>(v-1);
        if (v%6==0) f6<Clock>(v-1);
        if (v%5==0) f5<Clock>(v-1);
        if (v%4==0) f4<Clock>(v-1);
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f10(long v) {
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%9==0) f9<Clock>(v-1);
        if (v%8==0) f8<Clock>(v-1);
        if (v%7==0) f7<Clock>(v-1);
        if (v%6==0) f6<Clock>(v-1);
        if (v%5==0) f5<Clock>(v-1);
        if (v%4==0) f4<Clock>(v-1);
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f(long j)
    {
        for ( long i = 0; i < j; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (j!=0) {
            f<Clock>(j-1);
        }
        //boost::this_thread::sleep(boost::posix_time::milliseconds(10));
    }
}


namespace rep_basic {
    template <typename Clock>
    void f1(long v) {
        stopclock<Clock> b(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
    }

    template <typename Clock>
    void f2(long v) {
        stopclock<Clock> b(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        f1<Clock>(v-1);
    }
    
    template <typename Clock>
    void f3(long v) {
        stopclock<Clock> b(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f4(long v) {
        stopclock<Clock> b(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f5(long v) {
        stopclock<Clock> b(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%4==0) f4<Clock>(v-1);
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f6(long v) {
        stopclock<Clock> b(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%5==0) f5<Clock>(v-1);
        if (v%4==0) f4<Clock>(v-1);
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f7(long v) {
        stopclock<Clock> b(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%6==0) f6<Clock>(v-1);
        if (v%5==0) f5<Clock>(v-1);
        if (v%4==0) f4<Clock>(v-1);
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f8(long v) {
        stopclock<Clock> b(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%7==0) f7<Clock>(v-1);
        if (v%6==0) f6<Clock>(v-1);
        if (v%5==0) f5<Clock>(v-1);
        if (v%4==0) f4<Clock>(v-1);
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f9(long v) {
        stopclock<Clock> b(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%8==0) f8<Clock>(v-1);
        if (v%7==0) f7<Clock>(v-1);
        if (v%6==0) f6<Clock>(v-1);
        if (v%5==0) f5<Clock>(v-1);
        if (v%4==0) f4<Clock>(v-1);
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f10(long v) {
        stopclock<Clock> b(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%9==0) f9<Clock>(v-1);
        if (v%8==0) f8<Clock>(v-1);
        if (v%7==0) f7<Clock>(v-1);
        if (v%6==0) f6<Clock>(v-1);
        if (v%5==0) f5<Clock>(v-1);
        if (v%4==0) f4<Clock>(v-1);
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f(long j)
    {
        stopclock<Clock> b(9);   

        for ( long i = 0; i < j; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (j!=0) {
            //std::cout << "f1("<<j-1<<")\n";
            f<Clock>(j-1);
        }
        //boost::this_thread::sleep(boost::posix_time::milliseconds(10));
    }
}

namespace rep_suspendible {
    template <typename Clock>
    void f1(long v) {
        stopclock<suspendible_clock<Clock> > a(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
    }

    template <typename Clock>
    void f2(long v) {
        stopclock<suspendible_clock<Clock> > a(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        f1<Clock>(v-1);
    }
    
    template <typename Clock>
    void f3(long v) {
        stopclock<suspendible_clock<Clock> > a(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f4(long v) {
        stopclock<suspendible_clock<Clock> > a(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f5(long v) {
        stopclock<suspendible_clock<Clock> > a(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%4==0) f4<Clock>(v-1);
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f6(long v) {
        stopclock<suspendible_clock<Clock> > a(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%5==0) f5<Clock>(v-1);
        if (v%4==0) f4<Clock>(v-1);
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f7(long v) {
        stopclock<suspendible_clock<Clock> > a(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%6==0) f6<Clock>(v-1);
        if (v%5==0) f5<Clock>(v-1);
        if (v%4==0) f4<Clock>(v-1);
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f8(long v) {
        stopclock<suspendible_clock<Clock> > a(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%7==0) f7<Clock>(v-1);
        if (v%6==0) f6<Clock>(v-1);
        if (v%5==0) f5<Clock>(v-1);
        if (v%4==0) f4<Clock>(v-1);
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f9(long v) {
        stopclock<suspendible_clock<Clock> > a(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%8==0) f8<Clock>(v-1);
        if (v%7==0) f7<Clock>(v-1);
        if (v%6==0) f6<Clock>(v-1);
        if (v%5==0) f5<Clock>(v-1);
        if (v%4==0) f4<Clock>(v-1);
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f10(long v) {
        stopclock<suspendible_clock<Clock> > a(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%9==0) f9<Clock>(v-1);
        if (v%8==0) f8<Clock>(v-1);
        if (v%7==0) f7<Clock>(v-1);
        if (v%6==0) f6<Clock>(v-1);
        if (v%5==0) f5<Clock>(v-1);
        if (v%4==0) f4<Clock>(v-1);
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f(long j)
    {
        stopclock<suspendible_clock<Clock> > a(9);   

        for ( long i = 0; i < j; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (j!=0) {
            //std::cout << "f1("<<j-1<<")\n";
            f<Clock>(j-1);
        }
        //boost::this_thread::sleep(boost::posix_time::milliseconds(10));
    }
}

namespace rep_accu {
    template <typename Clock>
    void f1(long v) {
        static stopclock_accumulator<Clock> acc(9);
        typename stopclock_accumulator<Clock>::scoped_run c(acc);
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
    }

    template <typename Clock>
    void f2(long v) {
        static stopclock_accumulator<Clock> acc(9);
        typename stopclock_accumulator<Clock>::scoped_run c(acc);
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        f1<Clock>(v-1);
    }
    
    template <typename Clock>
    void f3(long v) {
        static stopclock_accumulator<Clock> acc(9);
        typename stopclock_accumulator<Clock>::scoped_run c(acc);
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f4(long v) {
        static stopclock_accumulator<Clock> acc(9);
        typename stopclock_accumulator<Clock>::scoped_run c(acc);
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f5(long v) {
        static stopclock_accumulator<Clock> acc(9);
        typename stopclock_accumulator<Clock>::scoped_run c(acc);
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%4==0) f4<Clock>(v-1);
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f6(long v) {
        static stopclock_accumulator<Clock> acc(9);
        typename stopclock_accumulator<Clock>::scoped_run c(acc);
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%5==0) f5<Clock>(v-1);
        if (v%4==0) f4<Clock>(v-1);
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f7(long v) {
        static stopclock_accumulator<Clock> acc(9);
        typename stopclock_accumulator<Clock>::scoped_run c(acc);
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%6==0) f6<Clock>(v-1);
        if (v%5==0) f5<Clock>(v-1);
        if (v%4==0) f4<Clock>(v-1);
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f8(long v) {
        static stopclock_accumulator<Clock> acc(9);
        typename stopclock_accumulator<Clock>::scoped_run c(acc);
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%7==0) f7<Clock>(v-1);
        if (v%6==0) f6<Clock>(v-1);
        if (v%5==0) f5<Clock>(v-1);
        if (v%4==0) f4<Clock>(v-1);
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f9(long v) {
        static stopclock_accumulator<Clock> acc(9);
        typename stopclock_accumulator<Clock>::scoped_run c(acc);
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%8==0) f8<Clock>(v-1);
        if (v%7==0) f7<Clock>(v-1);
        if (v%6==0) f6<Clock>(v-1);
        if (v%5==0) f5<Clock>(v-1);
        if (v%4==0) f4<Clock>(v-1);
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f10(long v) {
        static stopclock_accumulator<Clock> acc(9);
        typename stopclock_accumulator<Clock>::scoped_run c(acc);
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v==0) return;
        
        if (v%9==0) f9<Clock>(v-1);
        if (v%8==0) f8<Clock>(v-1);
        if (v%7==0) f7<Clock>(v-1);
        if (v%6==0) f6<Clock>(v-1);
        if (v%5==0) f5<Clock>(v-1);
        if (v%4==0) f4<Clock>(v-1);
        if (v%3==0) f3<Clock>(v-1);
        if (v%2==0) f2<Clock>(v-1);
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f(long j)
    {
        static stopclock_accumulator<Clock> acc(9);
        typename stopclock_accumulator<Clock>::scoped_run c(acc);

        for ( long i = 0; i < j; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (j!=0) {
            //std::cout << "f1("<<j-1<<")\n";
            f<Clock>(j-1);
        }
        //boost::this_thread::sleep(boost::posix_time::milliseconds(10));
    }
}

template <typename Clock, long SIZE>
void test_clock_size()
{
    std::cout << ">>> test_clock_size " << SIZE << std::endl;
    {
        res=0;
        stopclock<Clock> b(9, std::string(">>> REC NONE ")+  stopwatch_formatter::default_format());

        for (long i =1; i<= 3; ++i)
            rep_none::f<Clock>(i*SIZE);
        std::cout<< res << std::endl;
    }
    {
        res=0;
        stopclock<Clock> b(9, std::string(">>> REC BASIC ") + stopwatch_formatter::default_format());

        for (long i =1; i<= 3; ++i)
            rep_basic::f<Clock>(i*SIZE);
        std::cout<< res << std::endl;
    }
    {
        res=0;
        stopclock<suspendible_clock<Clock> > a(9, std::string(">>> REC SUSPEND ") + stopwatch_formatter::default_format());   

        for (long i =1; i<= 3; ++i)
            rep_suspendible::f<Clock>(i*SIZE);
        std::cout<< res << std::endl;
    }
    {
        res=0;
        stopclock<Clock> b(9, std::string(">>> REC ACCU ") + stopwatch_formatter::default_format());

        for (long i =1; i<= 3; ++i)
            rep_accu::f<Clock>(i*SIZE);
        std::cout<< res << std::endl;
    }
    {
        res=0;
        stopclock<Clock> b(9, std::string(">>> NON_REC NONE ")+  stopwatch_formatter::default_format());

        for (long i =1; i<= 3; ++i)
            rep_none::f10<Clock>(i*SIZE);
        std::cout<< res << std::endl;
    }
    {
        res=0;
        stopclock<Clock> b(9, std::string(">>> NON_REC BASIC ") + stopwatch_formatter::default_format());

        for (long i =1; i<= 3; ++i)
            rep_basic::f10<Clock>(i*SIZE);
        std::cout<< res << std::endl;
    }
    {
        res=0;
        stopclock<suspendible_clock<Clock> > a(9, std::string(">>> NON_REC SUSPEND ") + stopwatch_formatter::default_format());   

        for (long i =1; i<= 3; ++i)
            rep_suspendible::f10<Clock>(i*SIZE);
        std::cout<< res << std::endl;
    }
    {
        res=0;
        stopclock<Clock> b(9, std::string(">>> NON_REC ACCU ") + stopwatch_formatter::default_format());

        for (long i =1; i<= 3; ++i)
            rep_accu::f10<Clock>(i*SIZE);
        std::cout<< res << std::endl;
    }
}

template <typename Clock>
void test_clock()
{
    test_clock_size<Clock, 100>();
    test_clock_size<Clock, 200>();
    test_clock_size<Clock, 400>();
    test_clock_size<Clock, 800>();
    test_clock_size<Clock, 1600>();
    test_clock_size<Clock, 3200>();
}


int main() {
#ifdef BOOST_CHRONO_HAS_THREAD_CLOCK    
    std::cout << ">>> main thread_clock" << std::endl;
    test_clock<thread_clock>();
#endif    
    std::cout << ">>> main high_resolution_clock" << std::endl;
    test_clock<high_resolution_clock>();
#ifdef BOOST_CHRONO_HAS_THREAD_CLOCK    
    std::cout << ">>> main thread_clock" << std::endl;
    test_clock<thread_clock>();
#endif    

    return 0;

}
