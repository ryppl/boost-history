//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_PROCESS_LOCK__HPP
#define BOOST_SYNCHRO_PROCESS_LOCKS__HPP

#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>
#include <boost/interprocess/sync/upgradable_lock.hpp>
#include <boost/synchro/lockable_traits.hpp>

namespace boost { namespace synchro {


template <typename T>
struct lockable_type<boost::interprocess::scoped_lock<T> > {
	typedef T type;
};
template <typename T>
struct lockable_type<boost::interprocess::sharable_lock<T> > {
	typedef T type;
};
template <typename T>
struct lockable_type<boost::interprocess::upgradable_lock<T> > {
	typedef T type;
};

}
}

#endif
