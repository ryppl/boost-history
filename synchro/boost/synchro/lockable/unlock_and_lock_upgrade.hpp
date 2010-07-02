//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_LOCKABLE_UNLOCK_AND_LOCK_UPGRADE__HPP
#define BOOST_SYNCHRO_LOCKABLE_UNLOCK_AND_LOCK_UPGRADE__HPP

#include <boost/config/abi_prefix.hpp>

//!\file
//!Describes a generic free unlock_and_lock_upgrade function that can be applied to any model of the Lockable concept

namespace boost { namespace synchro { namespace lockable {

    namespace result_of {
        template <typename Lockable>
        struct unlock_and_lock_upgrade {
            typedef void type;
        };
    }

    namespace partial_specialization_workaround {
        template< typename Lockable >
        struct unlock_and_lock_upgrade {
            static typename result_of::template unlock_and_lock_upgrade<Lockable>::type apply( Lockable& lockable ) {
                return lockable.unlock_and_lock_upgrade();
            }
        };
    }

    //!Effects: The calling thread tries to obtain ownership of the lockable, and
    //!   if another thread has ownership of the lockable, it waits until it can
    //!   obtain the ownership. If a thread takes ownership of the lockable the
    //!   lockable must be unlocked by the same thread.
    //!Throws: boost::system_exception on error.
    template <typename Lockable>
    typename result_of::template unlock_and_lock_upgrade<Lockable>::type 
    unlock_and_lock_upgrade(Lockable& lockable) {
        return partial_specialization_workaround::unlock_and_lock_upgrade<Lockable>::apply(lockable);
    }


}}}   // namespace boost

#include <boost/config/abi_suffix.hpp>

#endif