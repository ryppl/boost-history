//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/async for documentation.
//
// Based on the shared.cpp example from the threadalert library of Roland Schwarz 
//////////////////////////////////////////////////////////////////////////////

#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
boost::mutex out_global_mutex;

void sleep(int sec)
{
    boost::xtime t;
    boost::xtime_get(&t,1);
    t.sec += sec; 
    boost::thread::sleep(t);
}


#include <boost/async/typeof/threader.hpp>
#include <boost/async/wait_for_any.hpp>
#include <iostream>
        
namespace basync = boost::async;

int my_thread1() {
    sleep(3);
    {
        boost::lock_guard<boost::mutex> lock(out_global_mutex);
        std::cout << "1 thread_id=" << boost::this_thread::get_id() << std::endl;
    }
    return 0;
}
    
int my_thread2() {
    sleep(1);
    {
        boost::lock_guard<boost::mutex> lock(out_global_mutex);
        std::cout << "2 thread_id=" << boost::this_thread::get_id() << std::endl;
    }
    return 0;
}

int my_thread3() {
    sleep(2);
    {
        boost::lock_guard<boost::mutex> lock(out_global_mutex);
        std::cout << "3 thread_id=" << boost::this_thread::get_id() << std::endl;
    }
    return 0;
}


int main() {
    basync::shared_threader ae;
    BOOST_AUTO(result,basync::wait_for_any(ae, my_thread1, my_thread2, my_thread3));
    std::cout << "Algotithm " << result.first+1 << " finished the first with wait_for_any result=" << result.second << std::endl;
       
    return 0;
}
