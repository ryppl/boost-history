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

#ifndef BOOST_SYNCHO_THREAD_MUTEX_HPP
#define BOOST_SYNCHO_THREAD_MUTEX_HPP

//-----------------------------------------------------------------------------
#include <boost/thread/mutex.hpp>
//-----------------------------------------------------------------------------
#include <boost/synchro/exceptions.hpp>
#include <boost/synchro/time.hpp>
#include <boost/synchro/lockable.hpp>
//-----------------------------------------------------------------------------

namespace boost { namespace synchro {

#ifdef BOOST_PTHREAD_HAS_TIMEDLOCK
    template<>
    inline bool lock_until<boost::timed_mutex>(
                boost::timed_mutex& lockable, system_time const& abs_time BOOST_STM_CALL_CONTEXT_2_PARAMS()) {
        return lockable.timed_lock(abs_time);
    }
#endif

}}
#endif // BOOST_SYNCHO_THREAD_MUTEX_HPP


