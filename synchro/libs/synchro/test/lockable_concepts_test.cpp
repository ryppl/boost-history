/* Copyright 2008-2009 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */
 
#include <boost/synchro/lockable_traits.hpp>
#include <boost/synchro/lockable_concepts.hpp>
#include <boost/synchro/lockable_adapter.hpp>
#include <boost/synchro/poly/lock_adapter.hpp>

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

#define BOOST_TEST_MODULE sync_lockable_concepts_test 

using namespace boost;
using namespace boost::synchro;

BOOST_CONCEPT_ASSERT((LockableConcept<null_mutex>));    
BOOST_CONCEPT_ASSERT((TimedLockableConcept<null_mutex>));    
BOOST_CONCEPT_ASSERT((ShareLockableConcept<null_mutex>));    
BOOST_CONCEPT_ASSERT((UpgradeLockableConcept<null_mutex>));    

BOOST_CONCEPT_ASSERT((LockableConcept<exclusive_lockable_adapter<null_mutex> >));    
BOOST_CONCEPT_ASSERT((TimedLockableConcept<timed_lockable_adapter<null_mutex> >));    
BOOST_CONCEPT_ASSERT((ShareLockableConcept<shared_lockable_adapter<null_mutex> >));    
BOOST_CONCEPT_ASSERT((UpgradeLockableConcept<upgrade_lockable_adapter<null_mutex> >));    

BOOST_CONCEPT_ASSERT((LockableConcept<poly::exclusive_lock_adapter<null_mutex> >));    
BOOST_CONCEPT_ASSERT((TimedLockableConcept<poly::timed_lock_adapter<null_mutex> >));    
BOOST_CONCEPT_ASSERT((ShareLockableConcept<poly::sharable_lock_adapter<null_mutex> >));    
BOOST_CONCEPT_ASSERT((UpgradeLockableConcept<poly::upgradable_lock_adapter<null_mutex> >));    

BOOST_CONCEPT_ASSERT((LockableConcept<thread_mutex>));    


BOOST_CONCEPT_ASSERT((LockableConcept<thread_timed_mutex>));    
BOOST_CONCEPT_ASSERT((TimedLockableConcept<thread_timed_mutex>));    

BOOST_CONCEPT_ASSERT((LockableConcept<thread_recursive_mutex>));    

BOOST_CONCEPT_ASSERT((LockableConcept<thread_recursive_timed_mutex>));    
BOOST_CONCEPT_ASSERT((TimedLockableConcept<thread_recursive_timed_mutex>));    

BOOST_CONCEPT_ASSERT((LockableConcept<thread_shared_mutex>));    
BOOST_CONCEPT_ASSERT((TimedLockableConcept<thread_shared_mutex>));    
BOOST_CONCEPT_ASSERT((ShareLockableConcept<thread_shared_mutex>));    
BOOST_CONCEPT_ASSERT((UpgradeLockableConcept<thread_shared_mutex>));   

BOOST_CONCEPT_ASSERT((LockableConcept<interprocess_mutex>));    
BOOST_CONCEPT_ASSERT((TimedLockableConcept<synchro::interprocess_mutex>));    

BOOST_CONCEPT_ASSERT((LockableConcept<synchro::interprocess_recursive_mutex>));    
BOOST_CONCEPT_ASSERT((TimedLockableConcept<synchro::interprocess_recursive_mutex>));    

BOOST_CONCEPT_ASSERT((LockableConcept<synchro::named_mutex>));    
BOOST_CONCEPT_ASSERT((TimedLockableConcept<synchro::named_mutex>));    

BOOST_CONCEPT_ASSERT((LockableConcept<synchro::interprocess_upgradable_mutex>));    
BOOST_CONCEPT_ASSERT((TimedLockableConcept<synchro::interprocess_upgradable_mutex>));    
BOOST_CONCEPT_ASSERT((ShareLockableConcept<synchro::interprocess_upgradable_mutex>));    
BOOST_CONCEPT_ASSERT((UpgradeLockableConcept<synchro::interprocess_upgradable_mutex>));    

BOOST_CONCEPT_ASSERT((LockableConcept<synchro::interprocess_named_upgradable_mutex>));    
BOOST_CONCEPT_ASSERT((TimedLockableConcept<synchro::interprocess_named_upgradable_mutex>));    
BOOST_CONCEPT_ASSERT((ShareLockableConcept<synchro::interprocess_named_upgradable_mutex>));    
BOOST_CONCEPT_ASSERT((UpgradeLockableConcept<boost::synchro::interprocess_named_upgradable_mutex>));    

//____________________________________________________________________________//


//____________________________________________________________________________//

// EOF
