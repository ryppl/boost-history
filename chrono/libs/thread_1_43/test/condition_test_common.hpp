#ifndef CONDITION_TEST_COMMON_HPP
#define CONDITION_TEST_COMMON_HPP
// Copyright (C) 2007 Anthony Williams
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread_time.hpp>

#ifdef BOOST_THREAD_TEST_CHRONO        
    using namespace boost::chrono;
    typedef system_clock::time_point system_time_point;
    system_time_point now() {return system_clock::now();}
#else        
    using namespace boost::posix_time;
    typedef boost::system_time system_time_point;
    system_time_point now() {return boost::get_system_time();}
#endif    


unsigned const timeout_seconds=5;
    
struct wait_for_flag
{
    boost::mutex mutex;
    boost::condition_variable cond_var;
    bool flag;
    unsigned woken;
        
    wait_for_flag():
        flag(false),woken(0)
    {}

    struct check_flag
    {
        bool const& flag;
            
        check_flag(bool const& flag_):
            flag(flag_)
        {}
            
        bool operator()() const
        {
            return flag;
        }
    private:
        void operator=(check_flag&);
    };

        
    void wait_without_predicate()
    {
        boost::mutex::scoped_lock lock(mutex);
        while(!flag)
        {
            cond_var.wait(lock);
        }
        ++woken;
    }

    void wait_with_predicate()
    {
        boost::mutex::scoped_lock lock(mutex);
        cond_var.wait(lock,check_flag(flag));
        if(flag)
        {
            ++woken;
        }
    }

    void timed_wait_without_predicate()
    {
        system_time_point const timeout=now()+seconds(timeout_seconds);
            
        boost::mutex::scoped_lock lock(mutex);
        while(!flag)
        {
#ifdef BOOST_THREAD_TEST_CHRONO        
            if(!cond_var.wait_until(lock,timeout))
#else
            if(!cond_var.timed_wait(lock,timeout))
#endif
            {
                return;
            }
        }
        ++woken;
    }

    void timed_wait_with_predicate()
    {
        system_time_point const timeout=now()+seconds(timeout_seconds);
        boost::mutex::scoped_lock lock(mutex);
#ifdef BOOST_THREAD_TEST_CHRONO        
        if(cond_var.wait_until(lock,timeout,check_flag(flag)) && flag)
#else
        if(cond_var.timed_wait(lock,timeout,check_flag(flag)) && flag)
#endif
        {
            ++woken;
        }
    }
    void relative_timed_wait_with_predicate()
    {
        boost::mutex::scoped_lock lock(mutex);
#ifdef BOOST_THREAD_TEST_CHRONO        
        if(cond_var.wait_for(lock,seconds(timeout_seconds),check_flag(flag)) && flag)
#else
        if(cond_var.timed_wait(lock,seconds(timeout_seconds),check_flag(flag)) && flag)
#endif        
        {
            ++woken;
        }
    }
};


#endif
