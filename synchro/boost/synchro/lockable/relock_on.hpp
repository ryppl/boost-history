//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_LOCKABLE_RELOCK_ON__HPP
#define BOOST_SYNCHRO_LOCKABLE_RELOCK_ON__HPP

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace synchro { namespace lockable {

    namespace result_of {
        template <typename Lockable>
        struct relock_on {
            typedef void type;
        };
    }

    namespace partial_specialization_workaround {
        template< typename Lockable >
        struct relock_on {
            static typename result_of::template relock_on<Lockable>::type apply( Lockable& lockable ) {
                return lockable.relock_on();
            }
        };
    }

    template <typename Lockable>
    typename result_of::template relock_on<Lockable>::type 
    relock_on(Lockable& lockable) {
        return partial_specialization_workaround::relock_on<Lockable>::apply(lockable);
    }


}}}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif
