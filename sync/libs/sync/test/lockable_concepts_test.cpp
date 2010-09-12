/* Copyright 2008-2009 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */
 
#include <boost/sync/lockable_traits.hpp>
#include <boost/sync/lockable_concepts.hpp>
//~ #include <boost/sync/lockable_adapter.hpp>
//~ #include <boost/sync/poly/lock_adapter.hpp>

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

#define BOOST_TEST_MODULE sync_lockable_concepts_test 

using namespace boost;
using namespace boost::sync;
using namespace boost::interprocess;

BOOST_CONCEPT_ASSERT((BasicLockableConcept<sync::null_mutex>));    
BOOST_CONCEPT_ASSERT((TryLockableConcept<sync::null_mutex>));    
BOOST_CONCEPT_ASSERT((TimedLockableConcept<sync::null_mutex>));    
BOOST_CONCEPT_ASSERT((ShareLockableConcept<sync::null_mutex>));    
BOOST_CONCEPT_ASSERT((UpgradeLockableConcept<sync::null_mutex>));    

//~ BOOST_CONCEPT_ASSERT((TryLockableConcept<exclusive_lockable_adapter<null_mutex> >));    
//~ BOOST_CONCEPT_ASSERT((TimedLockableConcept<timed_lockable_adapter<null_mutex> >));    
//~ BOOST_CONCEPT_ASSERT((ShareLockableConcept<shared_lockable_adapter<null_mutex> >));    
//~ BOOST_CONCEPT_ASSERT((UpgradeLockableConcept<upgrade_lockable_adapter<null_mutex> >));    

//~ BOOST_CONCEPT_ASSERT((TryLockableConcept<poly::exclusive_lock_adapter<null_mutex> >));    
//~ BOOST_CONCEPT_ASSERT((TimedLockableConcept<poly::timed_lock_adapter<null_mutex> >));    
//~ BOOST_CONCEPT_ASSERT((ShareLockableConcept<poly::sharable_lock_adapter<null_mutex> >));    
//~ BOOST_CONCEPT_ASSERT((UpgradeLockableConcept<poly::upgradable_lock_adapter<null_mutex> >));    

BOOST_CONCEPT_ASSERT((BasicLockableConcept<boost::mutex>));    
BOOST_CONCEPT_ASSERT((TryLockableConcept<boost::mutex>));    

BOOST_CONCEPT_ASSERT((BasicLockableConcept<boost::timed_mutex>));    
BOOST_CONCEPT_ASSERT((TryLockableConcept<boost::timed_mutex>));    
BOOST_CONCEPT_ASSERT((TimedLockableConcept<boost::timed_mutex>));    

BOOST_CONCEPT_ASSERT((BasicLockableConcept<boost::recursive_mutex>));    
BOOST_CONCEPT_ASSERT((TryLockableConcept<boost::recursive_mutex>));    

BOOST_CONCEPT_ASSERT((BasicLockableConcept<boost::recursive_timed_mutex>));    
BOOST_CONCEPT_ASSERT((TryLockableConcept<boost::recursive_timed_mutex>));    
BOOST_CONCEPT_ASSERT((TimedLockableConcept<boost::recursive_timed_mutex>));    

BOOST_CONCEPT_ASSERT((BasicLockableConcept<boost::shared_mutex>));    
BOOST_CONCEPT_ASSERT((TryLockableConcept<boost::shared_mutex>));    
BOOST_CONCEPT_ASSERT((TimedLockableConcept<boost::shared_mutex>));    
BOOST_CONCEPT_ASSERT((ShareLockableConcept<boost::shared_mutex>));    
BOOST_CONCEPT_ASSERT((UpgradeLockableConcept<boost::shared_mutex>));   

BOOST_CONCEPT_ASSERT((BasicLockableConcept<interprocess_mutex>));    
BOOST_CONCEPT_ASSERT((TryLockableConcept<interprocess_mutex>));    
BOOST_CONCEPT_ASSERT((TimedLockableConcept<interprocess_mutex>));    

BOOST_CONCEPT_ASSERT((BasicLockableConcept<interprocess_recursive_mutex>));    
BOOST_CONCEPT_ASSERT((TryLockableConcept<interprocess_recursive_mutex>));    
BOOST_CONCEPT_ASSERT((TimedLockableConcept<interprocess_recursive_mutex>));    

BOOST_CONCEPT_ASSERT((BasicLockableConcept<named_mutex>));    
BOOST_CONCEPT_ASSERT((TryLockableConcept<named_mutex>));    
BOOST_CONCEPT_ASSERT((TimedLockableConcept<named_mutex>));    

BOOST_CONCEPT_ASSERT((TryLockableConcept<interprocess_upgradable_mutex>));    
BOOST_CONCEPT_ASSERT((TimedLockableConcept<interprocess_upgradable_mutex>));    
BOOST_CONCEPT_ASSERT((ShareLockableConcept<interprocess_upgradable_mutex>));    
BOOST_CONCEPT_ASSERT((UpgradeLockableConcept<interprocess_upgradable_mutex>));    

BOOST_CONCEPT_ASSERT((TryLockableConcept<named_upgradable_mutex>));    
BOOST_CONCEPT_ASSERT((TimedLockableConcept<named_upgradable_mutex>));    
BOOST_CONCEPT_ASSERT((ShareLockableConcept<named_upgradable_mutex>));    
BOOST_CONCEPT_ASSERT((UpgradeLockableConcept<named_upgradable_mutex>));    

