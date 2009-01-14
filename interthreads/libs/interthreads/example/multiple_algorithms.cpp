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

void sleep(int sec)
{
	boost::xtime t;
	boost::xtime_get(&t,1);	
    t.sec += sec; 
    boost::thread::sleep(t);
}


//    #include <boost/interthreads/thread_decorator.hpp>
    #include <boost/interthreads/thread_tuple.hpp>
    #include <boost/interthreads/thread_tuple_once.hpp>
    #include <boost/interthreads/thread_group_once.hpp>
    #include <boost/interthreads/thread_and_join.hpp>
    #include <boost/thread.hpp>
    #include <iostream>
        
    namespace bith = boost::interthreads;

    void my_thread1() {
        {
            boost::lock_guard<boost::mutex> lock(out_global_mutex);
            std::cout << "1 thread_id=" << boost::this_thread::get_id() << std::endl;
        }
        sleep(10);
    }
    
    void my_thread2() {
        {
            boost::lock_guard<boost::mutex> lock(out_global_mutex);
            std::cout << "2 thread_id=" << boost::this_thread::get_id() << std::endl;
        }
        sleep(5);
    }

    void my_thread3() {
        {
            boost::lock_guard<boost::mutex> lock(out_global_mutex);
            std::cout << "3 thread_id=" << boost::this_thread::get_id() << std::endl;
        }
        sleep(7);
    }

    int main() {

        bith::thread_tuple<3> tt_0(my_thread1, my_thread2, my_thread3);
        bith::thread_tuple<3> tt_1;
        tt_1= tt_0.move();
        bith::thread_tuple<3> tt_2(tt_1.move());
        bith::thread_tuple<3> tt(boost::move(tt_2));

        tt.join_all();
        std::cout << "All finished join_all" << std::endl;
        
        bith::thread_tuple<3> kk_0= bith::make_thread_tuple(my_thread1, my_thread2, my_thread3);
        kk_0.join_all();
        std::cout << "All finished join_all" << std::endl;

#if 0        
        
        bith::thread_group_once tgo;
        boost::thread* th1 = tgo.create_thread(my_thread1);
        boost::thread* th2 = tgo.create_thread(my_thread2);
        boost::thread* th3 = tgo.create_thread(my_thread3);
        boost::thread* res_tgo= tgo.join_any();
        std::cout << "Algotithm " << res_tgo << " " << th2 << " finished the first with thread_group_once::join_any" << std::endl;
        
       
        bith::conc_join_all(my_thread1, my_thread2, my_thread3);
        std::cout << "All finished conc_join_all" << std::endl;

        unsigned res = bith::conc_join_any(my_thread1, my_thread2, my_thread3);
        std::cout << "Algotithm " << res+1 << " finished the first with conc_join_any" << std::endl;
#endif        
        
        return 0;
    }
    
