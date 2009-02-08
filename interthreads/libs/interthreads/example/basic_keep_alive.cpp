//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Roland Schwarz 2006. 
// (C) Copyright Vicente J. Botet Escriba 2008-2009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
// Based on the shared.cpp example from the threadalert library of Roland Schwarz 
//////////////////////////////////////////////////////////////////////////////

#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#define COUT_
#ifdef COUT_
#else          
boost::mutex out_global_mutex2;
#endif
void sleep(int sec)
{
    boost::xtime t;
    boost::xtime_get(&t,1);
    t.sec += sec; 
    boost::thread::sleep(t);
}


#include <boost/interthreads/thread_decorator.hpp>
#include <boost/interthreads/thread_keep_alive.hpp>

#include <boost/interthreads/typeof/threader_decorator.hpp>
#include <boost/interthreads/algorithm.hpp>

#include "./async_ostream.hpp"
#include <boost/thread/thread.hpp>
#include <boost/static_assert.hpp>
#include <iostream>
#include <sstream>
        
namespace bith = boost::interthreads;
void my_thread1_on_dead_thread(boost::thread::id id, boost::thread*) {
    std::cout << "my_thread1 " << id << " do not responds to keep-alive" << std::endl;
}


int my_thread11() {
    bith::this_thread::enable_keep_alive enabler;
    bith::this_thread::set_on_dead_thread(my_thread1_on_dead_thread);     
    boost::this_thread::sleep(boost::posix_time::milliseconds(2000));
    bith::this_thread::keep_alive_point();            
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    bith::this_thread::keep_alive_point();            
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    bith::this_thread::keep_alive_point();            

#ifdef COUT_
        bith::cout_ << "my_thread1" << std::endl;
        bith::cout_.flush();
#else
        {
            boost::lock_guard<boost::mutex> lock(out_global_mutex2);
            std::cout << "my_thread1" << std::endl;
        }
#endif            
    return 0;
}
int my_thread() {
    bith::this_thread::enable_keep_alive enabler;
    for (unsigned i=0; i<10; i++) {
        bith::this_thread::keep_alive_point();            

        boost::thread::id id= boost::this_thread::get_id();
        std::stringstream sss;
#ifdef COUT_
        bith::cout_ << "TID=" << i << " " << id << std::endl;
        bith::cout_.flush();
#else
        {
            boost::lock_guard<boost::mutex> lock(out_global_mutex2);
            std::cout << "TID=" << i << " " << id << std::endl;
        }
#endif            
        boost::this_thread::sleep(boost::posix_time::milliseconds(500));
    }
    return 0;
}
    
int main() {
    
    bith::shared_threader_decorator ae;
    bith::wait_for_all(ae, my_thread, my_thread, my_thread11);
    return 0;
}
