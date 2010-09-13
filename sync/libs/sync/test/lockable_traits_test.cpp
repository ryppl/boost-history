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


BOOST_STATIC_ASSERT((is_same<best_condition< ::boost::sync::null_mutex>::type, null_condition>::value));
BOOST_STATIC_ASSERT((is_same<scope< ::boost::sync::null_mutex>::type, mono_threaded_tag>::value));
BOOST_STATIC_ASSERT((is_same<category< ::boost::sync::null_mutex>::type, upgradable_timed_lockable_tag>::value));
BOOST_STATIC_ASSERT((is_same<reentrancy< ::boost::sync::null_mutex>::type, recursive_tag>::value));
BOOST_STATIC_ASSERT((is_exclusive< ::boost::sync::null_mutex>::value));
BOOST_STATIC_ASSERT((is_sharable< ::boost::sync::null_mutex>::value));
BOOST_STATIC_ASSERT((is_upgradable< ::boost::sync::null_mutex>::value));
BOOST_STATIC_ASSERT((is_mono_threaded< ::boost::sync::null_mutex>::value));
BOOST_STATIC_ASSERT((!is_multi_threaded< ::boost::sync::null_mutex>::value));
BOOST_STATIC_ASSERT((!is_multi_process< ::boost::sync::null_mutex>::value));
BOOST_STATIC_ASSERT((is_recursive< ::boost::sync::null_mutex>::value));
BOOST_STATIC_ASSERT((!is_owned< ::boost::sync::null_mutex>::value));

//~ BOOST_STATIC_ASSERT((is_same<best_condition<mutex>::type, boost::condition_variable>::value));
BOOST_STATIC_ASSERT((is_same<scope<mutex>::type, multi_threaded_tag>::value));
BOOST_STATIC_ASSERT((is_same<category<mutex>::type, exclusive_try_lockable_tag>::value));
BOOST_STATIC_ASSERT((!is_recursive<mutex>::value));
BOOST_STATIC_ASSERT((is_exclusive<mutex>::value));
BOOST_STATIC_ASSERT((!is_sharable<mutex>::value));
BOOST_STATIC_ASSERT((!is_upgradable<mutex>::value));
BOOST_STATIC_ASSERT((is_mono_threaded<mutex>::value));
BOOST_STATIC_ASSERT((is_multi_threaded<mutex>::value));
BOOST_STATIC_ASSERT((!is_multi_process<mutex>::value));
BOOST_STATIC_ASSERT((!is_recursive<mutex>::value));
BOOST_STATIC_ASSERT((is_owned<mutex>::value));


BOOST_STATIC_ASSERT((is_same<scope<timed_mutex>::type, multi_threaded_tag>::value));
BOOST_STATIC_ASSERT((is_same<category<timed_mutex>::type, exclusive_timed_lockable_tag>::value));
BOOST_STATIC_ASSERT((is_same<reentrancy<timed_mutex>::type, non_recursive_tag>::value));
BOOST_STATIC_ASSERT((is_exclusive<timed_mutex>::value));
BOOST_STATIC_ASSERT((!is_sharable<timed_mutex>::value));
BOOST_STATIC_ASSERT((!is_upgradable<timed_mutex>::value));
BOOST_STATIC_ASSERT((is_mono_threaded<timed_mutex>::value));
BOOST_STATIC_ASSERT((is_multi_threaded<timed_mutex>::value));
BOOST_STATIC_ASSERT((!is_multi_process<timed_mutex>::value));
BOOST_STATIC_ASSERT((!is_recursive<timed_mutex>::value));
BOOST_STATIC_ASSERT((is_owned<timed_mutex>::value));

BOOST_STATIC_ASSERT((is_same<scope<recursive_mutex>::type, multi_threaded_tag>::value));
BOOST_STATIC_ASSERT((is_same<category<recursive_mutex>::type, exclusive_try_lockable_tag>::value));
BOOST_STATIC_ASSERT((is_same<reentrancy<recursive_mutex>::type, recursive_tag>::value));
BOOST_STATIC_ASSERT((is_exclusive<recursive_mutex>::value));
BOOST_STATIC_ASSERT((!is_sharable<recursive_mutex>::value));
BOOST_STATIC_ASSERT((!is_upgradable<recursive_mutex>::value));
BOOST_STATIC_ASSERT((is_mono_threaded<recursive_mutex>::value));
BOOST_STATIC_ASSERT((is_multi_threaded<recursive_mutex>::value));
BOOST_STATIC_ASSERT((!is_multi_process<recursive_mutex>::value));
BOOST_STATIC_ASSERT((is_recursive<recursive_mutex>::value));
BOOST_STATIC_ASSERT((is_owned<recursive_mutex>::value));

BOOST_STATIC_ASSERT((is_same<scope<recursive_timed_mutex>::type, multi_threaded_tag>::value));
BOOST_STATIC_ASSERT((is_same<category<recursive_timed_mutex>::type, exclusive_timed_lockable_tag>::value));

BOOST_STATIC_ASSERT((is_same<scope<shared_mutex>::type, multi_threaded_tag>::value));
BOOST_STATIC_ASSERT((is_same<category<shared_mutex>::type, upgradable_basic_lockable_tag>::value));
BOOST_STATIC_ASSERT((is_exclusive<shared_mutex>::value));
BOOST_STATIC_ASSERT((is_sharable<shared_mutex>::value));
BOOST_STATIC_ASSERT((is_upgradable<shared_mutex>::value));
BOOST_STATIC_ASSERT((is_mono_threaded<shared_mutex>::value));
BOOST_STATIC_ASSERT((is_multi_threaded<shared_mutex>::value));
BOOST_STATIC_ASSERT((!is_multi_process<shared_mutex>::value));

BOOST_STATIC_ASSERT((is_same<scope<interprocess_mutex>::type, multi_process_tag>::value));
BOOST_STATIC_ASSERT((is_same<category<interprocess_mutex>::type, exclusive_timed_lockable_tag>::value));
BOOST_STATIC_ASSERT((is_exclusive<interprocess_mutex>::value));
BOOST_STATIC_ASSERT((!is_sharable<interprocess_mutex>::value));
BOOST_STATIC_ASSERT((!is_upgradable<interprocess_mutex>::value));
BOOST_STATIC_ASSERT((is_mono_threaded<interprocess_mutex>::value));
BOOST_STATIC_ASSERT((is_multi_threaded<interprocess_mutex>::value));
BOOST_STATIC_ASSERT((is_multi_process<interprocess_mutex>::value));

BOOST_STATIC_ASSERT((is_same<scope<interprocess_recursive_mutex>::type, multi_process_tag>::value));
BOOST_STATIC_ASSERT((is_same<category<interprocess_recursive_mutex>::type, exclusive_timed_lockable_tag>::value));

BOOST_STATIC_ASSERT((is_same<scope<named_mutex>::type, multi_process_tag>::value));
BOOST_STATIC_ASSERT((is_same<category<named_mutex>::type, exclusive_timed_lockable_tag>::value));


BOOST_STATIC_ASSERT((is_same<scope<interprocess_upgradable_mutex>::type, multi_process_tag>::value));
BOOST_STATIC_ASSERT((is_same<category<interprocess_upgradable_mutex>::type, upgradable_timed_lockable_tag>::value));

BOOST_STATIC_ASSERT((is_same<scope<named_upgradable_mutex>::type, multi_process_tag>::value));
BOOST_STATIC_ASSERT((is_same<category<named_upgradable_mutex>::type, upgradable_timed_lockable_tag>::value));




//____________________________________________________________________________//


//____________________________________________________________________________//

// EOF
