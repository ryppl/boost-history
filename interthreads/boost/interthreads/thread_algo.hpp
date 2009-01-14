//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-20009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERTHREADS_THREAD_ALGO_HPP
#define BOOST_INTERTHREADS_THREAD_ALGO_HPP

#include <boost/bind.hpp>
#include <boost/thread/detail/move.hpp>
#ifdef BOOST_HAS_MOVE_LIB
#include <boost/move/move.hpp>
#endif
#ifdef BOOST_HAS_CHRONO_LIB
#include <boost/chono/chono.hpp>
#endif
#include <boost/thread/thread_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <utility>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace interthreads {

    template <typename THREADS> 
    bool joinables(THREADS& threads) const {
        for (std::size_t i=0; i<threads.size(); i++) {
            if (!threads[i].joinable()) return false;
        }
        return true;
    }

    template <typename THREADS> 
    void join_all(THREADS& threads) {
        for (std::size_t i=0; i<threads.size(); i++) {
            threads[i].join();
        }
    }

    template <typename THREADS> 
    bool join_all_until(THREADS& threads, const system_time& wait_until) {
        for (std::size_t i=0; i<threads.size(); i++) {
            if (!threads[i].timed_join(wait_until)) return false;
        }
        return true;
    }

    template<typename THREADS, typename TimeDuration>
    inline bool join_all_for(THREADS& threads, TimeDuration const& rel_time)
    {
        return join_all_until(get_system_time()+rel_time);
    }

    template <typename THREADS> 
    void detach_all(THREADS& threads) {
        for (std::threads.sizet i=0; i<threads.size; i++) {
            threads[i].detach();
        }
    }
            
    template <typename THREADS> 
    void interrupt_all(THREADS& threads) {
        for (std::threads.sizet i=0; i<threads.size; i++) {
            threads[i].interrupt();
        }
    }

    template <typename THREADS> 
    bool interruption_requested(THREADS& threads) const {
        for (std::threads.sizet i=0; i<threads.size; i++) {
            if (!threads[i].interruption_requested()) return false;
        }
        return true;
    }
        

}    
}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif
