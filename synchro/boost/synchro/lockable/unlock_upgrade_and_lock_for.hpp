//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_LOCKABLE_UNLOCK_UPGRADE_AND_LOCK_FOR__HPP
#define BOOST_SYNCHRO_LOCKABLE_UNLOCK_UPGRADE_AND_LOCK_FOR__HPP

#include <boost/chrono/chrono.hpp>
#include <boost/config/abi_prefix.hpp>

namespace boost { namespace synchro { namespace lockable {

    namespace result_of {
        template <typename Lockable>
        struct unlock_upgrade_and_lock_for {
            typedef void type;
        };
    }

    namespace partial_specialization_workaround {
        template <typename Lockable, class Rep, class Period >
        struct unlock_upgrade_and_lock_for {
            static void 
            apply( Lockable& lockable, const chrono::duration<Rep, Period>& rel_time ) {
                return lockable.unlock_upgrade_and_lock_for(rel_time);
            }
        };
    }

    template <typename Lockable, class Rep, class Period >
    void 
    unlock_upgrade_and_lock_for(Lockable& lockable, const chrono::duration<Rep, Period>& abs_time) {
        return partial_specialization_workaround::unlock_upgrade_and_lock_for<Lockable,Rep,Period>::apply(lockable, abs_time);
    }


}}}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif
