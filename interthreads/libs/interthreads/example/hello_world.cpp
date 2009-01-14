//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Roland Schwarz 2006. 
// (C) Copyright Vicente J. Botet Escriba 2008-20009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
// Based on the shared.cpp example from the threadalert library of Roland Schwarz 
//////////////////////////////////////////////////////////////////////////////
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
boost::mutex out_global_mutex;

    #include <iostream>
    #include <boost/interthreads/thread_decorator.hpp>
    #include <boost/thread.hpp>
    #include <boost/function.hpp>
        
    namespace bith = boost::interthreads;

    void my_setup() {
        std::cout << "Hello World!" << std::endl;
    }

    void my_cleanup() {
        std::cout << "Bye, Bye!" << std::endl;
    }

    bith::thread_decoration my_decoration(my_setup, my_cleanup);

    struct mycallable1
    {
        void operator()() const {
            std::cout << "mycallable1" << std::endl;
        };
    };

    struct mycallable2
    {
        void operator()() const {
            std::cout << "mycallable2" << std::endl;
        };
    };
    void my_thread() {
        std::cout << "..." << std::endl;
//        mycallable1 x;
//        boost::this_thread::at_thread_exit(x);
    }

    template <typename F>
    void my_test(F f) {
        F cpy = f;
        cpy();
    }
    
    struct X {
        template <typename F>
        X(F f) {
            F cpy = f;
            cpy();
        }
    };
    
    int main() {
#if 0        
        mycallable2 x;
        boost::this_thread::at_thread_exit(x);
//        boost::thread th1(x);

        bith::thread_decorator my_thread_d1(my_thread);
        my_test(my_thread_d1);
        X x1(my_thread_d1);
        
        bith::thread_decorator my_thread_d2(my_thread_d1);
        my_test(my_thread_d1);
        X x2(my_thread_d2);

        bith::thread_decorator my_thread_d3;
        my_thread_d3 = my_thread_d2;
        my_test(my_thread_d2);
        X x3(my_thread_d3);
#endif
#if 1        
        bith::thread_decorator my_thread_d(my_thread);
//        boost::thread th(my_thread_d);

//    boost::detail::thread_move_t<bith::thread_decorator> mt(my_thread_d);
//        boost::thread th(mt);
        boost::thread th(bith::make_decorator(my_thread));
        boost::thread th2(bith::make_decorator(my_thread));

//        boost::function<void ()> fct(my_thread_d);
//        fct();
//        boost::thread th(boost::ref(my_thread_d));
//        boost::thread th(boost::detail::thread_move_t<bith::thread_decorator>(my_thread_d));
//        boost::thread th(boost::move(my_thread_d));
//        boost::thread th(fct);
        
	{
		boost::mutex::scoped_lock out_guard(out_global_mutex);
	    std::cout << "waiting1 " <<std::endl;
    }
        th.join();
	{
		boost::mutex::scoped_lock out_guard(out_global_mutex);
	    std::cout << "waiting2" <<std::endl;
    }
        th2.join();
	{
		boost::mutex::scoped_lock out_guard(out_global_mutex);
	    std::cout << "end" <<std::endl;
    }
#endif

#if 0 
        mycallable2 c2;
        boost::function<void ()> fct(c2);
        boost::thread th2(fct);
        th2.join();
#endif

//        boost::thread th(my_thread_d);
//        boost::thread th(my_thread);

        return 0;
    }
