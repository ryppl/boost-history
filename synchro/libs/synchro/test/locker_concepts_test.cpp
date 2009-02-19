/* Copyright 2008-2009 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */
 
#include <boost/synchro/lockable_traits.hpp>
#include <boost/synchro/lockable_concepts.hpp>
#include <boost/synchro/locker_concepts.hpp>
#include <boost/synchro/lockable_adapter.hpp>
#include <boost/synchro/poly/lock_adapter.hpp>

#include <boost/synchro/null_mutex.hpp>
#include <boost/synchro/null_condition.hpp>

#include <boost/synchro/thread/mutex.hpp>
#include <boost/synchro/thread/recursive_mutex.hpp>
#include <boost/synchro/thread/shared_mutex.hpp>
#include <boost/synchro/thread/locks.hpp>

#include <boost/synchro/process/mutex.hpp>
#include <boost/synchro/process/recursive_mutex.hpp>
#include <boost/synchro/process/upgradable_mutex.hpp>
#include <boost/synchro/process/named_mutex.hpp>
#include <boost/synchro/process/named_recursive_mutex.hpp>
#include <boost/synchro/process/named_upgradable_mutex.hpp>
#include <boost/synchro/process/locks.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/static_assert.hpp>

#define BOOST_TEST_MODULE sync_locker_concepts_test 

using namespace boost;
using namespace boost::synchro;

BOOST_CONCEPT_ASSERT((LockerConcept<unique_locker<thread_mutex> >));    

BOOST_CONCEPT_ASSERT((LockerConcept<unique_locker<thread_timed_mutex> >));    
BOOST_CONCEPT_ASSERT((TimedLockerConcept<unique_locker<thread_timed_mutex> >));    
BOOST_CONCEPT_ASSERT((UniqueLockerConcept<unique_locker<thread_timed_mutex> >));    

BOOST_CONCEPT_ASSERT((LockerConcept<shared_locker<thread_shared_mutex> >));    
BOOST_CONCEPT_ASSERT((TimedLockerConcept<shared_locker<thread_shared_mutex> >));    
BOOST_CONCEPT_ASSERT((UniqueLockerConcept<shared_locker<thread_shared_mutex> >));    
BOOST_CONCEPT_ASSERT((SharedLockerConcept<shared_locker<thread_shared_mutex> >));    

BOOST_CONCEPT_ASSERT((LockerConcept<upgrade_locker<thread_shared_mutex> >));    
// FAILS BOOST_CONCEPT_ASSERT((TimedLockerConcept<upgrade_locker<thread_shared_mutex> >));    
BOOST_CONCEPT_ASSERT((UniqueLockerConcept<upgrade_locker<thread_shared_mutex> >));    
BOOST_CONCEPT_ASSERT((SharedLockerConcept<upgrade_locker<thread_shared_mutex> >));    
BOOST_CONCEPT_ASSERT((UpgradeLockerConcept<upgrade_locker<thread_shared_mutex> >));    

BOOST_CONCEPT_ASSERT((LockerConcept<unique_locker<interprocess_mutex> >));    
BOOST_CONCEPT_ASSERT((TimedLockerConcept<unique_locker<interprocess_mutex> >));    
BOOST_CONCEPT_ASSERT((UniqueLockerConcept<unique_locker<interprocess_mutex> >));    

BOOST_CONCEPT_ASSERT((LockerConcept<shared_locker<interprocess_upgradable_mutex> >));    
BOOST_CONCEPT_ASSERT((TimedLockerConcept<shared_locker<interprocess_upgradable_mutex> >));    
BOOST_CONCEPT_ASSERT((UniqueLockerConcept<shared_locker<interprocess_upgradable_mutex> >));    
BOOST_CONCEPT_ASSERT((SharedLockerConcept<shared_locker<interprocess_upgradable_mutex> >));    
BOOST_CONCEPT_ASSERT((UpgradeLockerConcept<shared_locker<interprocess_upgradable_mutex> >));    

BOOST_CONCEPT_ASSERT((LockerConcept<upgrade_locker<interprocess_upgradable_mutex> >));    
BOOST_CONCEPT_ASSERT((TimedLockerConcept<upgrade_locker<interprocess_upgradable_mutex> >));    
BOOST_CONCEPT_ASSERT((UniqueLockerConcept<upgrade_locker<interprocess_upgradable_mutex> >));    
BOOST_CONCEPT_ASSERT((SharedLockerConcept<upgrade_locker<interprocess_upgradable_mutex> >));    
BOOST_CONCEPT_ASSERT((UpgradeLockerConcept<upgrade_locker<interprocess_upgradable_mutex> >));    
#if 0

BOOST_CONCEPT_ASSERT((LockableConcept<exclusive_lockable_adapter<null_mutex> >));    
BOOST_CONCEPT_ASSERT((LockableConcept<poly::exclusive_lock_adapter<null_mutex> >));    
BOOST_CONCEPT_ASSERT((LockableConcept<thread_recursive_mutex>));    
BOOST_CONCEPT_ASSERT((LockableConcept<thread_recursive_timed_mutex>));    
BOOST_CONCEPT_ASSERT((LockableConcept<synchro::interprocess_recursive_mutex>));    
BOOST_CONCEPT_ASSERT((LockableConcept<synchro::named_mutex>));    
BOOST_CONCEPT_ASSERT((LockableConcept<synchro::interprocess_named_upgradable_mutex>));    
#endif
//____________________________________________________________________________//


//____________________________________________________________________________//

// EOF
