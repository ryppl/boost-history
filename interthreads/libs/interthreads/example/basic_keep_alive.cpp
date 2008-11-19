//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Roland Schwarz 2006. 
// (C) Copyright Vicente J. Botet Escriba 2008. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
// Based on the shared.cpp example from the threadalert library of Roland Schwarz 
//////////////////////////////////////////////////////////////////////////////

#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
boost::mutex out_global_mutex2;

void sleep(int sec)
{
	boost::xtime t;
	boost::xtime_get(&t,1);	
    t.sec += sec; 
    boost::thread::sleep(t);
}


    #include <boost/interthreads/thread_decorator.hpp>
    #include <boost/interthreads/thread_keep_alive.hpp>
    #include "./async_ostream.cpp"
    #include "./async_ostream.hpp"
    #include <boost/thread/thread.hpp>
    #include <iostream>
    #include <sstream>
        
    namespace bith = boost::interthreads;

    void my_thread() {
        bith::this_thread::enable_keep_alive enabler;
        for (int i=0; i<10; i++) {
            bith::this_thread::keep_alive_point();            

            boost::thread::id id= boost::this_thread::get_id();
            std::stringstream sss;
#define COUT_
#ifdef COUT_
            bith::cout_ << "TID=" << i << " " << id << std::endl;
            bith::cout_.flush();
#else
            {
                boost::lock_guard<boost::mutex> lock(out_global_mutex2);
                std::cout << "TID=" << i << " " << id << std::endl;
            }
#endif            
            boost::this_thread::sleep(boost::posix_time::milliseconds(10));
        }
    }
    
    int main() {
        boost::thread th1(bith::make_decorator(my_thread));
        boost::thread th2(bith::make_decorator(my_thread));
        boost::thread th3(bith::make_decorator(my_thread));
        boost::thread th4(bith::make_decorator(my_thread));
        boost::thread th5(bith::make_decorator(my_thread));

        th1.join();
        th2.join();
        th3.join();
        th4.join();
        th5.join();
        sleep(2);
        return 0;
    }
