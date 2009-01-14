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

#include <boost/fusion/include/for_each.hpp>

#include <boost/interthreads/thread_decorator.hpp>
#include <boost/interthreads/thread_specific_shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/interthreads/fork_all.hpp>
#include <boost/interthreads/algorithm.hpp>
#include <boost/interthreads/typeof/threader_decorator.hpp>

#include <iostream>
        
namespace bith = boost::interthreads;

struct mono_thread_id {
    static bith::thread_decoration decoration_;
    typedef bith::thread_specific_shared_ptr<unsigned> tssp_type;
    static tssp_type current_;
    static unsigned counter_;
    static boost::mutex sync_;
        
    static unsigned create() {
        boost::lock_guard<boost::mutex> lock(sync_);
        unsigned res = counter_;
        ++counter_;
        return res;
    }
    static void setup() {
            std::cout << "setup" << std::endl;
        current_.reset(new unsigned(create()));
    }
public:        
    static unsigned id() {
        return *current_;
    }
    static unsigned id(boost::thread::id id) {

        boost::shared_ptr<unsigned> shp = current_[id];
        if (shp.get()!=0) {
            return *shp;
        } else {
            return 0xffffffff;
        }
    }

};

struct mono_thread_id_out {
    template<typename T>
    void operator()(T& t) const {
        std::cout << "thread::id=" << t.get_id() 
              << " mono_thread_id=" << mono_thread_id::id(t.get_id()) 
              << std::endl;
    }
};

struct mono_thread_id_wait_and_get {
    template<typename T>
    void operator()(T& t) const {
        const boost::shared_ptr<unsigned> shp1 = mono_thread_id::current_.wait_and_get(t.get_id());
        if (shp1.get()==0) {
            boost::lock_guard<boost::mutex> lock(out_global_mutex);
            std::cout << "ERROR 1" << std::endl;
        }
    }
};

bith::thread_decoration mono_thread_id::decoration_(mono_thread_id::setup);
mono_thread_id::tssp_type mono_thread_id::current_;
unsigned mono_thread_id::counter_=0;
boost::mutex mono_thread_id::sync_;
    
void my_thread() {
    {
        boost::lock_guard<boost::mutex> lock(out_global_mutex);
        std::cout << ">> thread::id=" << boost::this_thread::get_id()  << " mono_thread_id=" << mono_thread_id::id() << std::endl;
    }
    sleep(5);
    {
        boost::lock_guard<boost::mutex> lock(out_global_mutex);
        std::cout << "<< thread::id=" << boost::this_thread::get_id()  << " mono_thread_id=" << mono_thread_id::id() << std::endl;
    }
}

void doit() {
    bith::shared_threader_decorator ae;
    bith::result_of::fork_all<bith::shared_threader_decorator, boost::fusion::tuple<void(*)(), void(*)()> >::type handles = 
               bith::fork_all(ae, my_thread, my_thread);
    {
        const boost::shared_ptr<unsigned> shp1 = mono_thread_id::current_.wait_and_get(boost::fusion::at_c<0>(handles).get_id());
        if (shp1.get()==0) {
            boost::lock_guard<boost::mutex> lock(out_global_mutex);
            std::cout << "ERROR 1" << std::endl;
        }
    }
    {
        const boost::shared_ptr<unsigned> shp1 = mono_thread_id::current_.wait_and_get(boost::fusion::at_c<1>(handles).get_id());
        if (shp1.get()==0) {
            boost::lock_guard<boost::mutex> lock(out_global_mutex);
            std::cout << "ERROR 1" << std::endl;
        }
    }
    //sleep(1);
    boost::fusion::for_each(handles, mono_thread_id_wait_and_get());
    boost::fusion::for_each(handles, mono_thread_id_out());
    bith::join_all(handles);
}

void doit_clasic() {
    boost::thread th1(bith::make_decorator(my_thread));
    boost::thread th2(bith::make_decorator(my_thread));
        
    const boost::shared_ptr<unsigned> shp1 = mono_thread_id::current_.wait_and_get(th1.get_id());
    if (shp1.get()==0) {
        boost::lock_guard<boost::mutex> lock(out_global_mutex);
        std::cout << "ERROR 1" << std::endl;
    }

    const boost::shared_ptr<unsigned> shp2 = mono_thread_id::current_.wait_and_get(th2.get_id());
    if (shp2.get()==0) {
        boost::lock_guard<boost::mutex> lock(out_global_mutex);
        std::cout << "ERROR 2" << std::endl;
    }

    {
        unsigned u = mono_thread_id::id(th1.get_id());
        boost::lock_guard<boost::mutex> lock(out_global_mutex);
        std::cout << "thread::id=" << th1.get_id() <<  " mono_thread_id=" 
        << u << std::endl;
    }    
        
    th1.join();
    th2.join();
}
int main() {
    doit();
    return 0;
}
    
