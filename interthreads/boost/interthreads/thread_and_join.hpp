//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERTHREADS_THREAD_AND_JOIN_HPP
#define BOOST_INTERTHREADS_THREAD_AND_JOIN_HPP

#include <boost/bind.hpp>
#include <boost/thread/thread_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/detail/move.hpp>
#include <boost/interthreads/thread_tuple.hpp>
#include <boost/interthreads/thread_tuple_once.hpp>
#include <utility>

namespace boost {
namespace interthreads {

    template<typename F1, typename F2>
    inline void thread_and_join_all(F1 f1, F2 f2)
    {
    	thread_tuple<2> stg(f1, f2);
    	stg.join_all();
    }
    template<typename F1, typename F2, typename F3>
    inline void thread_and_join_all(F1 f1, F2 f2, F3 f3)
    {
    	thread_tuple<3> stg(f1, f2, f3);
    	stg.join_all();
    }

    template<typename F1, typename F2>
    inline void thread_and_timed_join_all(const system_time& wait_until, F1 f1, F2 f2)
    {
    	thread_tuple<2> stg(f1, f2);
    	stg.timed_join_all(wait_until);
    }
    
    template<typename F1, typename F2, typename F3>
    inline void thread_and_timed_join_first_all(const system_time& wait_until, F1 f1, F2 f2, F3 f3)
    {
    	thread_tuple<3> stg(f1, f2, f3);
    	stg.timed_join_all(wait_until);
    }
    
    template<typename TimeDuration, typename F1, typename F2>
    inline std::size_t thread_and_timed_join_all(TimeDuration wait_for, F1 f1, F2 f2)
    {
        return thread_and_timed_join_all(get_system_time()+wait_for, f1, f2);
    }
    
    template<typename TimeDuration, typename F1, typename F2, typename F3>
    inline std::size_t thread_and_join_all(TimeDuration wait_for, F1 f1, F2 f2, F3 f3)
    {
        return thread_and_timed_join_all(get_system_time()+wait_for, f1, f2, f3);
    }
    
    
    
    template<typename F1, typename F2>
    inline std::size_t thread_and_join_first_then_interrupt(F1 f1, F2 f2)
    {
    	thread_tuple_once<2> stg(f1, f2);
    	std::size_t res= stg.join_first();
    	stg.interrupt_all();
        return res;
    }
    
    template<typename F1, typename F2, typename F3>
    inline std::size_t thread_and_join_first_then_interrupt(F1 f1, F2 f2, F3 f3)
    {
    	thread_tuple_once<3> stg(f1, f2, f3);
    	std::size_t res= stg.join_first();
    	stg.interrupt_all();
        return res;
    }

    template<typename F1, typename F2>
    inline std::size_t thread_and_timed_join_first_then_interrupt(const system_time& wait_until, F1 f1, F2 f2)
    {
    	thread_tuple_once<2> stg(f1, f2);
    	std::pair<bool,std::size_t> res= stg.timed_join_first();
        if (res.first) stg.interrupt_all();
        return res;
    }
    
    template<typename F1, typename F2, typename F3>
    inline std::size_t thread_and_timed_join_first_then_interrupt(const system_time& wait_until, F1 f1, F2 f2, F3 f3)
    {
    	thread_tuple_once<3> stg(f1, f2, f3);
    	std::pair<bool,std::size_t> res= stg.join_first();
    	if (res.first) stg.interrupt_all();
        return res;
    }
    
    template<typename TimeDuration, typename F1, typename F2>
    inline std::size_t thread_and_timed_join_first_then_interrupt(TimeDuration wait_for, F1 f1, F2 f2)
    {
        return thread_and_timed_join_first_then_interrupt(get_system_time()+wait_for, f1, f2);
    }
    
    template<typename TimeDuration, typename F1, typename F2, typename F3>
    inline std::size_t thread_and_join_first_then_interrupt(TimeDuration wait_for, F1 f1, F2 f2, F3 f3)
    {
        return thread_and_timed_join_first_then_interrupt(get_system_time()+wait_for, f1, f2, f3);
    }
}    
}   // namespace boost


#endif
