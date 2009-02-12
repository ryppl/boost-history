//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_IS_STRICT_LOCKER__HPP
#define BOOST_SYNCHRO_IS_STRICT_LOCKER__HPP

#include <boost/mpl/bool.hpp>

namespace boost { namespace synchro {

/**
 * An strict locker is a locker ensuring the mutex is locked on the scope of the lock
 * There is no only one way to define a strict lock as the strict_locker and
 * nesteed_strict_locker shows. So we need a metafunction that states if a
 * locker is a strict locker
 */

template <typename Locker>
struct is_strict_locker : mpl::false_ {};

}
}

#endif
