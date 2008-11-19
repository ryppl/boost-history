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
boost::mutex out_global_mutex;
#include <boost/thread/locks.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <boost/interthreads/thread_specific_shared_ptr.hpp>
#include <boost/interthreads/thread_decorator.hpp>

void sleep(int sec)
{
	boost::xtime t;
	boost::xtime_get(&t,1);	t.sec += sec; boost::thread::sleep(t);
}

// A state class living in shared memory.
// The class must have its own sync mechanism.
class state {
public:
	state() : result(0)
	{}
	void set_result(int n)
	{
		boost::mutex::scoped_lock lock(monitor);
		result = n;
	}
	int get_result()
	{
		boost::mutex::scoped_lock lock(monitor);
		return result;
	}
    static void deleter(state * p) { delete p; }
private:
    ~state() {}    
	boost::mutex monitor;
	int result;
};

// The conventional tss pointer.
// It can be seen as a memory space that is
// only privately accessible by the thread,
// when interpreted in our context.
boost::thread_specific_ptr<int> private_int;

// The thread member pointer.
// Its syntax is almost the same when used
// from "inside" a thread as that of the
// thread_specific_ptr. I named it public
// to underpin the fact it is accessible
// from "outside" too. (Perhaps other names
// than thread_specific_shared_ptr and thread_specific_ptr
// would be more appropriate? Any suggestions?
// E.g. thread_public_ptr and thread_private_ptr?)
boost::interthreads::thread_specific_shared_ptr<state> public_state;

// It might be convenient to have a function
// that is called automatically on thread start
// up to auto initialize the variables.
void init_state_fn()
{
    {
	    boost::mutex::scoped_lock out_guard(out_global_mutex);
	    std::cout << "init_state_fn" << " " << boost::this_thread::get_id()<<std::endl;
    }

	public_state.reset(new state(), state::deleter);
	private_int.reset(new int(0));
}

boost::interthreads::thread_decoration init_state(init_state_fn);


void run()
{
    init_state_fn();
	sleep(2);
    {
	    boost::mutex::scoped_lock out_guard(out_global_mutex);
	    std::cout << "run " << " " << boost::this_thread::get_id()<<std::endl;
    }
	public_state->set_result(42);
    
	boost::shared_ptr<state> ths = public_state[boost::this_thread::get_id()];
    int result;
	result = ths->get_result();
    {
	    boost::mutex::scoped_lock out_guard(out_global_mutex);
	    std::cout << "ahead " << result << " " << boost::this_thread::get_id()<<std::endl;
    }
	sleep(2);
    
}

int main(int argc, char* argv[])
{
    int result;

	boost::thread* th = new boost::thread(boost::interthreads::make_decorator(run));

    const boost::shared_ptr<state> ths = public_state.wait_and_get(th->get_id());
    if (ths.get()!=0) {
	    result = ths->get_result();
        {
    	    boost::mutex::scoped_lock out_guard(out_global_mutex);
	        std::cout << "ahead " << result << " " << boost::this_thread::get_id()<<std::endl;
        }
    }

	private_int.reset(new int(0));
	ths->set_result(1111);
    if (ths.get()!=0) {
	    result = ths->get_result();
        {
    	    boost::mutex::scoped_lock out_guard(out_global_mutex);
	        std::cout << "ahead " << result << " " << boost::this_thread::get_id()<<std::endl;
        }
    }
    
    sleep(2);
	boost::shared_ptr<state> ths3 = public_state[th->get_id()];
    if (ths.get()!=0) {
	    result = ths->get_result();
        {
    	    boost::mutex::scoped_lock out_guard(out_global_mutex);
	        std::cout << "ahead " << result << " " << boost::this_thread::get_id()<<std::endl;
        }
    }

	th->join();

	delete th;

	// We still may access the state object by means of the shared_ptr.
    if (ths.get()!=0) {
    	result = ths->get_result();
        {
	        boost::mutex::scoped_lock out_guard(out_global_mutex);
	        std::cout << "ahead " << result << " " << boost::this_thread::get_id()<<std::endl;
        }
    }
    const boost::shared_ptr<state> cths(new state(), state::deleter);
    cths->set_result(2);
    {
        boost::mutex::scoped_lock out_guard(out_global_mutex);
        std::cout << "xxx" << cths->get_result() << " " << boost::this_thread::get_id()<<std::endl;
    }
	return 0;
}

