//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_LOCKABLE_RELOCK_WHEN_FOR__HPP
#define BOOST_SYNCHRO_LOCKABLE_RELOCK_WHEN_FOR__HPP

#include <boost/chrono/chrono.hpp>
#include <boost/config/abi_prefix.hpp>

namespace boost { namespace synchro { namespace lockable {

    namespace result_of {
        template <typename Lockable, typename Predicate, typename T>
        struct relock_when_for {
            typedef void type;
        };
    }

    namespace partial_specialization_workaround {
        template <typename Lockable,typename Predicate, class Rep, class Period >
        struct relock_when_for {
            static typename result_of::template relock_when_for<Lockable,Predicate,chrono::duration<Rep, Period> >::type 
            apply( Lockable& lockable, Predicate pred, const chrono::duration<Rep, Period>& rel_time ) {
                return lockable.relock_when_for(pred, rel_time);
            }
        };
    }

    template <typename Lockable, typename Predicate, class Rep, class Period >
    typename result_of::template relock_when_for<Lockable, Predicate, chrono::duration<Rep, Period> >::type 
    relock_when_for(Lockable& lockable, Predicate pred, const chrono::duration<Rep, Period>& abs_time) {
        return partial_specialization_workaround::relock_when_for<Lockable,Predicate,Rep,Period>::apply(lockable, pred, abs_time);
    }


}}}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif
