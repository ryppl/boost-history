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

#ifndef BOOST_SYNCHO_LOCKABLE_LOCK_FOR_HPP
#define BOOST_SYNCHO_LOCKABLE_LOCK_FOR_HPP

//-----------------------------------------------------------------------------
#include <boost/synchro/time.hpp>
#include <boost/synchro/lockable/lock_until.hpp>
//-----------------------------------------------------------------------------

namespace boost { namespace synchro {

    template <typename Lockable, typename TimeDuration >
    bool lock_for(Lockable& lockable, const TimeDuration& rel_time) {
        return lock_until(&lockable, get_system_time()+rel_time);
    }

}}
#endif // BOOST_SYNCHO_LOCKABLE_LOCK_FOR_HPP


