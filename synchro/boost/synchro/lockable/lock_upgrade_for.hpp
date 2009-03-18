//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_LOCKABLE_LOCK_UPGRADE_FOR__HPP
#define BOOST_SYNCHRO_LOCKABLE_LOCK_UPGRADE_FOR__HPP

#include <boost/chrono/chrono.hpp>
#include <boost/config/abi_prefix.hpp>

namespace boost { namespace synchro { namespace lockable {

    namespace result_of {
        template <typename Lockable, class Rep, class Period >
        struct lock_upgrade_for {
            typedef void type;
        };
    }

    namespace partial_specialization_workaround {
        template <typename Lockable, class Rep, class Period >
        struct lock_upgrade_for {
            static typename result_of::template lock_upgrade_for<Lockable,Rep, Period>::type 
            apply( Lockable& lockable, const chrono::duration<Rep, Period>& rel_time ) {
                return lockable.lock_upgrade_for(rel_time);
            }
        };
    }

    template <typename Lockable, class Rep, class Period >
    typename result_of::template lock_upgrade_for<Lockable,Rep, Period>::type 
    lock_upgrade_for(Lockable& lockable, const chrono::duration<Rep, Period>& abs_time) {
        return partial_specialization_workaround::lock_upgrade_for<Lockable,Rep,Period>::apply(lockable, abs_time);
    }


}}}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif
