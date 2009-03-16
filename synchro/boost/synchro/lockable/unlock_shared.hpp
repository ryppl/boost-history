//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_LOCKABLE_UNLOCK_SHARED__HPP
#define BOOST_SYNCHRO_LOCKABLE_UNLOCK_SHARED__HPP

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace synchro { namespace lockable {

    namespace result_of {
        template <typename Lockable>
        struct unlock_shared {
            typedef void type;
        };
    }

    namespace partial_specialization_workaround {
        template< typename Lockable >
        struct unlock_shared {
            static typename result_of::template unlock_shared<Lockable>::type apply( Lockable& lockable ) {
                return lockable.unlock_shared();
            }
        };
    }

    template <typename Lockable>
    typename result_of::template unlock_shared<Lockable>::type 
    unlock_shared(Lockable& lockable) {
        return partial_specialization_workaround::unlock_shared<Lockable>::apply(lockable);
    }


}}}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif
