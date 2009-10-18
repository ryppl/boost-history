//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Justin E. Gottchlich 2009.
// (C) Copyright Vicente J. Botet Escriba 2009.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/stm for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHO_PTHREAD_MUTEX_HPP
#define BOOST_SYNCHO_PTHREAD_MUTEX_HPP

//-----------------------------------------------------------------------------
#include <pthread.h>
//-----------------------------------------------------------------------------
#include <boost/synchro/detail/config.hpp>
//-----------------------------------------------------------------------------
#include <boost/synchro/exceptions.hpp>
#include <boost/synchro/time.hpp>
#include <boost/synchro/lockable.hpp>
//-----------------------------------------------------------------------------

namespace boost { namespace synchro {

    template<>
    inline void lock<pthread_mutex_t>(pthread_mutex_t& lockable) {
        //int const res=
        pthread_mutex_lock(&lockable);
        //if (res!=0) throw lock_error();
    }

    template<>
    inline void unlock<pthread_mutex_t>(pthread_mutex_t& lockable) {
        //int const res=
        pthread_mutex_unlock(&lockable);
        //if (res!=0) throw lock_error();
    }

    template<>
    inline bool try_lock<pthread_mutex_t>(pthread_mutex_t& lockable) {
        int const res=pthread_mutex_trylock(&lockable);
        //BOOST_ASSERT(!res || res==EBUSY);
        return (!res);
    }

#ifdef BOOST_PTHREAD_HAS_TIMEDLOCK
    template<>
    inline bool lock_until<pthread_mutex_t>(
                pthread_mutex_t& lockable, system_time const& abs_time) {
        struct timespec const timeout=detail::get_timespec(abs_time);
        int const res=pthread_mutex_timedlock(&lockable,&timeout);
        //BOOST_ASSERT(!res || res==ETIMEDOUT);
        return !res;
    }   
#endif

    
}}
#endif // BOOST_SYNCHO_TRY_LOCK_HPP


