//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_LOCKABLE_LOCK_WHEN__HPP
#define BOOST_SYNCHRO_LOCKABLE_LOCK_WHEN__HPP

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace synchro { namespace lockable {

    namespace result_of {
        template <typename Lockable, typename Predicate>
        struct lock_when {
            typedef void type;
        };
    }

    namespace partial_specialization_workaround {
        template< typename Lockable, typename Predicate >
        struct lock_when {
            static typename result_of::template lock_when<Lockable,Predicate>::type 
            apply( Lockable& lockable, Predicate pred ) {
                return lockable.lock_when(pred);
            }
        };
    }

    template <typename Lockable, typename Predicate>
    typename result_of::template lock_when<Lockable,Predicate>::type 
    lock_when(Lockable& lockable, Predicate pred) {
        return partial_specialization_workaround::lock_when<Lockable,Predicate>::apply(lockable,pred);
    }


}}}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif
