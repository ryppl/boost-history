//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Roland Schwarz 2006. 
// (C) Copyright Vicente J. Botet Escriba 2008. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <boost/thread/tss.hpp>
#include <boost/thread.hpp>
#include <boost/interthreads/thread_decorator.hpp>
//#include <boost/interthreads/thread_tuple.hpp>

boost::mutex out_global_mutex;

void cleanup_i(boost::thread::id* data) {
	boost::mutex::scoped_lock out_guard(out_global_mutex);
	std::cout << "cleanup_i " << *data <<" " << boost::this_thread::get_id()<<std::endl;
}

boost::thread_specific_ptr<boost::thread::id> specific_ptr_i(cleanup_i);

boost::once_flag global_init_flag=BOOST_ONCE_INIT;
void global_init() {
	boost::mutex::scoped_lock out_guard(out_global_mutex);
	std::cout << "global_init" << boost::this_thread::get_id()<<std::endl;
}

void thread_init_1() {
	specific_ptr_i.reset(new boost::thread::id(boost::this_thread::get_id()));
	boost::call_once(global_init_flag, global_init);
	boost::mutex::scoped_lock out_guard(out_global_mutex);
	std::cout << "thread_init_1 " << boost::this_thread::get_id()<<std::endl;
}

void thread_init_2() {
	boost::mutex::scoped_lock out_guard(out_global_mutex);
	std::cout << "thread_init_2 " << boost::this_thread::get_id()<<std::endl;
}
void sleep(int sec)
{
	boost::xtime t;
	boost::xtime_get(&t,1);	
    t.sec += sec; 
    boost::thread::sleep(t);
}

void run1() {
	sleep(3);
	{
		boost::mutex::scoped_lock out_guard(out_global_mutex);
		std::cout << "long operation on" << boost::this_thread::get_id()<<std::endl;
	}
	sleep(10);
	{
		boost::mutex::scoped_lock out_guard(out_global_mutex);
	    std::cout << "long operation end" << boost::this_thread::get_id()<<std::endl;
    }
}

void run2() {
	sleep(1);
	{
		boost::mutex::scoped_lock out_guard(out_global_mutex);
		std::cout << "long operation on" << boost::this_thread::get_id()<<std::endl;
	}
	sleep(20);
	{
		boost::mutex::scoped_lock out_guard(out_global_mutex);
	    std::cout << "long operation end" << boost::this_thread::get_id()<<std::endl;
    }
}

boost::interthreads::thread_decoration p1(thread_init_1);
boost::interthreads::thread_decoration p2(thread_init_2);


int main(int argc, char* argv[])
{
    boost::interthreads::decorate();
	
	//boost::interthreads::thread_tuple<2> gr(boost::interthreads::make_decorator(run1), boost::interthreads::make_decorator(run2)); 
    boost::interthreads::thread_decorator run1p(run1);
    boost::interthreads::thread_decorator run2p(run2);
	boost::thread pt1(run1p);
	boost::thread pt2(run2p);
//	boost::thread pt1(boost::interthreads::make_decorator(run1));
//	boost::thread pt2(boost::interthreads::make_decorator(run2));
	sleep(1);
	boost::thread pt12(run1p);
//	boost::thread pt12(boost::interthreads::make_decorator(run1));
	sleep(1);
	{
		boost::mutex::scoped_lock out_guard(out_global_mutex);
	    std::cout << "waiting pt1" <<std::endl;
    }
    
	pt1.join();
	{
		boost::mutex::scoped_lock out_guard(out_global_mutex);
	    std::cout << "waiting pt12" <<std::endl;
    }
	pt12.join();
	//gr.join_all();
	{
		boost::mutex::scoped_lock out_guard(out_global_mutex);
	    std::cout << "waiting pt2" <<std::endl;
    }
	pt2.join();
	
	{
		boost::mutex::scoped_lock out_guard(out_global_mutex);
	    std::cout << "END" <<std::endl;
    }

	return 0;
}



