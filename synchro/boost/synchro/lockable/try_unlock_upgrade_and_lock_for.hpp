//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_LOCKABLE_TRY_UNLOCK_UPGRADE_AND_LOCK_FOR__HPP
#define BOOST_SYNCHRO_LOCKABLE_TRY_UNLOCK_UPGRADE_AND_LOCK_FOR__HPP

#include <boost/chrono/chrono.hpp>
#include <boost/config/abi_prefix.hpp>

namespace boost { namespace synchro { namespace lockable {

    namespace result_of {
    template <typename Lockable, class Rep, class Period >
        struct try_unlock_upgrade_and_lock_for {
            typedef bool type;
        };
    }

    namespace partial_specialization_workaround {
        template <typename Lockable, class Rep, class Period >
        struct try_unlock_upgrade_and_lock_for {
            static typename result_of::template try_unlock_upgrade_and_lock_for<Lockable,Rep, Period>::type 
            apply( Lockable& lockable, const chrono::duration<Rep, Period>& rel_time ) {
                return lockable.try_unlock_upgrade_and_lock_for(rel_time);
            }
        };
    }

    template <typename Lockable, class Rep, class Period >
    typename result_of::template try_unlock_upgrade_and_lock_for<Lockable,Rep, Period>::type 
    try_unlock_upgrade_and_lock_for(Lockable& lockable, const chrono::duration<Rep, Period>& rel_time) {
        return partial_specialization_workaround::try_unlock_upgrade_and_lock_for<Lockable,Rep, Period>::apply(lockable, rel_time);
    }


}}}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif
