//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_THREAD_SHARED_MUTEX__HPP
#define BOOST_SYNCHRO_THREAD_SHARED_MUTEX__HPP

#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/synchro/lockable_traits.hpp>
#include <boost/synchro/thread/lockable_scope_traits.hpp>

namespace boost { namespace synchro {

typedef boost::shared_mutex thread_shared_mutex;

template<>
struct timed_interface_tag<boost::shared_mutex> {
	typedef has_timed_interface_tag type;
};
template<>
struct reentrancy_tag<boost::shared_mutex> {
	typedef non_recursive_tag type;
};
template<>
struct category_tag<boost::shared_mutex> {
	typedef upgradable_lock_tag type;
};
template<>
struct scope_tag<boost::shared_mutex> {
	typedef multi_threaded_tag type;
};

template<>
struct lifetime_tag<boost::shared_mutex> {
	typedef process_lifetime_tag type;
};

template<>
struct naming_tag<boost::shared_mutex> {
	typedef anonymous_tag type;
};

template <>
struct best_condition<boost::shared_mutex> {
	typedef boost::condition_variable_any type;
};

template <>
struct best_condition_any<boost::shared_mutex> {
	typedef boost::condition_variable_any type;
};

}
}

#endif
