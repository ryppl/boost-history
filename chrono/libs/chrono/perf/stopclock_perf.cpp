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
    std::size_t count[11];

    template <typename Clock>
    void f10(long v);

    template <typename Clock>
    void f1(long v) {
        std::cout<< ">>> f1 " <<  v << "\n";
        ++count[1];
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=10) return;
        f10<Clock>(v-1);
    }

    template <typename Clock>
    void f2(long v) {
        std::cout<< ">>> f2 " <<  v << "\n";
        ++count[2];
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=2) return;
        
        f1<Clock>(v-1);
    }
    
    template <typename Clock>
    void f3(long v) {
        std::cout<< ">>> f3 " <<  v << "\n";
        ++count[3];
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=3) return;
        
        if (v%2==0) {f2<Clock>(v-1); return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f4(long v) {
        std::cout<< ">>> f4 " <<  v << "\n";
        ++count[4];
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=4) return;
        
        if (v%3==0) {f3<Clock>(v-1); return;}
        if (v%2==0) {f2<Clock>(v-1); return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f5(long v) {
        std::cout<< ">>> f5 " <<  v << "\n";
        ++count[5];
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=5) return;
        
        if (v%4==0) {f4<Clock>(v-1); return;}
        if (v%3==0) {f3<Clock>(v-1); return;}
        if (v%2==0) {f2<Clock>(v-1); return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f6(long v) {
        std::cout<< ">>> f6 " <<  v << "\n";
        ++count[6];
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=6) return;
        
        if (v%5==0) {f5<Clock>(v-1); return;}
        if (v%4==0) {f4<Clock>(v-1); return;}
        if (v%3==0) {f3<Clock>(v-1); return;}
        if (v%2==0) {f2<Clock>(v-1); return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f7(long v) {
        std::cout<< ">>> f7 " <<  v << "\n";
        ++count[7];
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=7) return;
        
        if (v%6==0) {f6<Clock>(v-1); return;}
        if (v%5==0) {f5<Clock>(v-1); return;}
        if (v%4==0) {f4<Clock>(v-1); return;}
        if (v%3==0) {f3<Clock>(v-1); return;}
        if (v%2==0) {f2<Clock>(v-1); return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f8(long v) {
        std::cout<< ">>> f8 " <<  v << "\n";
        ++count[8];
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=8) return;
        
        if (v%7==0) {f7<Clock>(v-1); return;}
        if (v%6==0) {f6<Clock>(v-1); return;}
        if (v%5==0) {f5<Clock>(v-1); return;}
        if (v%4==0) {f4<Clock>(v-1); return;}
        if (v%3==0) {f3<Clock>(v-1); return;}
        if (v%2==0) {f2<Clock>(v-1); return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f9(long v) {
        std::cout<< ">>> f9 " <<  v << "\n";
        ++count[9];
        //std::cout<< ">>> COUNT NONE " << " " << rep_none::count[9] << std::endl;
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=9) return;
        
        if (v%8==0) {f8<Clock>(v-1); return;}
        if (v%7==0) {f7<Clock>(v-1); return;}
        if (v%6==0) {f6<Clock>(v-1); return;}
        if (v%5==0) {f5<Clock>(v-1); return;}
        if (v%4==0) {f4<Clock>(v-1); return;}
        if (v%3==0) {f3<Clock>(v-1); return;}
        if (v%2==0) {f2<Clock>(v-1); return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f10(long v) {
        std::cout<< ">>> f10 " <<  v << "\n";
        ++count[10];
        //std::cout<< ">>> COUNT NONE 10 " << " " << rep_none::count[10] << std::endl;
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=10) return;
        
        if ((v%9)==0) {f9<Clock>(v-1); return;}
        if ((v%8)==0) {f8<Clock>(v-1); return;}
        if ((v%7)==0) {f7<Clock>(v-1); return;}
        if ((v%6)==0) {f6<Clock>(v-1); return;}
        if ((v%5)==0) {f5<Clock>(v-1); return;}
        if ((v%4)==0) {f4<Clock>(v-1); return;}
        if ((v%3)==0) {f3<Clock>(v-1); return;}
        if ((v%2)==0) {f2<Clock>(v-1); return;}
        f1<Clock>(v-1);
    }

    template <typename Clock>
    void f(long j)
    {
        ++count[0];
        for ( long i = 0; i < j; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (j>1) {
            f<Clock>(j-1);
        }
        //boost::this_thread::sleep(boost::posix_time::milliseconds(10));
    }
}


namespace rep_basic {
    std::size_t count[11];

    template <typename Clock>
    void f10(long v);

    template <typename Clock>
    void f1(long v) {
        stopclock<Clock> b(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=10) return;
        f10<Clock>(v-1);
    }

    template <typename Clock>
    void f2(long v) {
        stopclock<Clock> b(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=2) return;
        
        f1<Clock>(v-1);
    }
    
    template <typename Clock>
    void f3(long v) {
        stopclock<Clock> b(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=3) return;
        
        if (v%2==0) {f2<Clock>(v-1); return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f4(long v) {
        stopclock<Clock> b(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=4) return;
        
        if (v%3==0) {f3<Clock>(v-1); return;}
        if (v%2==0) {f2<Clock>(v-1); return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f5(long v) {
        stopclock<Clock> b(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=5) return;
        
        if (v%4==0) {f4<Clock>(v-1);return;}
        if (v%3==0) {f3<Clock>(v-1);return;}
        if (v%2==0) {f2<Clock>(v-1);return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f6(long v) {
        stopclock<Clock> b(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=6) return;
        
        if (v%5==0) {f5<Clock>(v-1);return;}
        if (v%4==0) {f4<Clock>(v-1);return;}
        if (v%3==0) {f3<Clock>(v-1);return;}
        if (v%2==0) {f2<Clock>(v-1);return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f7(long v) {
        stopclock<Clock> b(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=7) return;
        
        if (v%6==0) {f6<Clock>(v-1);return;}
        if (v%5==0) {f5<Clock>(v-1);return;}
        if (v%4==0) {f4<Clock>(v-1);return;}
        if (v%3==0) {f3<Clock>(v-1);return;}
        if (v%2==0) {f2<Clock>(v-1);return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f8(long v) {
        stopclock<Clock> b(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=8) return;
        
        if (v%7==0) {f7<Clock>(v-1);return;}
        if (v%6==0) {f6<Clock>(v-1);return;}
        if (v%5==0) {f5<Clock>(v-1);return;}
        if (v%4==0) {f4<Clock>(v-1);return;}
        if (v%3==0) {f3<Clock>(v-1);return;}
        if (v%2==0) {f2<Clock>(v-1);return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f9(long v) {
        stopclock<Clock> b(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=9) return;
        
        if (v%8==0) {f8<Clock>(v-1);return;}
        if (v%7==0) {f7<Clock>(v-1);return;}
        if (v%6==0) {f6<Clock>(v-1);return;}
        if (v%5==0) {f5<Clock>(v-1);return;}
        if (v%4==0) {f4<Clock>(v-1);return;}
        if (v%3==0) {f3<Clock>(v-1);return;}
        if (v%2==0) {f2<Clock>(v-1);return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f10(long v) {
        stopclock<Clock> b(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=10) return;
        
        if (v%9==0) {f9<Clock>(v-1);return;}
        if (v%8==0) {f8<Clock>(v-1);return;}
        if (v%7==0) {f7<Clock>(v-1);return;}
        if (v%6==0) {f6<Clock>(v-1);return;}
        if (v%5==0) {f5<Clock>(v-1);return;}
        if (v%4==0) {f4<Clock>(v-1);return;}
        if (v%3==0) {f3<Clock>(v-1);return;}
        if (v%2==0) {f2<Clock>(v-1);return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f(long j)
    {
        stopclock<Clock> b(9);   

        for ( long i = 0; i < j; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (j>1) {
            //std::cout << "f1("<<j-1<<")\n";
            f<Clock>(j-1);
        }
        //boost::this_thread::sleep(boost::posix_time::milliseconds(10));
    }
}

namespace rep_suspendible {
    std::size_t count[11];

    template <typename Clock>
    void f10(long v);

    template <typename Clock>
    void f1(long v) {
        stopclock<suspendible_clock<Clock> > a(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=1) return;
        f10<Clock>(v-1);
    }

    template <typename Clock>
    void f2(long v) {
        stopclock<suspendible_clock<Clock> > a(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=2) return;
        
        f1<Clock>(v-1);
    }
    
    template <typename Clock>
    void f3(long v) {
        stopclock<suspendible_clock<Clock> > a(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=3) return;
        
        if (v%2==0) {f2<Clock>(v-1);return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f4(long v) {
        stopclock<suspendible_clock<Clock> > a(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=4) return;
        
        if (v%3==0) {f3<Clock>(v-1);return;}
        if (v%2==0) {f2<Clock>(v-1);return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f5(long v) {
        stopclock<suspendible_clock<Clock> > a(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=5) return;
        
        if (v%4==0) {f4<Clock>(v-1);return;}
        if (v%3==0) {f3<Clock>(v-1);return;}
        if (v%2==0) {f2<Clock>(v-1);return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f6(long v) {
        stopclock<suspendible_clock<Clock> > a(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=6) return;
        
        if (v%5==0) {f5<Clock>(v-1);return;}
        if (v%4==0) {f4<Clock>(v-1);return;}
        if (v%3==0) {f3<Clock>(v-1);return;}
        if (v%2==0) {f2<Clock>(v-1);return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f7(long v) {
        stopclock<suspendible_clock<Clock> > a(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=7) return;
        
        if (v%6==0) {f6<Clock>(v-1);return;}
        if (v%5==0) {f5<Clock>(v-1);return;}
        if (v%4==0) {f4<Clock>(v-1);return;}
        if (v%3==0) {f3<Clock>(v-1);return;}
        if (v%2==0) {f2<Clock>(v-1);return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f8(long v) {
        stopclock<suspendible_clock<Clock> > a(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=8) return;
        
        if (v%7==0) {f7<Clock>(v-1);return;}
        if (v%6==0) {f6<Clock>(v-1);return;}
        if (v%5==0) {f5<Clock>(v-1);return;}
        if (v%4==0) {f4<Clock>(v-1);return;}
        if (v%3==0) {f3<Clock>(v-1);return;}
        if (v%2==0) {f2<Clock>(v-1);return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f9(long v) {
        stopclock<suspendible_clock<Clock> > a(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=9) return;
        
        if (v%8==0) {f8<Clock>(v-1);return;}
        if (v%7==0) {f7<Clock>(v-1);return;}
        if (v%6==0) {f6<Clock>(v-1);return;}
        if (v%5==0) {f5<Clock>(v-1);return;}
        if (v%4==0) {f4<Clock>(v-1);return;}
        if (v%3==0) {f3<Clock>(v-1);return;}
        if (v%2==0) {f2<Clock>(v-1);return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f10(long v) {
        stopclock<suspendible_clock<Clock> > a(9);   
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=10) return;
        
        if (v%9==0) {f9<Clock>(v-1);return;}
        if (v%8==0) {f8<Clock>(v-1);return;}
        if (v%7==0) {f7<Clock>(v-1);return;}
        if (v%6==0) {f6<Clock>(v-1);return;}
        if (v%5==0) {f5<Clock>(v-1);return;}
        if (v%4==0) {f4<Clock>(v-1);return;}
        if (v%3==0) {f3<Clock>(v-1);return;}
        if (v%2==0) {f2<Clock>(v-1);return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f(long j)
    {
        stopclock<suspendible_clock<Clock> > a(9);   

        for ( long i = 0; i < j; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (j>1) {
            //std::cout << "f1("<<j-1<<")\n";
            f<Clock>(j-1);
        }
        //boost::this_thread::sleep(boost::posix_time::milliseconds(10));
    }
}

namespace rep_accu {
    std::size_t count[11];

    template <typename Clock>
    void f10(long v);

    template <typename Clock>
    void f1(long v) {
        static stopclock_accumulator<Clock> acc(9);
        typename stopclock_accumulator<Clock>::scoped_run c(acc);
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=10) return;
        f10<Clock>(v-1);
    }

    template <typename Clock>
    void f2(long v) {
        static stopclock_accumulator<Clock> acc(9);
        typename stopclock_accumulator<Clock>::scoped_run c(acc);
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=2) return;
        
        f1<Clock>(v-1);
    }
    
    template <typename Clock>
    void f3(long v) {
        static stopclock_accumulator<Clock> acc(9);
        typename stopclock_accumulator<Clock>::scoped_run c(acc);
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=3) return;
        
        if (v%2==0) {f2<Clock>(v-1);return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f4(long v) {
        static stopclock_accumulator<Clock> acc(9);
        typename stopclock_accumulator<Clock>::scoped_run c(acc);
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=4) return;
        
        if (v%3==0) {f3<Clock>(v-1);return;}
        if (v%2==0) {f2<Clock>(v-1);return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f5(long v) {
        static stopclock_accumulator<Clock> acc(9);
        typename stopclock_accumulator<Clock>::scoped_run c(acc);
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=5) return;
        
        if (v%4==0) {f4<Clock>(v-1);return;}
        if (v%3==0) {f3<Clock>(v-1);return;}
        if (v%2==0) {f2<Clock>(v-1);return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f6(long v) {
        static stopclock_accumulator<Clock> acc(9);
        typename stopclock_accumulator<Clock>::scoped_run c(acc);
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=6) return;
        
        if (v%5==0) {f5<Clock>(v-1);return;}
        if (v%4==0) {f4<Clock>(v-1);return;}
        if (v%3==0) {f3<Clock>(v-1);return;}
        if (v%2==0) {f2<Clock>(v-1);return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f7(long v) {
        static stopclock_accumulator<Clock> acc(9);
        typename stopclock_accumulator<Clock>::scoped_run c(acc);
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=7) return;
        
        if (v%6==0) {f6<Clock>(v-1);return;}
        if (v%5==0) {f5<Clock>(v-1);return;}
        if (v%4==0) {f4<Clock>(v-1);return;}
        if (v%3==0) {f3<Clock>(v-1);return;}
        if (v%2==0) {f2<Clock>(v-1);return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f8(long v) {
        static stopclock_accumulator<Clock> acc(9);
        typename stopclock_accumulator<Clock>::scoped_run c(acc);
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=8) return;
        
        if (v%7==0) {f7<Clock>(v-1);return;}
        if (v%6==0) {f6<Clock>(v-1);return;}
        if (v%5==0) {f5<Clock>(v-1);return;}
        if (v%4==0) {f4<Clock>(v-1);return;}
        if (v%3==0) {f3<Clock>(v-1);return;}
        if (v%2==0) {f2<Clock>(v-1);return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f9(long v) {
        static stopclock_accumulator<Clock> acc(9);
        typename stopclock_accumulator<Clock>::scoped_run c(acc);
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=9) return;
        
        if (v%8==0) {f8<Clock>(v-1);return;}
        if (v%7==0) {f7<Clock>(v-1);return;}
        if (v%6==0) {f6<Clock>(v-1);return;}
        if (v%5==0) {f5<Clock>(v-1);return;}
        if (v%4==0) {f4<Clock>(v-1);return;}
        if (v%3==0) {f3<Clock>(v-1);return;}
        if (v%2==0) {f2<Clock>(v-1);return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f10(long v) {
        static stopclock_accumulator<Clock> acc(9);
        typename stopclock_accumulator<Clock>::scoped_run c(acc);
        // burn some time
        for ( long i = 0; i < v; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (v<=10) return;
        
        if (v%9==0) {f9<Clock>(v-1);return;}
        if (v%8==0) {f8<Clock>(v-1);return;}
        if (v%7==0) {f7<Clock>(v-1);return;}
        if (v%6==0) {f6<Clock>(v-1);return;}
        if (v%5==0) {f5<Clock>(v-1);return;}
        if (v%4==0) {f4<Clock>(v-1);return;}
        if (v%3==0) {f3<Clock>(v-1);return;}
        if (v%2==0) {f2<Clock>(v-1);return;}
        f1<Clock>(v-1);
    }
    template <typename Clock>
    void f(long j)
    {
        static stopclock_accumulator<Clock> acc(9);
        typename stopclock_accumulator<Clock>::scoped_run c(acc);

        for ( long i = 0; i < j; ++i )
            res+=std::sqrt( res+123.456L+i );  // burn some time
        if (j>1) {
            //std::cout << "f1("<<j-1<<")\n";
            f<Clock>(j-1);
        }
        //boost::this_thread::sleep(boost::posix_time::milliseconds(10));
    }
}

template <typename Clock, long SIZE>
void test_clock_size()
{
    long K =1;
    std::cout << ">>> test_clock_size " << SIZE << std::endl;
    {
        rep_none::count[0]=0;
        res=0;
        stopclock<Clock> b(9, std::string(">>> REC NONE ")+  stopwatch_formatter::default_format());

        for (long i =1; i<= K; ++i)
            rep_none::f<Clock>(i*SIZE);
        std::cout<< res << std::endl;        
        std::cout<< ">>> COUNT REC NONE " << SIZE << " " << rep_none::count[0] << std::endl;

    }
    {
        res=0;
        stopclock<Clock> b(9, std::string(">>> REC BASIC ") + stopwatch_formatter::default_format());

        for (long i =1; i<= K; ++i)
            rep_basic::f<Clock>(i*SIZE);
        std::cout<< res << std::endl;
    }
    {
        res=0;
        stopclock<suspendible_clock<Clock> > a(9, std::string(">>> REC SUSPEND ") + stopwatch_formatter::default_format());   

        for (long i =1; i<= K; ++i)
            rep_suspendible::f<Clock>(i*SIZE);
        std::cout<< res << std::endl;
    }
    {
        res=0;
        stopclock<Clock> b(9, std::string(">>> REC ACCU ") + stopwatch_formatter::default_format());

        for (long i =1; i<= K; ++i)
            rep_accu::f<Clock>(i*SIZE);
        std::cout<< res << std::endl;
    }
    {
        res=0;
        for (int i=1;i<=10;i++) rep_none::count[i]=0;
        stopclock<Clock> b(9, std::string(">>> NON_REC NONE ")+  stopwatch_formatter::default_format());

        for (long i =1; i<= K; ++i)
            rep_none::f10<Clock>(i*SIZE);
        std::cout<< res << std::endl;
        std::size_t c=0;
        for (int i=1;i<=10;i++) 
            c+= rep_none::count[i];
        std::cout<< ">>> COUNT NON_REC NONE " << SIZE << " " << c << std::endl;
    }
    {
        res=0;
        stopclock<Clock> b(9, std::string(">>> NON_REC BASIC ") + stopwatch_formatter::default_format());

        for (long i =1; i<= K; ++i)
            rep_basic::f10<Clock>(i*SIZE);
        std::cout<< res << std::endl;
    }
    {
        res=0;
        stopclock<suspendible_clock<Clock> > a(9, std::string(">>> NON_REC SUSPEND ") + stopwatch_formatter::default_format());   

        for (long i =1; i<= K; ++i)
            rep_suspendible::f10<Clock>(i*SIZE);
        std::cout<< res << std::endl;
    }
    {
        res=0;
        stopclock<Clock> b(9, std::string(">>> NON_REC ACCU ") + stopwatch_formatter::default_format());

        for (long i =1; i<= K; ++i)
            rep_accu::f10<Clock>(i*SIZE);
        std::cout<< res << std::endl;
    }
#if 0
#endif
}

template <typename Clock>
void test_clock()
{
    //test_clock_size<Clock, 3200>();
    //test_clock_size<Clock, 1600>();
    //test_clock_size<Clock, 800>();
    //test_clock_size<Clock, 400>();
    //test_clock_size<Clock, 200>();
    test_clock_size<Clock, 1050>();
    test_clock_size<Clock, 1040>();    
    test_clock_size<Clock, 1030>();    
    test_clock_size<Clock, 1020>();    
    test_clock_size<Clock, 1010>();    
    test_clock_size<Clock, 1000>();    
    test_clock_size<Clock, 100>();






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
