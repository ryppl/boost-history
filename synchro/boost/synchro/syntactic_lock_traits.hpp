//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_SYNTACTIC_LOCK_TRAITS__HPP
#define BOOST_SYNCHRO_SYNTACTIC_LOCK_TRAITS__HPP

#include <boost/synchro/lockable_scope_traits.hpp>
#include <boost/synchro/lockable_traits.hpp>

namespace boost { namespace synchro {

//[syntactic_lock_traits
template<typename Lockable>
struct syntactic_lock_traits
: lockable_scope_traits<typename scope_tag<Lockable>::type, Lockable> {};
//]
}
}

#endif
