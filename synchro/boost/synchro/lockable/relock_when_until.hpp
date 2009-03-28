//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_LOCKABLE_RELOCK_WHEN_UNTIL__HPP
#define BOOST_SYNCHRO_LOCKABLE_RELOCK_WHEN_UNTIL__HPP

#include <boost/chrono/chrono.hpp>
#include <boost/config/abi_prefix.hpp>

namespace boost { namespace synchro { namespace lockable {

    namespace result_of {
        template <typename Lockable, typename Predicate, typename T>
        struct relock_when_until {
            typedef void type;
        };
    }

    namespace partial_specialization_workaround {
        template <typename Lockable, typename Predicate, class Clock, class Duration >
        struct relock_when_until {
            static typename result_of::template relock_when_until<Lockable,Predicate,chrono::time_point<Clock, Duration> >::type 
            apply( Lockable& lockable, Predicate pred, const chrono::time_point<Clock, Duration>& abs_time ) {
                return lockable.relock_when_until(pred, abs_time);
            }
        };
    }

    template <typename Lockable, typename Predicate, class Clock, class Duration >
    typename result_of::template relock_when_until<Lockable,Predicate,chrono::time_point<Clock, Duration> >::type 
    relock_when_until(Lockable& lockable, Predicate pred, const chrono::time_point<Clock, Duration>& abs_time) {
        return partial_specialization_workaround::relock_when_until<Lockable,Predicate,Clock,Duration>::apply(lockable, pred, abs_time);
    }


}}}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif
