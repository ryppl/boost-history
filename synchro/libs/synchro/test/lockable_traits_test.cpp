/* Copyright 2008-2009 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */




#include <boost/synchro/lockable_traits.hpp>

#include <boost/synchro/null_mutex.hpp>
#include <boost/synchro/null_condition.hpp>

#include <boost/synchro/thread/mutex.hpp>
#include <boost/synchro/thread/recursive_mutex.hpp>
#include <boost/synchro/thread/shared_mutex.hpp>

#include <boost/synchro/process/mutex.hpp>
#include <boost/synchro/process/recursive_mutex.hpp>
#include <boost/synchro/process/upgradable_mutex.hpp>
#include <boost/synchro/process/named_mutex.hpp>
#include <boost/synchro/process/named_recursive_mutex.hpp>
#include <boost/synchro/process/named_upgradable_mutex.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/static_assert.hpp>

#define BOOST_TEST_MODULE sync_lock_traits_test

using namespace boost;
using namespace boost::synchro;


BOOST_STATIC_ASSERT((is_same<best_condition<null_mutex>::type, null_condition>::value));
BOOST_STATIC_ASSERT((is_same<scope_tag<null_mutex>::type, mono_threaded_tag>::value));
BOOST_STATIC_ASSERT((is_same<category_tag<null_mutex>::type, upgradable_lock_tag>::value));
BOOST_STATIC_ASSERT((is_same<reentrancy_tag<null_mutex>::type, recursive_tag>::value));
BOOST_STATIC_ASSERT((is_same<timed_interface_tag<null_mutex>::type, has_timed_interface_tag>::value));
BOOST_STATIC_ASSERT((is_exclusive_lock<null_mutex>::value));
BOOST_STATIC_ASSERT((is_sharable_lock<null_mutex>::value));
BOOST_STATIC_ASSERT((is_upgradable_lock<null_mutex>::value));
BOOST_STATIC_ASSERT((is_mono_threaded<null_mutex>::value));
BOOST_STATIC_ASSERT((!is_multi_threaded<null_mutex>::value));
BOOST_STATIC_ASSERT((!is_multi_process<null_mutex>::value));
BOOST_STATIC_ASSERT((is_recursive_lock<null_mutex>::value));
BOOST_STATIC_ASSERT((has_timed_interface<null_mutex>::value));

BOOST_STATIC_ASSERT((is_same<best_condition<synchro::thread_mutex>::type, boost::condition_variable>::value));
BOOST_STATIC_ASSERT((is_same<scope_tag<synchro::thread_mutex>::type, multi_threaded_tag>::value));
BOOST_STATIC_ASSERT((is_same<category_tag<synchro::thread_mutex>::type, exclusive_lock_tag>::value));
BOOST_STATIC_ASSERT((is_same<reentrancy_tag<synchro::thread_mutex>::type, non_recursive_tag>::value));
BOOST_STATIC_ASSERT((is_same<timed_interface_tag<synchro::thread_mutex>::type, hasnt_timed_interface_tag>::value));
BOOST_STATIC_ASSERT((is_exclusive_lock<synchro::thread_mutex>::value));
BOOST_STATIC_ASSERT((!is_sharable_lock<synchro::thread_mutex>::value));
BOOST_STATIC_ASSERT((!is_upgradable_lock<synchro::thread_mutex>::value));
BOOST_STATIC_ASSERT((is_mono_threaded<synchro::thread_mutex>::value));
BOOST_STATIC_ASSERT((is_multi_threaded<synchro::thread_mutex>::value));
BOOST_STATIC_ASSERT((!is_multi_process<synchro::thread_mutex>::value));
BOOST_STATIC_ASSERT((!is_recursive_lock<synchro::thread_mutex>::value));
BOOST_STATIC_ASSERT((!has_timed_interface<synchro::thread_mutex>::value));


BOOST_STATIC_ASSERT((is_same<scope_tag<synchro::thread_timed_mutex>::type, multi_threaded_tag>::value));
BOOST_STATIC_ASSERT((is_same<category_tag<synchro::thread_timed_mutex>::type, exclusive_lock_tag>::value));
BOOST_STATIC_ASSERT((is_same<reentrancy_tag<synchro::thread_timed_mutex>::type, non_recursive_tag>::value));
BOOST_STATIC_ASSERT((is_same<timed_interface_tag<synchro::thread_timed_mutex>::type, has_timed_interface_tag>::value));
BOOST_STATIC_ASSERT((is_exclusive_lock<synchro::thread_timed_mutex>::value));
BOOST_STATIC_ASSERT((!is_sharable_lock<synchro::thread_timed_mutex>::value));
BOOST_STATIC_ASSERT((!is_upgradable_lock<synchro::thread_timed_mutex>::value));
BOOST_STATIC_ASSERT((is_mono_threaded<synchro::thread_timed_mutex>::value));
BOOST_STATIC_ASSERT((is_multi_threaded<synchro::thread_timed_mutex>::value));
BOOST_STATIC_ASSERT((!is_multi_process<synchro::thread_timed_mutex>::value));
BOOST_STATIC_ASSERT((!is_recursive_lock<synchro::thread_timed_mutex>::value));
BOOST_STATIC_ASSERT((has_timed_interface<synchro::thread_timed_mutex>::value));

BOOST_STATIC_ASSERT((is_same<scope_tag<synchro::thread_recursive_mutex>::type, multi_threaded_tag>::value));
BOOST_STATIC_ASSERT((is_same<category_tag<synchro::thread_recursive_mutex>::type, exclusive_lock_tag>::value));
BOOST_STATIC_ASSERT((is_same<reentrancy_tag<synchro::thread_recursive_mutex>::type, recursive_tag>::value));
BOOST_STATIC_ASSERT((is_same<timed_interface_tag<synchro::thread_recursive_mutex>::type, hasnt_timed_interface_tag>::value));
BOOST_STATIC_ASSERT((is_exclusive_lock<synchro::thread_recursive_mutex>::value));
BOOST_STATIC_ASSERT((!is_sharable_lock<synchro::thread_recursive_mutex>::value));
BOOST_STATIC_ASSERT((!is_upgradable_lock<synchro::thread_recursive_mutex>::value));
BOOST_STATIC_ASSERT((is_mono_threaded<synchro::thread_recursive_mutex>::value));
BOOST_STATIC_ASSERT((is_multi_threaded<synchro::thread_recursive_mutex>::value));
BOOST_STATIC_ASSERT((!is_multi_process<synchro::thread_recursive_mutex>::value));
BOOST_STATIC_ASSERT((is_recursive_lock<synchro::thread_recursive_mutex>::value));
BOOST_STATIC_ASSERT((!has_timed_interface<synchro::thread_recursive_mutex>::value));

BOOST_STATIC_ASSERT((is_same<scope_tag<synchro::thread_recursive_timed_mutex>::type, multi_threaded_tag>::value));
BOOST_STATIC_ASSERT((is_same<category_tag<synchro::thread_recursive_timed_mutex>::type, exclusive_lock_tag>::value));

BOOST_STATIC_ASSERT((is_same<scope_tag<synchro::thread_shared_mutex>::type, multi_threaded_tag>::value));
BOOST_STATIC_ASSERT((is_same<category_tag<synchro::thread_shared_mutex>::type, upgradable_lock_tag>::value));
BOOST_STATIC_ASSERT((is_exclusive_lock<synchro::thread_shared_mutex>::value));
BOOST_STATIC_ASSERT((is_sharable_lock<synchro::thread_shared_mutex>::value));
BOOST_STATIC_ASSERT((is_upgradable_lock<synchro::thread_shared_mutex>::value));
BOOST_STATIC_ASSERT((is_mono_threaded<synchro::thread_shared_mutex>::value));
BOOST_STATIC_ASSERT((is_multi_threaded<synchro::thread_shared_mutex>::value));
BOOST_STATIC_ASSERT((!is_multi_process<synchro::thread_shared_mutex>::value));

BOOST_STATIC_ASSERT((is_same<scope_tag<synchro::interprocess_mutex>::type, multi_process_tag>::value));
BOOST_STATIC_ASSERT((is_same<category_tag<synchro::interprocess_mutex>::type, exclusive_lock_tag>::value));
BOOST_STATIC_ASSERT((is_exclusive_lock<synchro::interprocess_mutex>::value));
BOOST_STATIC_ASSERT((!is_sharable_lock<synchro::interprocess_mutex>::value));
BOOST_STATIC_ASSERT((!is_upgradable_lock<synchro::interprocess_mutex>::value));
BOOST_STATIC_ASSERT((is_mono_threaded<synchro::interprocess_mutex>::value));
BOOST_STATIC_ASSERT((is_multi_threaded<synchro::interprocess_mutex>::value));
BOOST_STATIC_ASSERT((is_multi_process<synchro::interprocess_mutex>::value));

BOOST_STATIC_ASSERT((is_same<scope_tag<synchro::interprocess_recursive_mutex>::type, multi_process_tag>::value));
BOOST_STATIC_ASSERT((is_same<category_tag<synchro::interprocess_recursive_mutex>::type, exclusive_lock_tag>::value));

BOOST_STATIC_ASSERT((is_same<scope_tag<synchro::named_mutex>::type, multi_process_tag>::value));
BOOST_STATIC_ASSERT((is_same<category_tag<synchro::named_mutex>::type, exclusive_lock_tag>::value));


BOOST_STATIC_ASSERT((is_same<scope_tag<boost::synchro::interprocess_upgradable_mutex>::type, multi_process_tag>::value));
BOOST_STATIC_ASSERT((is_same<category_tag<boost::synchro::interprocess_upgradable_mutex>::type, upgradable_lock_tag>::value));

BOOST_STATIC_ASSERT((is_same<scope_tag<boost::synchro::interprocess_named_upgradable_mutex>::type, multi_process_tag>::value));
BOOST_STATIC_ASSERT((is_same<category_tag<boost::synchro::interprocess_named_upgradable_mutex>::type, upgradable_lock_tag>::value));


#if 0
BOOST_STATIC_ASSERT((is_same<scope_tag<synchro::named_upgradable_mutex>::type, multi_process_tag>::value));
BOOST_STATIC_ASSERT((is_same<category_tag<synchro::named_upgradable_mutex>::type, upgradable_lock_tag>::value));
BOOST_STATIC_ASSERT((is_same<scope_tag<synchro::interprocess_upgradable_mutex>::type, multi_process_tag>::value));
BOOST_STATIC_ASSERT((is_same<category_tag<synchro::interprocess_upgradable_mutex>::type, upgradable_lock_tag>::value));
#endif


//____________________________________________________________________________//


//____________________________________________________________________________//

// EOF
