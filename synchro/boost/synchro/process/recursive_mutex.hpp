//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_PROCESS_RECURSIVE_MUTEX__HPP
#define BOOST_SYNCHRO_PROCESS_RECURSIVE_MUTEX__HPP

#include <boost/interprocess/sync/interprocess_recursive_mutex.hpp>
#include <boost/synchro/lockable_traits.hpp>
#include <boost/synchro/process/lockable_scope_traits.hpp>

namespace boost { namespace synchro {

typedef boost::interprocess::interprocess_recursive_mutex interprocess_recursive_mutex;

template<>
struct timed_interface_tag<boost::interprocess::interprocess_recursive_mutex> {
	typedef has_timed_interface_tag type;
};
template<>
struct reentrancy_tag<boost::interprocess::interprocess_recursive_mutex> {
	typedef recursive_tag type;
};
template<>
struct category_tag<boost::interprocess::interprocess_recursive_mutex> {
	typedef exclusive_lock_tag type;
};
template<>
struct scope_tag<boost::interprocess::interprocess_recursive_mutex> {
	typedef multi_process_tag type;
};

template<>
struct lifetime_tag<boost::interprocess::interprocess_recursive_mutex> {
	typedef kernel_lifetime_tag type;
};

template<>
struct naming_tag<boost::interprocess::interprocess_recursive_mutex> {
	typedef anonymous_tag type;
};


template <>
struct best_condition<boost::interprocess::interprocess_recursive_mutex> {
	typedef boost::interprocess::interprocess_condition type;
};

template <>
struct best_condition_any<boost::interprocess::interprocess_recursive_mutex> {
	typedef boost::interprocess::interprocess_condition type;
};


#if 0
template<>
struct syntactic_lock_traits<boost::interprocess::interprocess_recursive_mutex>
	: syntactic_process_lock_traits<boost::interprocess::interprocess_recursive_mutex> {};
#endif

}
}

#endif
