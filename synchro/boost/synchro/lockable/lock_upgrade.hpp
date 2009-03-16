//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_LOCKABLE_LOCK_UPGRADE__HPP
#define BOOST_SYNCHRO_LOCKABLE_LOCK_UPGRADE__HPP

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace synchro { namespace lockable {

    namespace result_of {
        template <typename Lockable>
        struct lock_upgrade {
            typedef void type;
        };
    }

    namespace partial_specialization_workaround {
        template< typename Lockable >
        struct lock_upgrade {
            static typename result_of::template lock_upgrade<Lockable>::type apply( Lockable& lockable ) {
                return lockable.lock_upgrade();
            }
        };
    }

    template <typename Lockable>
    typename result_of::template lock_upgrade<Lockable>::type 
    lock_upgrade(Lockable& lockable) {
        return partial_specialization_workaround::lock_upgrade<Lockable>::apply(lockable);
    }


}}}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif
