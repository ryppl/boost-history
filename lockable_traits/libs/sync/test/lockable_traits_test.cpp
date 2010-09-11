/* Copyright 2008-2009 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */




#include <boost/sync/lockable_traits.hpp>
#if 0
#include <boost/sync/null_mutex.hpp>
#include <boost/sync/null_condition.hpp>

#include <boost/sync/thread/mutex.hpp>
#include <boost/sync/thread/recursive_mutex.hpp>
#include <boost/sync/thread/shared_mutex.hpp>

#include <boost/sync/process/mutex.hpp>
#include <boost/sync/process/recursive_mutex.hpp>
#include <boost/sync/process/upgradable_mutex.hpp>
#include <boost/sync/process/named_mutex.hpp>
#include <boost/sync/process/named_recursive_mutex.hpp>
#include <boost/sync/process/named_upgradable_mutex.hpp>
#else
#include <boost/sync/null/null_mutex.hpp>
#include <boost/sync/null/null_condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_recursive_mutex.hpp>
#include <boost/interprocess/sync/interprocess_upgradable_mutex.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/named_recursive_mutex.hpp>
#include <boost/interprocess/sync/named_upgradable_mutex.hpp>
#endif
#include <boost/type_traits/is_same.hpp>
#include <boost/static_assert.hpp>

#define BOOST_TEST_MODULE sync_lock_traits_test

using namespace boost;
using namespace boost::sync;
using namespace boost::interprocess;


BOOST_STATIC_ASSERT((is_same<best_condition<sync::null_mutex>::type, null_condition>::value));
BOOST_STATIC_ASSERT((is_same<scope<sync::null_mutex>::type, mono_threaded_tag>::value));
BOOST_STATIC_ASSERT((is_same<category<sync::null_mutex>::type, upgradable_lock_tag>::value));
BOOST_STATIC_ASSERT((is_same<reentrancy<sync::null_mutex>::type, recursive_tag>::value));
BOOST_STATIC_ASSERT((is_same<kind<sync::null_mutex>::type, timed_lockable_tag>::value));
BOOST_STATIC_ASSERT((is_exclusive_lock<sync::null_mutex>::value));
BOOST_STATIC_ASSERT((is_sharable_lock<sync::null_mutex>::value));
BOOST_STATIC_ASSERT((is_upgradable_lock<sync::null_mutex>::value));
BOOST_STATIC_ASSERT((is_mono_threaded<sync::null_mutex>::value));
BOOST_STATIC_ASSERT((!is_multi_threaded<sync::null_mutex>::value));
BOOST_STATIC_ASSERT((!is_multi_process<sync::null_mutex>::value));
BOOST_STATIC_ASSERT((is_recursive_lock<sync::null_mutex>::value));
BOOST_STATIC_ASSERT((is_timed_lockable<sync::null_mutex>::value));

//~ BOOST_STATIC_ASSERT((is_same<best_condition<mutex>::type, boost::condition_variable>::value));
BOOST_STATIC_ASSERT((is_same<scope<mutex>::type, multi_threaded_tag>::value));
BOOST_STATIC_ASSERT((is_same<category<mutex>::type, exclusive_lock_tag>::value));
BOOST_STATIC_ASSERT((!is_recursive_lock<mutex>::value));
BOOST_STATIC_ASSERT((is_same<kind<mutex>::type, try_lockable_tag>::value));
BOOST_STATIC_ASSERT((is_exclusive_lock<mutex>::value));
BOOST_STATIC_ASSERT((!is_sharable_lock<mutex>::value));
BOOST_STATIC_ASSERT((!is_upgradable_lock<mutex>::value));
BOOST_STATIC_ASSERT((is_mono_threaded<mutex>::value));
BOOST_STATIC_ASSERT((is_multi_threaded<mutex>::value));
BOOST_STATIC_ASSERT((!is_multi_process<mutex>::value));
BOOST_STATIC_ASSERT((!is_recursive_lock<mutex>::value));
BOOST_STATIC_ASSERT((!is_timed_lockable<mutex>::value));


BOOST_STATIC_ASSERT((is_same<scope<timed_mutex>::type, multi_threaded_tag>::value));
BOOST_STATIC_ASSERT((is_same<category<timed_mutex>::type, exclusive_lock_tag>::value));
BOOST_STATIC_ASSERT((is_same<reentrancy<timed_mutex>::type, non_recursive_tag>::value));
BOOST_STATIC_ASSERT((is_same<kind<timed_mutex>::type, timed_lockable_tag>::value));
BOOST_STATIC_ASSERT((is_exclusive_lock<timed_mutex>::value));
BOOST_STATIC_ASSERT((!is_sharable_lock<timed_mutex>::value));
BOOST_STATIC_ASSERT((!is_upgradable_lock<timed_mutex>::value));
BOOST_STATIC_ASSERT((is_mono_threaded<timed_mutex>::value));
BOOST_STATIC_ASSERT((is_multi_threaded<timed_mutex>::value));
BOOST_STATIC_ASSERT((!is_multi_process<timed_mutex>::value));
BOOST_STATIC_ASSERT((!is_recursive_lock<timed_mutex>::value));
BOOST_STATIC_ASSERT((is_timed_lockable<timed_mutex>::value));

BOOST_STATIC_ASSERT((is_same<scope<recursive_mutex>::type, multi_threaded_tag>::value));
BOOST_STATIC_ASSERT((is_same<category<recursive_mutex>::type, exclusive_lock_tag>::value));
BOOST_STATIC_ASSERT((is_same<reentrancy<recursive_mutex>::type, recursive_tag>::value));
BOOST_STATIC_ASSERT((is_same<kind<recursive_mutex>::type, try_lockable_tag>::value));
BOOST_STATIC_ASSERT((is_exclusive_lock<recursive_mutex>::value));
BOOST_STATIC_ASSERT((!is_sharable_lock<recursive_mutex>::value));
BOOST_STATIC_ASSERT((!is_upgradable_lock<recursive_mutex>::value));
BOOST_STATIC_ASSERT((is_mono_threaded<recursive_mutex>::value));
BOOST_STATIC_ASSERT((is_multi_threaded<recursive_mutex>::value));
BOOST_STATIC_ASSERT((!is_multi_process<recursive_mutex>::value));
BOOST_STATIC_ASSERT((is_recursive_lock<recursive_mutex>::value));
BOOST_STATIC_ASSERT((!is_timed_lockable<recursive_mutex>::value));

BOOST_STATIC_ASSERT((is_same<scope<recursive_timed_mutex>::type, multi_threaded_tag>::value));
BOOST_STATIC_ASSERT((is_same<category<recursive_timed_mutex>::type, exclusive_lock_tag>::value));

BOOST_STATIC_ASSERT((is_same<scope<shared_mutex>::type, multi_threaded_tag>::value));
BOOST_STATIC_ASSERT((is_same<category<shared_mutex>::type, upgradable_lock_tag>::value));
BOOST_STATIC_ASSERT((is_exclusive_lock<shared_mutex>::value));
BOOST_STATIC_ASSERT((is_sharable_lock<shared_mutex>::value));
BOOST_STATIC_ASSERT((is_upgradable_lock<shared_mutex>::value));
BOOST_STATIC_ASSERT((is_mono_threaded<shared_mutex>::value));
BOOST_STATIC_ASSERT((is_multi_threaded<shared_mutex>::value));
BOOST_STATIC_ASSERT((!is_multi_process<shared_mutex>::value));

BOOST_STATIC_ASSERT((is_same<scope<interprocess_mutex>::type, multi_process_tag>::value));
BOOST_STATIC_ASSERT((is_same<category<interprocess_mutex>::type, exclusive_lock_tag>::value));
BOOST_STATIC_ASSERT((is_exclusive_lock<interprocess_mutex>::value));
BOOST_STATIC_ASSERT((!is_sharable_lock<interprocess_mutex>::value));
BOOST_STATIC_ASSERT((!is_upgradable_lock<interprocess_mutex>::value));
BOOST_STATIC_ASSERT((is_mono_threaded<interprocess_mutex>::value));
BOOST_STATIC_ASSERT((is_multi_threaded<interprocess_mutex>::value));
BOOST_STATIC_ASSERT((is_multi_process<interprocess_mutex>::value));

BOOST_STATIC_ASSERT((is_same<scope<interprocess_recursive_mutex>::type, multi_process_tag>::value));
BOOST_STATIC_ASSERT((is_same<category<interprocess_recursive_mutex>::type, exclusive_lock_tag>::value));

BOOST_STATIC_ASSERT((is_same<scope<named_mutex>::type, multi_process_tag>::value));
BOOST_STATIC_ASSERT((is_same<category<named_mutex>::type, exclusive_lock_tag>::value));


BOOST_STATIC_ASSERT((is_same<scope<interprocess_upgradable_mutex>::type, multi_process_tag>::value));
BOOST_STATIC_ASSERT((is_same<category<interprocess_upgradable_mutex>::type, upgradable_lock_tag>::value));

BOOST_STATIC_ASSERT((is_same<scope<named_upgradable_mutex>::type, multi_process_tag>::value));
BOOST_STATIC_ASSERT((is_same<category<named_upgradable_mutex>::type, upgradable_lock_tag>::value));


#if 0
BOOST_STATIC_ASSERT((is_same<scope<named_upgradable_mutex>::type, multi_process_tag>::value));
BOOST_STATIC_ASSERT((is_same<category<named_upgradable_mutex>::type, upgradable_lock_tag>::value));
BOOST_STATIC_ASSERT((is_same<scope<interprocess_upgradable_mutex>::type, multi_process_tag>::value));
BOOST_STATIC_ASSERT((is_same<category<interprocess_upgradable_mutex>::type, upgradable_lock_tag>::value));
#endif


//____________________________________________________________________________//


//____________________________________________________________________________//

// EOF
