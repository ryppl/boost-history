//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_THREAD_LOCKS__HPP
#define BOOST_SYNCHRO_THREAD_LOCKS__HPP


#include <boost/thread/locks.hpp>
#include <boost/synchro/lockable_traits.hpp>

namespace boost { namespace synchro {

template <typename T>
struct lockable_type<boost::unique_lock<T> > {
	typedef T type;
};
template <typename T>
struct lockable_type<boost::shared_lock<T> > {
	typedef T type;
};
template <typename T>
struct lockable_type<boost::upgrade_lock<T> > {
	typedef T type;
};

}
}

#endif
