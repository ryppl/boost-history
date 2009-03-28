//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_LOCKABLE_RELOCK_ON_UNTIL__HPP
#define BOOST_SYNCHRO_LOCKABLE_RELOCK_ON_UNTIL__HPP

#include <boost/chrono/chrono.hpp>
#include <boost/config/abi_prefix.hpp>

namespace boost { namespace synchro { namespace lockable {

    namespace result_of {
        template <typename Lockable, class Clock, class Duration >
        struct relock_on_until {
            typedef void type;
        };
    }

    namespace partial_specialization_workaround {
        template <typename Lockable, class Clock, class Duration >
        struct relock_on_until {
            static typename result_of::template relock_on_until<Lockable,Clock,Duration>::type 
            apply( Lockable& lockable, const chrono::time_point<Clock, Duration>& abs_time ) {
                return lockable.relock_on_until(abs_time);
            }
        };
    }

    template <typename Lockable, class Clock, class Duration >
    typename result_of::template relock_on_until<Lockable,Clock,Duration>::type 
    relock_on_until(Lockable& lockable, const chrono::time_point<Clock, Duration>& abs_time) {
        return partial_specialization_workaround::relock_on_until<Lockable,Clock,Duration>::apply(lockable, abs_time);
    }


}}}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif
