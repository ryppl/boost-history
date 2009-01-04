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
#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace interthreads {

#if 0   
template <typename F>    
struct par {
    
};    

par(f1) || f2 

#endif

    template<typename F1, typename F2>
    inline void conc_join_all(F1 f1, F2 f2)
    {
    	thread_tuple<2> stg(f1, f2);
    	stg.join_all();
    }
    
#if 0    
    template<typename F0, typename F1>
    tuple<
        typename result_of<F0()>::type
      , typename result_of<F1()>::type 
    > 
    conc_all(F0 f0, F1 f1)
    {
    	future_tuple<typename result_of<F0()>::type, typename result_of<F1()>::type > ft(f0, f1);
    	return ft.get_all();
    }   
#endif

    template<typename F1, typename F2, typename F3>
    inline void conc_join_all(F1 f1, F2 f2, F3 f3)
    {
    	thread_tuple<3> stg(f1, f2, f3);
    	stg.join_all();
    }

    template<typename F1, typename F2>
    inline void conc_join_all_until(const system_time& wait_until, F1 f1, F2 f2)
    {
    	thread_tuple<2> stg(f1, f2);
    	stg.join_all_until(wait_until);
    }
    
    template<typename F1, typename F2, typename F3>
    inline void conc_join_all_until(const system_time& wait_until, F1 f1, F2 f2, F3 f3)
    {
    	thread_tuple<3> stg(f1, f2, f3);
    	stg.join_all_until(wait_until);
    }
    
    template<typename TimeDuration, typename F1, typename F2>
    inline std::size_t conc_join_all_for(TimeDuration wait_for, F1 f1, F2 f2)
    {
        return conc_join_all_until(get_system_time()+wait_for, f1, f2);
    }
    
    template<typename TimeDuration, typename F1, typename F2, typename F3>
    inline std::size_t conc_join_all_for(TimeDuration wait_for, F1 f1, F2 f2, F3 f3)
    {
        return conc_join_all_until(get_system_time()+wait_for, f1, f2, f3);
    }
    
    
    template<typename F1, typename F2>
    inline std::size_t conc_join_any(F1 f1, F2 f2)
    {
    	thread_tuple_once<2> stg(f1, f2);
    	std::size_t res= stg.join_any();
    	stg.interrupt_all();
        return res;
    }
    
    template<typename F1, typename F2, typename F3>
    inline std::size_t conc_join_any(F1 f1, F2 f2, F3 f3)
    {
    	thread_tuple_once<3> stg(f1, f2, f3);
    	std::size_t res= stg.join_any();
    	stg.interrupt_all();
        return res;
    }

    template<typename F1, typename F2>
    inline std::size_t conc_join_any_until(const system_time& wait_until, F1 f1, F2 f2)
    {
    	thread_tuple_once<2> stg(f1, f2);
    	std::pair<bool,std::size_t> res= stg.join_any_until(wait_until);
        if (res.first) stg.interrupt_all();
        return res;
    }
    
    template<typename F1, typename F2, typename F3>
    inline std::size_t conc_join_any_until(const system_time& wait_until, F1 f1, F2 f2, F3 f3)
    {
    	thread_tuple_once<3> stg(f1, f2, f3);
    	std::pair<bool,std::size_t> res= stg.join_any_until(wait_until);
    	if (res.first) stg.interrupt_all();
        return res;
    }
    
    template<typename TimeDuration, typename F1, typename F2>
    inline std::size_t conc_join_any_for(TimeDuration wait_for, F1 f1, F2 f2)
    {
        return conc_join_any_until(get_system_time()+wait_for, f1, f2);
    }
    
    template<typename TimeDuration, typename F1, typename F2, typename F3>
    inline std::size_t conc_join_any_for(TimeDuration wait_for, F1 f1, F2 f2, F3 f3)
    {
        return conc_join_any_until(get_system_time()+wait_for, f1, f2, f3);
    }
}    
}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif
