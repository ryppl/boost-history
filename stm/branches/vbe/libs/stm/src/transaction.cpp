//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Justin E. Gottchlich 2009.
// (C) Copyright Vicente J. Botet Escriba 2009.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/stm for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/stm/transaction.hpp>
#include <boost/stm/latm.hpp>
#include <boost/stm/non_tx/detail/cache_map.hpp>
#include <boost/stm/contention_managers/contention_manager.hpp>
#include <iostream>

using namespace std;
//using namespace boost::stm;
namespace boost { 
boost::mutex log_mutex;
namespace stm {
namespace non_tx {
    std::map<void*, base_transaction_object*> detail::cache_map::map_;
#ifndef BOOST_STM_USE_BOOST_MUTEX
    Mutex detail::cache_map::mtx_=PTHREAD_MUTEX_INITIALIZER;
#else    
    Mutex detail::cache_map::mtx_;
#endif
}

#ifndef BOOST_USES_STATIC_TSS
synchro::implicit_thread_specific_ptr<transaction::transaction_tss_storage> transaction::transaction_tss_storage_;
#endif
///////////////////////////////////////////////////////////////////////////////
// Static initialization
///////////////////////////////////////////////////////////////////////////////
transaction::in_flight_trans_cont transaction::transactionsInFlight_;
//latm::mutex_set transaction::latmLockedLocks_;
//latm::mutex_thread_id_set_map transaction::latmLockedLocksAndThreadIdsMap_;
//latm::mutex_thread_id_map transaction::latmLockedLocksOfThreadMap_;
//latm::mutex_set transaction::tmConflictingLocks_;
transaction::DeletionBuffer transaction::deletionBuffer_;

//////////////
// LATM
/////////////
namespace latm {
#if defined(BOOST_STM_LATM_STATIC_FULL_MODE)
latm_mode mode::instance_;
#elif defined(BOOST_STM_LATM_STATIC_TM_MODE)
latm_mode mode::instance_;
#elif defined(BOOST_STM_LATM_STATIC_TX_MODE)
latm_mode mode::instance_;
#elif defined(BOOST_STM_LATM_DYNAMIC_MODE)
full_mode<abstract_mode> mode::full_;
tm_mode<abstract_mode> mode::tm_;
tx_mode<abstract_mode> mode::tx_;
latm_mode* mode::instance_=&mode::full_;
#else
latm_mode mode::instance_;
#endif
}


clock_t transaction::global_clock_ = 0;
size_t transaction::stalls_ = 0;

bool transaction::dynamicPriorityAssignment_ = false;
bool transaction::direct_updating_ = false;
bool transaction::directLateWriteReadConflict_ = false;
bool transaction::usingMoveSemantics_ = false;

//pthread_mutexattr_t transaction::transactionMutexAttribute_;

Mutex transaction::transactionsInFlightMutex_;
Mutex transaction::transactionMutex_;
Mutex transaction::deletionBufferMutex_;
//Mutex transaction::latmMutex_;

std::ofstream transaction::logFile_;

#ifndef BOOST_STM_USE_BOOST_MUTEX
pthread_mutex_t base_memory_manager::transactionObjectMutex_ = PTHREAD_MUTEX_INITIALIZER;
#else
boost::mutex base_memory_manager::transactionObjectMutex_;
#endif
boost::stm::MemoryPool<void*> base_memory_manager::memory_(16384);


bool transaction::initialized_ = false;
///////////////////////////////////////////////////////////////////////////////
// first param = initialSleepTime (millis)
// second param = sleepIncrease factor (initialSleepTime * factor)
// third param = # of increases before resetting
///////////////////////////////////////////////////////////////////////////////
#if defined(BOOST_STM_CM_STATIC_CONF)
#if defined(BOOST_STM_CM_STATIC_CONF_except_and_back_off_on_abort_notice_cm)
   //boost::stm::contention_manager_type boost::stm::transaction::cm_(0, 0, 0);
   int boost::stm::except_and_back_off_on_abort_notice_cm::sleepTime_=0;
   int const boost::stm::except_and_back_off_on_abort_notice_cm::kSleepFactorIncrease_=0;
   int const boost::stm::except_and_back_off_on_abort_notice_cm::kMaxIncreases_=0;
   int const boost::stm::except_and_back_off_on_abort_notice_cm::initialSleepTime_=0;
   int boost::stm::except_and_back_off_on_abort_notice_cm::kMaxSleepTime_=0;
#else
    BOOST_STM_CM_STATIC();
#endif


#else
base_contention_manager *poly_contention_manager::cm_ =
    new ExceptAndBackOffOnAbortNoticeCM(0, 0, 0);
//    new DefaultContentionManager();
//    new NoExceptionOnAbortNoticeOnReadWritesCM();
//    new DefaultContentionManager();
//    new ExceptAndBackOffOnAbortNoticeCM(5, 2, 10);
#endif
transaction_bookkeeping transaction::bookkeeping_;


#ifndef USE_SINGLE_THREAD_CONTEXT_MAP
#if PERFORMING_LATM
#if USING_TRANSACTION_SPECIFIC_LATM
latm::thread_id_mutex_set_map transaction::threadConflictingMutexes_;
#endif
latm::thread_id_mutex_set_map transaction::threadObtainedLocks_;
latm::thread_id_mutex_set_map transaction::threadCurrentlyLockedLocks_;
#endif
transaction::ThreadWriteContainer transaction::threadWriteLists_;
transaction::ThreadReadContainer transaction::threadReadLists_;
//transaction::ThreadEagerReadContainer transaction::threadEagerReadLists_;
transaction::ThreadMemoryContainerList transaction::threadNewMemoryLists_;
transaction::ThreadMemoryContainerList transaction::threadDeletedMemoryLists_;
transaction::ThreadTxTypeContainer transaction::threadTxTypeLists_;
transaction::ThreadBloomFilterList transaction::threadBloomFilterLists_;
transaction::ThreadBloomFilterList transaction::threadWBloomFilterLists_;
transaction::ThreadBoolContainer transaction::threadForcedToAbortLists_;

transaction::ThreadMutexContainer transaction::threadMutexes_;
transaction::ThreadBoolContainer transaction::threadBlockedLists_;
#else
#ifndef BOOST_STM_HAVE_SINGLE_TSS_CONTEXT_MAP
#if PERFORMING_LATM
#if USING_TRANSACTION_SPECIFIC_LATM
latm::thread_id_mutex_set_map transaction::threadConflictingMutexes_;
#endif
latm::thread_id_mutex_set_map transaction::threadObtainedLocks_;
latm::thread_id_mutex_set_map transaction::threadCurrentlyLockedLocks_;
#endif
transaction::ThreadMutexContainer transaction::threadMutexes_;
transaction::ThreadBoolContainer transaction::threadBlockedLists_;
#endif

transaction::tss_context_map_type transaction::tss_context_map_;
#endif


///////////////////////////////////////////////////////////////////////////////
// static initialization method - must be called before the transaction
// class is used because it initializes our transactionMutex_ which is used
// to guarantee a consistent state of the static
// transactionsInFlight_<transaction* > is correct.
///////////////////////////////////////////////////////////////////////////////
void transaction::initialize()
{
   base_memory_manager::alloc_size(16384);

   if (initialized_) return;
   initialized_ = true;

   logFile_.open("DracoSTM_log.txt");

#ifndef BOOST_STM_USE_BOOST_MUTEX
   pthread_mutex_init(&transactionMutex_, 0);
   pthread_mutex_init(&transactionsInFlightMutex_, 0);
   pthread_mutex_init(&deletionBufferMutex_, 0);
   pthread_mutex_init(&latm::instance().latmMutex_, 0);
#endif
}

///////////////////////////////////////////////////////////////////////////////
void transaction::initialize_thread()
{
#ifdef BOOST_USES_STATIC_TSS
    transaction_tss_storage_type::reset(new transaction_tss_storage());
#endif    
   synchro::lock_guard<Mutex> lock(*general_lock());

   //--------------------------------------------------------------------------
   // WARNING: before you think lock_all_mutexes() does not make sense, make
   //          sure you read the following example, which will certainly change
   //          your mind about what you think you know ... (bug found by Arthur
   //          Athrun)
   //
   //          end_transaction() must lock all mutexes() in addition to the
   //          important general access mutex, which serializes commits.
   //
   //          In order to make end_transaction as efficient as possible, we
   //          must release general_access() before we release the specific
   //          threaded mutexes. Unfortunately, because of this, a thread can
   //          can enter this function and add a new thread (and mutex) to the
   //          mutex list. Then end_transaction() can finish its execution and
   //          unlock all mutexes. The problem is that between end_transaction
   //          and this function, any number of operations can be performed.
   //          One of those operations may lock the mutex of the new thread,
   //          which may then be unlocked by end_transaction. If that happens,
   //          all kinds of inconsistencies could occur ...
   //
   //          In order to fix this, we could change the unlock order of
   //          end_transaction() so it unlocks all mutexes before releasing the
   //          the general mutex. The problem with that is end_transaction is
   //          a high serialization point and the general mutex is the most
   //          contended upon lock. As such, it is not wise to prolong its
   //          release. Instead, we can change this method, so it locks all the
   //          thread's mutexes. This ensures that this method cannot be entered
   //          until end_transaction() completes, guaranteeing that
   //          end_transaction() cannot unlock mutexes it doesn't own.
   //
   //          Questions? Contact Justin Gottschlich or Vicente Botet.
   //
   //          DO NOT REMOVE LOCK_ALL_MUTEXES / UNLOCK_ALL_MUTEXES!!
   //
   //--------------------------------------------------------------------------
   lock_all_mutexes_but_this(this_thread::get_id());

   thread_id_t threadId = this_thread::get_id();

#ifndef USE_SINGLE_THREAD_CONTEXT_MAP
/////////////////////////////////
   ThreadWriteContainer::iterator writeIter = threadWriteLists_.find(threadId);
   ThreadReadContainer::iterator readIter = threadReadLists_.find(threadId);
   ThreadBloomFilterList::iterator bloomIter = threadBloomFilterLists_.find(threadId);
   ThreadBloomFilterList::iterator wbloomIter = threadWBloomFilterLists_.find(threadId);

   ThreadMemoryContainerList::iterator newMemIter = threadNewMemoryLists_.find(threadId);
   ThreadMemoryContainerList::iterator deletedMemIter = threadDeletedMemoryLists_.find(threadId);
   ThreadTxTypeContainer::iterator txTypeIter = threadTxTypeLists_.find(threadId);
   ThreadBoolContainer::iterator abortIter = threadForcedToAbortLists_.find(threadId);
   ThreadMutexContainer::iterator mutexIter = threadMutexes_.find(threadId);

#if PERFORMING_LATM
#if USING_TRANSACTION_SPECIFIC_LATM
   latm::thread_id_mutex_set_map::iterator conflictingMutexIter = threadConflictingMutexes_.find(threadId);
   if (threadConflictingMutexes_.end() == conflictingMutexIter)
   {
      threadConflictingMutexes_[threadId] = new latm::mutex_set;
   }
#endif

   latm::thread_id_mutex_set_map::iterator obtainedLocksIter = threadObtainedLocks_.find(threadId);
   if (threadObtainedLocks_.end() == obtainedLocksIter)
   {
      threadObtainedLocks_[threadId] = new latm::mutex_set;
   }

   latm::thread_id_mutex_set_map::iterator currentlyLockedLocksIter = threadCurrentlyLockedLocks_.find(threadId);
   if (threadCurrentlyLockedLocks_.end() == currentlyLockedLocksIter)
   {
      threadCurrentlyLockedLocks_[threadId] = new latm::mutex_set;
   }
#endif

   ThreadBoolContainer::iterator blockedIter = threadBlockedLists_.find(threadId);

   if (threadWriteLists_.end() == writeIter)
   {
      threadWriteLists_[threadId] = new WriteContainer();
   }

   if (threadReadLists_.end() == readIter)
   {
      threadReadLists_[threadId] = new ReadContainer();
   }

   if (threadBloomFilterLists_.end() == bloomIter)
   {
      bloom_filter *bf = new boost::stm::bloom_filter();
      threadBloomFilterLists_[threadId] = bf;
   }

   if (threadWBloomFilterLists_.end() == wbloomIter)
   {
      bloom_filter *bf = new boost::stm::bloom_filter();
      threadWBloomFilterLists_[threadId] = bf;
   }


   if (threadNewMemoryLists_.end() == newMemIter)
   {
      threadNewMemoryLists_[threadId] = new MemoryContainerList();
   }

   if (threadDeletedMemoryLists_.end() == deletedMemIter)
   {
      threadDeletedMemoryLists_[threadId] = new MemoryContainerList();
   }

   if (threadTxTypeLists_.end() == txTypeIter)
   {
      threadTxTypeLists_[threadId] = new TxType(eNormalTx);
   }

   if (threadForcedToAbortLists_.end() == abortIter)
   {
      threadForcedToAbortLists_.insert(thread_bool_pair(threadId, new int(0)));
   }

   if (threadMutexes_.end() == mutexIter)
   {
      Mutex *mutex = new Mutex;
#ifndef BOOST_STM_USE_BOOST_MUTEX
#if WIN32
      *mutex = PTHREAD_MUTEX_INITIALIZER;
#endif
      pthread_mutex_init(mutex, 0);
#endif
      threadMutexes_.insert(thread_mutex_pair(threadId, mutex));
      mutexIter = threadMutexes_.find(threadId);
   }

   if (threadBlockedLists_.end() == blockedIter)
   {
      threadBlockedLists_.insert(thread_bool_pair(threadId, new int(0)));
   }

//////////////////////////////////////
#else
//////////////////////////////////////
#ifndef BOOST_STM_HAVE_SINGLE_TSS_CONTEXT_MAP
//////////////////////////////////////

   ThreadMutexContainer::iterator mutexIter = threadMutexes_.find(threadId);
   ThreadBoolContainer::iterator blockedIter = threadBlockedLists_.find(threadId);
#if PERFORMING_LATM
#if USING_TRANSACTION_SPECIFIC_LATM
   latm::thread_id_mutex_set_map::iterator conflictingMutexIter = threadConflictingMutexes_.find(threadId);
   if (threadConflictingMutexes_.end() == conflictingMutexIter)
   {
      threadConflictingMutexes_[threadId] = new latm::mutex_set;
   }
#endif

   latm::thread_id_mutex_set_map::iterator obtainedLocksIter = threadObtainedLocks_.find(threadId);
   if (threadObtainedLocks_.end() == obtainedLocksIter)
   {
      threadObtainedLocks_[threadId] = new latm::mutex_set;
   }

   latm::thread_id_mutex_set_map::iterator currentlyLockedLocksIter = threadCurrentlyLockedLocks_.find(threadId);
   if (threadCurrentlyLockedLocks_.end() == currentlyLockedLocksIter)
   {
      threadCurrentlyLockedLocks_[threadId] = new latm::mutex_set;
   }
#endif

   if (threadMutexes_.end() == mutexIter)
   {
      Mutex *mutex = new Mutex;
#ifndef BOOST_STM_USE_BOOST_MUTEX
#if WIN32
      *mutex = PTHREAD_MUTEX_INITIALIZER;
#endif
      pthread_mutex_init(mutex, 0);
#endif
      threadMutexes_.insert(thread_mutex_pair(threadId, mutex));
      mutexIter = threadMutexes_.find(threadId);
   }

   if (threadBlockedLists_.end() == blockedIter)
   {
      threadBlockedLists_.insert(thread_bool_pair(threadId, new int(0)));
   }

   tss_context_map_type::iterator memIter = tss_context_map_.find(threadId);
   if (tss_context_map_.end() == memIter)
   {
      tss_context_map_.insert(std::pair<thread_id_t, tx_context*>(threadId, new tx_context));
      memIter = tss_context_map_.find(threadId);
      memIter->second->txType = eNormalTx;
   }

//////////////////////////////////////
#else
//////////////////////////////////////
   tss_context_map_type::iterator memIter = tss_context_map_.find(threadId);
   if (tss_context_map_.end() == memIter)
   {
      tss_context_map_.insert(std::pair<thread_id_t, tss_context*>(threadId, new tss_context));
      memIter = tss_context_map_.find(threadId);
      memIter->second->tx_.txType = eNormalTx;
   }
#endif
#endif

   //--------------------------------------------------------------------------
   // WARNING: before you think unlock_all_mutexes() does not make sense, make
   //          sure you read the following example, which will certainly change
   //          your mind about what you think you know ... (bug found by Arthur
   //          Athrun)
   //
   //          end_transaction() must lock all mutexes() in addition to the
   //          important general access mutex, which serializes commits.
   //
   //          In order to make end_transaction as efficient as possible, we
   //          must release general_access() before we release the specific
   //          threaded mutexes. Unfortunately, because of this, a thread can
   //          can enter this function and add a new thread (and mutex) to the
   //          mutex list. Then end_transaction() can finish its execution and
   //          unlock all mutexes. The problem is that between end_transaction
   //          and this function, any number of operations can be performed.
   //          One of those operations may lock the mutex of the new thread,
   //          which may then be unlocked by end_transaction. If that happens,
   //          all kinds of inconsistencies could occur ...
   //
   //          In order to fix this, we could change the unlock order of
   //          end_transaction() so it unlocks all mutexes before releasing the
   //          the general mutex. The problem with that is end_transaction is
   //          a high serialization point and the general mutex is the most
   //          contended upon lock. As such, it is not wise to prolong its
   //          release. Instead, we can change this method, so it locks all the
   //          thread's mutexes. This ensures that this method cannot be entered
   //          until end_transaction() completes, guaranteeing that
   //          end_transaction() cannot unlock mutexes it doesn't own.
   //
   //          Questions? Contact Justin Gottschlich or Vicente Botet.
   //
   //          DO NOT REMOVE LOCK_ALL_MUTEXES / UNLOCK_ALL_MUTEXES!!
   //
   //--------------------------------------------------------------------------
   // this will unlock the mutex of the thread that was just added, but it
   // doesn't matter because that mutex will already be unlocked
   //--------------------------------------------------------------------------
   unlock_all_mutexes_but_this(this_thread::get_id());;

   //--------------------------------------------------------------------------

   //synchro::unlock(*general_lock());
}

///////////////////////////////////////////////////////////////////////////////
void transaction::terminate_thread()
{
   synchro::lock_guard<Mutex> lock_g(*general_lock());
   //synchro::lock(*general_lock());
   synchro::lock_guard<Mutex> lock_i(*inflight_lock());
   //synchro::lock(*inflight_lock());

   thread_id_t threadId = this_thread::get_id();

#ifndef USE_SINGLE_THREAD_CONTEXT_MAP
   ThreadWriteContainer::iterator writeIter = threadWriteLists_.find(threadId);
   ThreadReadContainer::iterator readIter = threadReadLists_.find(threadId);
   ThreadMemoryContainerList::iterator newMemIter = threadNewMemoryLists_.find(threadId);
   ThreadMemoryContainerList::iterator deletedMemIter = threadDeletedMemoryLists_.find(threadId);
   ThreadBloomFilterList::iterator bloomIter = threadBloomFilterLists_.find(threadId);
   ThreadBloomFilterList::iterator wbloomIter = threadWBloomFilterLists_.find(threadId);
   ThreadTxTypeContainer::iterator txTypeIter = threadTxTypeLists_.find(threadId);
   ThreadBoolContainer::iterator abortIter = threadForcedToAbortLists_.find(threadId);

   delete writeIter->second;
   delete readIter->second;
   delete bloomIter->second;
   delete wbloomIter->second;

   delete newMemIter->second;
   delete deletedMemIter->second;
   delete txTypeIter->second;
   delete abortIter->second;

   threadWriteLists_.erase(writeIter);
   threadReadLists_.erase(readIter);
   threadBloomFilterLists_.erase(bloomIter);
   threadWBloomFilterLists_.erase(wbloomIter);

   threadNewMemoryLists_.erase(newMemIter);
   threadDeletedMemoryLists_.erase(deletedMemIter);
   threadTxTypeLists_.erase(txTypeIter);
   threadForcedToAbortLists_.erase(abortIter);

   ThreadMutexContainer::iterator mutexIter = threadMutexes_.find(threadId);
#ifndef BOOST_STM_USE_BOOST_MUTEX
   pthread_mutex_destroy(mutexIter->second);
#endif
   delete mutexIter->second;
   threadMutexes_.erase(mutexIter);

#ifndef MAP_THREAD_MUTEX_CONTAINER
   {
   // realign all in-flight transactions so they are accessing the correct mutex
   for (in_flight_trans_cont::iterator i = transactionsInFlight_.begin();
      i != transactionsInFlight_.end(); ++i)
   {
      transaction* t = *i;

      t->mutexRef_ = &mutex(t->threadId_);
   }
   }
#endif

   ThreadBoolContainer::iterator blockedIter = threadBlockedLists_.find(threadId);
   delete blockedIter->second;
   threadBlockedLists_.erase(blockedIter);

#if PERFORMING_LATM
#if USING_TRANSACTION_SPECIFIC_LATM
   latm::thread_id_mutex_set_map::iterator conflictingMutexIter = threadConflictingMutexes_.find(threadId);
   delete conflictingMutexIter->second;
   threadConflictingMutexes_.erase(conflictingMutexIter);
#endif

   latm::thread_id_mutex_set_map::iterator obtainedLocksIter = threadObtainedLocks_.find(threadId);
   delete obtainedLocksIter->second;
   threadObtainedLocks_.erase(obtainedLocksIter);

   latm::thread_id_mutex_set_map::iterator currentlyLockedLocksIter = threadCurrentlyLockedLocks_.find(threadId);
   delete currentlyLockedLocksIter->second;
   threadCurrentlyLockedLocks_.erase(currentlyLockedLocksIter);
#endif


#else
   tss_context_map_type::iterator memIter = tss_context_map_.find(threadId);
   delete memIter->second;
   tss_context_map_.erase(memIter);
#endif




#ifndef MAP_THREAD_BOOL_CONTAINER
   {
   // realign all in-flight transactions so they are accessing the correct mutex
   for (in_flight_trans_cont::iterator i = transactionsInFlight_.begin();
      i != transactionsInFlight_.end(); ++i)
   {
      transaction* t = *i;

      t->forcedToAbortRef_ = threadForcedToAbortLists_.find(t->threadId_)->second;
      t->blockedRef_ = blocked(t->threadId_);
   }
   }
#endif


   //synchro::unlock(*inflight_lock());
   //synchro::unlock(*general_lock());
}

}}
