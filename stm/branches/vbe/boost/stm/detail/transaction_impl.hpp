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

//-----------------------------------------------------------------------------
//
// TransactionImpl.h
//
// This file contains method implementations for transaction.hpp. The main
// purpose of this file is to reduce the complexity of the transaction class
// by separating its implementation into a secondary .h file.
//
// Do NOT place these methods in a .cc/.cpp/.cxx file. These methods must be
// inlined to keep DracoSTM performing fast. If these methods are placed in a
// C++ source file they will incur function call overhead - something we need
// to reduce to keep performance high.
//
//-----------------------------------------------------------------------------
#ifndef BOOST_STM_TRANSACTION_IMPL_H
#define BOOST_STM_TRANSACTION_IMPL_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <math.h>

//using namespace std;

namespace boost { namespace stm {

//--------------------------------------------------------------------------
//
// PRE-CONDITION: transactionsInFlightMutex is obtained prior to call
//
//--------------------------------------------------------------------------
inline bool transaction::isolatedTxInFlight()
{
BOOST_STM_TRANSACTION_INVARIANT;
    //~ BOOST_STM_INFO<<std::endl;
   for (in_flight_trans_cont::iterator i = in_flight_transactions().begin();
      i != in_flight_transactions().end(); ++i)
   {
        BOOST_ASSERT(*i!=0);
        BOOST_STM_TRANSACTION_INVARIANT_VAR(*i);

    //~ BOOST_STM_INFO<<std::endl;
      // if this is our threadId, skip it
      if ((*i)->threadId_ == this->threadId_) continue;
    //~ BOOST_STM_INFO<<std::endl;

      if ((*i)->isolated()) return true;
    //~ BOOST_STM_INFO<<std::endl;
   }
    //~ BOOST_STM_INFO<<std::endl;

   return false;
}

//--------------------------------------------------------------------------
//
// PRE-CONDITION: transactionsInFlightMutex is obtained prior to call
//
//--------------------------------------------------------------------------
inline bool transaction::irrevocableTxInFlight()
{
BOOST_STM_TRANSACTION_INVARIANT;
   for (in_flight_trans_cont::iterator i = in_flight_transactions().begin();
      i != in_flight_transactions().end(); ++i)
   {
        BOOST_ASSERT(*i!=0);
        BOOST_STM_TRANSACTION_INVARIANT_VAR(*i);
      // if this is our threadId, skip it
      if ((*i)->threadId_ == this->threadId_) continue;

      if ((*i)->irrevocable()) return true;
   }

   return false;
}

//--------------------------------------------------------------------------
//
// PRE-CONDITION: transactionMutex is obtained prior to call
// PRE-CONDITION: transactionsInFlightMutex is obtained prior to call
//
//--------------------------------------------------------------------------
inline bool transaction::abortAllInFlightTxs()
{
BOOST_STM_TRANSACTION_INVARIANT;
   for (in_flight_trans_cont::iterator i = in_flight_transactions().begin();
      i != in_flight_transactions().end(); ++i)
   {
        BOOST_ASSERT(*i!=0);
        BOOST_STM_TRANSACTION_INVARIANT_VAR(*i);
      // if this is our threadId, skip it
      if ((*i)->threadId_ == this->threadId_) continue;

      (*i)->force_to_abort();
   }

   return true;
}

//--------------------------------------------------------------------------
//
// PRE-CONDITION: transactionsInFlightMutex is obtained prior to call
//
//--------------------------------------------------------------------------
inline bool transaction::canAbortAllInFlightTxs()
{
BOOST_STM_TRANSACTION_INVARIANT;
   for (in_flight_trans_cont::iterator i = in_flight_transactions().begin();
      i != in_flight_transactions().end(); ++i)
   {
        BOOST_ASSERT(*i!=0);
        BOOST_STM_TRANSACTION_INVARIANT_VAR(*i);
      // if this is our threadId, skip it
      if ((*i)->threadId_ == this->threadId_) continue;

      if (cm_permission_to_abort(*(*i), *this)) return false;
   }

   return true;
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
inline void transaction::make_irrevocable()
{
BOOST_STM_TRANSACTION_INVARIANT;
   if (irrevocable()) return;
   //-----------------------------------------------------------------------
   // in order to make a tx irrevocable, no other irrevocable txs can be
   // running. if there are, we must stall until they commit.
   //-----------------------------------------------------------------------
   while (true)
   {
      {
      synchro::lock_guard<Mutex> lock_m(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));

      if (!irrevocableTxInFlight())
      {
         tx_type(eIrrevocableTx);
         return;
      }

      }
      SLEEP(10);
      cm_perform_irrevocable_tx_wait_priority_promotion(*this);
   }
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
inline void transaction::make_isolated()
{
BOOST_STM_TRANSACTION_INVARIANT;
   if (isolated()) return;

   //using namespace std;
   //-----------------------------------------------------------------------
   // in order to make a tx irrevocable, no other irrevocable txs can be
   // running. if there are, we must stall until they commit.
   //-----------------------------------------------------------------------
   while (true)
   {
      if (forced_to_abort())
      {
         lock_and_abort();
         throw aborted_transaction_exception
         ("aborting tx in make_isolated");
      }

      {
      synchro::lock_guard<Mutex> lock_g(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
      synchro::lock_guard<Mutex> lock_i(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));

      if (!irrevocableTxInFlight() && canAbortAllInFlightTxs())
      {
         tx_type(eIrrevocableAndIsolatedTx);
         abortAllInFlightTxs();
         return;
      }

     }
      //SLEEP(10);
      cm_perform_isolated_tx_wait_priority_promotion(*this);
   }
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
inline bool transaction::irrevocable() const
{
BOOST_STM_TRANSACTION_INVARIANT;
   //~ BOOST_STM_INFO << "tx_type="<<tx_type()<<"@"<<const_cast<transaction*>(this)->tx_type_ptr()<<std::endl;
   switch (tx_type())
   {
   case eNormalTx: return false;
   case eIrrevocableTx: return true;
   case eIrrevocableAndIsolatedTx: return true;
   default:
            BOOST_STM_ERROR << "tx_type="<<tx_type()<<"@"<<const_cast<transaction*>(this)->tx_type_ptr()<<std::endl;
        BOOST_ASSERT(false&&"tx type not found");
        return false;
   }
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
inline bool transaction::isolated() const
{
BOOST_STM_TRANSACTION_INVARIANT;
   //~ BOOST_STM_INFO << "tx_type="<<tx_type()<<"@"<< const_cast<transaction*>(this)->tx_type_ptr()<<std::endl;
   switch (tx_type())
   {
   case eNormalTx: return false;
   case eIrrevocableTx: return false;
   case eIrrevocableAndIsolatedTx: return true;
   default:
            BOOST_STM_ERROR << "tx_type="<<tx_type()<<"@"<<const_cast<transaction*>(this)->tx_type_ptr()<<std::endl;
        BOOST_ASSERT(false&&"tx type not found");
        return false;
   }
}

//-----------------------------------------------------------------------------
//
// ASSUMPTIONS: this tx MUST be isolated and using DEFERRED update
//
// This method commits the tx's writes and clears the tx's news/deletes
//
//-----------------------------------------------------------------------------
inline void transaction::commit_deferred_update_tx()
{
BOOST_STM_TRANSACTION_INVARIANT;
   // ensure this method is isolated
   if (!this->irrevocable()) throw invalid_operation("cannot commit deferred tx: not isolated");

   //--------------------------------------------------------------------------
   // otherwise, force the tx to commit its writes/reads
   //--------------------------------------------------------------------------
   #if 0
   while (!synchro::try_lock(&transactionMutex_))
   {
      bookkeeping_.inc_lock_convoy_ms(1);
      SLEEP(1);
   }
   #endif

   synchro::lock_guard<Mutex> lock_g(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
   synchro::lock_guard<Mutex> lock_m(*mutex() BOOST_STM_CALL_CONTEXT("mutex"));

   //--------------------------------------------------------------------------
   // this is a very important and subtle optimization. if the transaction is
   // only reading memory, it does not need to lock the system. it only needs
   // to lock itself and the tx in-flight mutex and remove itself from the
   // tx in flight list
   //--------------------------------------------------------------------------
   if (is_only_reading())
   {
   }
   else
   {
      synchro::lock_guard<Mutex> lock_i(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));

      //-----------------------------------------------------------------------
      // commit writes, clear new and deletes
      //-----------------------------------------------------------------------
      deferredCommitWriteState();
#ifndef DISABLE_READ_SETS
      readList().clear();
#endif
      deferredCommitTransactionNewMemory();
      deferredCommitTransactionDeletedMemory();

   }

   //--------------------------------------------------------------------------
   // the tx remains in-flight
   //--------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
inline void transaction::lock_tx()
{
BOOST_STM_TRANSACTION_INVARIANT;
   while (!synchro::try_lock(*mutex()))
   {
      SLEEP(1);
   }

   //hasMutex_ = 1;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
inline void transaction::unlock_tx()
{
BOOST_STM_TRANSACTION_INVARIANT;
   synchro::unlock(*mutex() BOOST_STM_CALL_CONTEXT("mutex"));
   //hasMutex_ = 0;
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
inline void transaction::lock_all_mutexes_but_this(thread_id_t threadId)
{
    BOOST_STM_CALL_CONTEXT_DCL_INST(0);
#ifdef BOOST_STM_HAVE_SINGLE_TSS_CONTEXT_MAP
    for (tss_context_map_type::iterator i = tss_context_map_.begin();
      i != tss_context_map_.end(); ++i)
   {
        BOOST_ASSERT(i->secondi!=0);

      if (i->first == threadId) continue;
      synchro::lock(i->second->mutex_ BOOST_STM_CALL_CONTEXT("mutex")); // INSTANCE
   }
#else
    for (ThreadMutexContainer::iterator i = threadMutexes_.begin();
      i != threadMutexes_.end(); ++i)
   {
      if (i->first == threadId) continue;
        BOOST_ASSERT(i->second!=0);

      synchro::lock(*(i->second) BOOST_STM_CALL_CONTEXT("mutex"));
   }
#endif
}

//--------------------------------------------------------------------------
inline void transaction::unlock_all_mutexes_but_this(thread_id_t threadId)
{
    BOOST_STM_CALL_CONTEXT_DCL_INST(0);
#ifdef BOOST_STM_HAVE_SINGLE_TSS_CONTEXT_MAP
    for (tss_context_map_type::iterator i = tss_context_map_.begin();
      i != tss_context_map_.end(); ++i)
   {
      if (i->first == threadId) continue;
      synchro::unlock(i->second->mutex_ BOOST_STM_CALL_CONTEXT("mutex"));
   }
#else
   for (ThreadMutexContainer::iterator i = threadMutexes_.begin();
      i != threadMutexes_.end(); ++i)
   {
      if (i->first == threadId) continue;

        BOOST_ASSERT(i->second!=0);

      synchro::unlock(*(i->second) BOOST_STM_CALL_CONTEXT("mutex"));
   }
#endif
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
inline void transaction::lock_all_mutexes()
{
BOOST_STM_TRANSACTION_INVARIANT;
#ifdef BOOST_STM_HAVE_SINGLE_TSS_CONTEXT_MAP
    for (tss_context_map_type::iterator i = tss_context_map_.begin();
      i != tss_context_map_.end(); ++i)
   {
        BOOST_ASSERT(i->second!=0);

      synchro::lock(i->second->mutex_ BOOST_STM_CALL_CONTEXT("mutex"));
   }
#else
   for (ThreadMutexContainer::iterator i = threadMutexes_.begin();
      i != threadMutexes_.end(); ++i)
   {
        BOOST_ASSERT(i->second!=0);

      synchro::lock(*(i->second) BOOST_STM_CALL_CONTEXT("mutex"));
   }
#endif
   //hasMutex_ = 1;
}

//-----------------------------------------------------------------------------
// side-effect: this unlocks all mutexes including its own. this is a slight
// optimization over unlock_all_mutexes_but_this() as it doesn't have an
// additional "if" to slow down performance. however, as it will be
// releasing its own mutex, it must reset hasMutex_
//-----------------------------------------------------------------------------
inline void transaction::unlock_all_mutexes()
{
BOOST_STM_TRANSACTION_INVARIANT;
#ifdef BOOST_STM_HAVE_SINGLE_TSS_CONTEXT_MAP
    for (tss_context_map_type::iterator i = tss_context_map_.begin();
      i != tss_context_map_.end(); ++i)
   {
        BOOST_ASSERT(i->second!=0);
      synchro::unlock(i->second->mutex_ BOOST_STM_CALL_CONTEXT("mutex"));
   }
#else
   for (ThreadMutexContainer::iterator i = threadMutexes_.begin();
      i != threadMutexes_.end(); ++i)
   {
        BOOST_ASSERT(i->second!=0);
      synchro::unlock(*(i->second) BOOST_STM_CALL_CONTEXT("mutex"));
   }
#endif
   //hasMutex_ = 0;
}

//--------------------------------------------------------------------------
//
//
//             THREE ENTRY POINTS TO STARTING A TRANSACTION
//
//             constructor
//             begin()
//             restart()
//
//
//--------------------------------------------------------------------------
inline transaction::transaction() :
   threadId_(this_thread::get_id()),
    //~ transaction_tss_storage_ref_(*transaction_tss_storage_),


   auto_general_lock_(*general_lock()),

#if USE_SINGLE_THREAD_CONTEXT_MAP
////////////////////////////////////////
// BUG possible core dump if find doesn't suceed
   BOOST_STM_ASSERT_VAR_INIT(context_,*tss_context_map_.find(threadId_)->second),

#ifdef BOOST_STM_TX_CONTAINS_REFERENCES_TO_TSS_FIELDS
#ifndef BOOST_STM_HAVE_SINGLE_TSS_CONTEXT_MAP
   BOOST_STM_ASSERT_VAR_INIT(write_list_ref_,&context_.writeMem),
   bloomRef_(&context_.bloom),
   wbloomRef_(&context_.wbloom),
   newMemoryListRef_(&context_.newMem),
   deletedMemoryListRef_(&context_.delMem),
   BOOST_STM_ASSERT_VAR_INIT(txTypeRef_,&context_.txType),
#ifdef USING_SHARED_FORCED_TO_ABORT
   forcedToAbortRef_(&context_.abort),
#else
   forcedToAbortRef_(false),
#endif
#else
   BOOST_STM_ASSERT_VAR_INIT(write_list_ref_,&context_.tx_.writeMem),
   bloomRef_(&context_.tx_.bloom),
   wbloomRef_(&context_.tx_.wbloom),
   newMemoryListRef_(&context_.tx_.newMem),
   deletedMemoryListRef_(&context_.tx_.delMem),
   BOOST_STM_ASSERT_VAR_INIT(txTypeRef_,&context_.tx_.txType),
#ifdef USING_SHARED_FORCED_TO_ABORT
   forcedToAbortRef_(&context_.tx_.abort),
#else
   forcedToAbortRef_(false),
#endif
#endif
#endif
#ifndef BOOST_STM_HAVE_SINGLE_TSS_CONTEXT_MAP
   mutexRef_(threadMutexes_.find(threadId_)->second),

#if PERFORMING_LATM
   blockedRef_(*blocked_ptr(threadId_)),
#endif

#if PERFORMING_LATM
#if USING_TRANSACTION_SPECIFIC_LATM
// BUG possible core dump if find doesn't suceed
   conflictingMutexRef_(*threadConflictingMutexes_.find(threadId_)->second),
#endif
// BUG possible core dump if find doesn't suceed
   obtainedLocksRef_(*threadObtainedLocks_.find(threadId_)->second),
// BUG possible core dump if find doesn't suceed
   BOOST_STM_ASSERT_VAR_INIT(currentlyLockedLocksRef_,*threadCurrentlyLockedLocks_.find(threadId_)->second),
#endif
#endif
////////////////////////////////////////
#else
////////////////////////////////////////
#ifndef DISABLE_READ_SETS
// BUG possible core dump if find doesn't suceed
   readListRef_(*threadReadLists_.find(threadId_)->second),
#endif
   BOOST_STM_ASSERT_VAR_INIT(write_list_ref_,threadWriteLists_.find(threadId_)->second),
   bloomRef_(threadBloomFilterLists_.find(threadId_)->second),
#if PERFORMING_WRITE_BLOOM
   wbloomRef_(threadWBloomFilterLists_.find(threadId_)->second),
   //sm_wbv_(*threadSmallWBloomFilterLists_.find(threadId_)->second),
#endif
   newMemoryListRef_(threadNewMemoryLists_.find(threadId_)->second),
   deletedMemoryListRef_(threadDeletedMemoryLists_.find(threadId_)->second),
   BOOST_STM_ASSERT_VAR_INIT(txTypeRef_,threadTxTypeLists_.find(threadId_)->second),
#ifdef USING_SHARED_FORCED_TO_ABORT
   forcedToAbortRef_(threadForcedToAbortLists_.find(threadId_)->second),
#else
   forcedToAbortRef_(false),
#endif

   mutexRef_(threadMutexes_.find(threadId_)->second),

#if PERFORMING_LATM
   blockedRef_(*blocked_ptr(threadId_)),
#endif

#if PERFORMING_LATM
#if USING_TRANSACTION_SPECIFIC_LATM
// BUG possible core dump if find doesn't suceed
   conflictingMutexRef_(*threadConflictingMutexes_.find(threadId_)->second),
#endif
// BUG possible core dump if find doesn't suceed
   obtainedLocksRef_(*threadObtainedLocks_.find(threadId_)->second),
   BOOST_STM_ASSERT_VAR_INIT(currentlyLockedLocksRef_,*threadCurrentlyLockedLocks_.find(threadId_)->second),
#endif
////////////////////////////////////////
#endif
   //hasMutex_(0),
   priority_(0),
   state_(e_no_state),
   reads_(0),
   startTime_(time(0)),
#ifndef BOOST_STM_USE_STACK
   BOOST_STM_ASSERT_VAR_INIT(parent_,stm::current_transaction()),
#else
   //~ nested_((stm::current_transaction()!=0));
   BOOST_STM_ASSERT_VAR_INIT(nested_,(transaction_tss_storage_->transactions_.top()!=0)),
#endif
    assert_end_flag_(0xffffffff)
{
   auto_general_lock_.unlock();
    //~ {
        //~ boost::lock_guard<boost::mutex> lk(log_mutex);
        //~ std::cout << this << " " << this->parent() << " " << this_thread::get_id() << " ****INFO transaction" << __FILE__ << "[" << __LINE__ << "]" << BOOST_CURRENT_FUNCTION << std::endl;
    //~ }
   if (direct_updating()) doIntervalDeletions();
#if PERFORMING_LATM
   while (blocked()) { SLEEP(10) ; }
#endif

   put_tx_inflight();
   transactions().push(this);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
inline void transaction::begin()
{
BOOST_STM_TRANSACTION_INVARIANT;
   if (e_in_flight == state_) return;

#if PERFORMING_LATM
   while (blocked()) { SLEEP(10) ; }
#endif
   put_tx_inflight();
}

#ifdef LOGGING_BLOCKS
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
inline std::string transaction::outputBlockedThreadsAndLockedLocks()
{
BOOST_STM_TRANSACTION_INVARIANT;
   using namespace std;

   ostringstream o;

   o << "Threads and their conflicting mutexes:" << endl << endl;

   for (latm::thread_id_mutex_set_map::iterator iter = threadConflictingMutexes_.begin();
   threadConflictingMutexes_.end() != iter; ++iter)
   {
      // if this mutex is found in the transaction's conflicting mutexes
      // list, then allow the thread to make forward progress again
      // by turning its "blocked" but only if it does not appear in the
      // locked_locks_thread_id_map
      o << iter->first << " blocked: " << *blocked_ptr(iter->first) << endl;
      o << "\t";

      for (latm::mutex_set::iterator inner = iter->second->begin(); inner != iter->second->end(); ++inner)
      {
         o << *inner << " ";
      }
      o << endl;
   }

   o << "Currently locked locks:" << endl << endl;

   for (latm::mutex_thread_id_set_map::iterator i = latm::instance().latmLockedLocksAndThreadIdsMap_.begin();
   i != latm::instance().latmLockedLocksAndThreadIdsMap_.end(); ++i)
   {
      o << i->first << endl << "\t";

      for (ThreadIdSet::iterator j = i->second.begin(); j != i->second.end(); ++j)
      {
         o << *j << " ";
      }
      o << endl;
   }

   return o.str();
}
#endif

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
inline bool transaction::restart()
{
BOOST_STM_TRANSACTION_INVARIANT;
    //~ BOOST_STM_INFO<<std::endl;
   if (e_in_flight == state_) lock_and_abort();
    //~ BOOST_STM_INFO<<std::endl;

#if PERFORMING_LATM
#ifdef LOGGING_BLOCKS
    //~ BOOST_STM_INFO<<std::endl;
   int iterations = 0;
    //~ BOOST_STM_INFO<<std::endl;
#endif
    //~ BOOST_STM_INFO<<std::endl;
   while (blocked())
   {
#ifdef LOGGING_BLOCKS
      if (++iterations > 100)
      {
         synchro::lock_guard<Mutex> autolock_l(*latm_lock() BOOST_STM_CALL_CONTEXT("latm_lock"));
         synchro::lock_guard<Mutex> autolock_g(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
         //unblock_threads_if_locks_are_empty();
         logFile_ << outputBlockedThreadsAndLockedLocks().c_str();
         SLEEP(10000);
      }
#endif

      SLEEP(10);
   }
#endif
    //~ BOOST_STM_INFO<<std::endl;
   //-----------------------------------------------------------------------
   // this is a vital check for composed transactions that abort, but the
   // outer instance tx is never destructed, but instead restarted via
   // restart() interface
   //-----------------------------------------------------------------------
#if PERFORMING_COMPOSITION
#ifdef USING_SHARED_FORCED_TO_ABORT
   {
   synchro::lock_guard<Mutex> lock_i(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));
BOOST_STM_TRANSACTION_INVARIANT;
    //~ BOOST_STM_INFO<<std::endl;
   if (!otherInFlightTransactionsOfSameThreadNotIncludingThis(this))
   {
    //~ BOOST_STM_INFO<<std::endl;
      unforce_to_abort();
    //~ BOOST_STM_INFO<<std::endl;
   }
   }
#else
    //~ BOOST_STM_INFO<<std::endl;
   unforce_to_abort();
    //~ BOOST_STM_INFO<<std::endl;
#endif
#endif

   {BOOST_STM_TRANSACTION_INVARIANT;

    //~ BOOST_STM_INFO<<std::endl;
   put_tx_inflight();
   }
    //~ BOOST_STM_INFO<<std::endl;

#if 0
   if (doing_dynamic_priority_assignment())
   {
      priority_ += 1 + (reads_ / 100);
   }

   reads_ = 0;
#endif

    //~ BOOST_STM_INFO<<std::endl;
   return true;
}

#if 0
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
inline bool transaction::can_go_inflight()
{
BOOST_STM_TRANSACTION_INVARIANT;
   // if we're doing full lock protection, allow transactions
   // to start only if no locks are obtained or the only lock that
   // is obtained is on this_thread::get_id()
   if (latm::instance().doing_full_lock_protection())
   {
      for (latm::mutex_thread_id_map::iterator j = latmLockedLocksOfThreadMap_.begin();
      j != latmLockedLocksOfThreadMap_.end(); ++j)
      {
         if (this_thread::get_id() != j->second)
         {
            return false;
         }
      }
   }

   // if we're doing tm lock protection, allow transactions
   // to start only if
   else if (latm::instance().doing_tm_lock_protection())
   {
      for (latm::mutex_set::iterator i = tmConflictingLocks_.begin(); i != tmConflictingLocks_.end(); ++i)
      {
         // if one of your conflicting locks is currently locked ...
         if (latmLockedLocks_.end() != latmLockedLocks_.find(*i))
         {
            // if it is locked by our thread, it is ok ... otherwise it is not
            latm::mutex_thread_id_map::iterator j = latmLockedLocksOfThreadMap_.find(*i);

            if (j != latmLockedLocksOfThreadMap_.end() &&
               this_thread::get_id() != j->second)
            {
               return false;
            }
         }
      }
   }

   return true;
}
#endif
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
inline void transaction::put_tx_inflight()
{
BOOST_STM_TRANSACTION_INVARIANT;
    //~ BOOST_STM_INFO<<std::endl;
#if PERFORMING_LATM
   while (true)
   {
    //~ BOOST_STM_INFO<<std::endl;
      {
      synchro::lock_guard<Mutex> lock_i(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));
    //~ BOOST_STM_INFO<<std::endl;

          bool b1=latm::instance().can_go_inflight();
    //~ BOOST_STM_INFO<<std::endl;
          bool b2=!isolatedTxInFlight();
    //~ BOOST_STM_INFO<<std::endl;
      if (b1 && b2)
      {
    //~ BOOST_STM_INFO<<std::endl;
BOOST_STM_TRANSACTION_INVARIANT;

         in_flight_transactions().insert(this);
         state_ = e_in_flight;
         break;
      }
    //~ BOOST_STM_INFO<<std::endl;

      }
      SLEEP(10);
   }
#else
   synchro::lock_guard<Mutex> lock_i(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));
    //~ BOOST_STM_INFO<<std::endl;
   {BOOST_STM_TRANSACTION_INVARIANT;

   in_flight_transactions().insert(this);
   }
    //~ BOOST_STM_INFO<<std::endl;
   state_ = e_in_flight;
#endif
    //~ BOOST_STM_INFO<<std::endl;
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
inline transaction::~transaction()
{
    BOOST_STM_CALL_CONTEXT_DCL_INST(this);
    //~ {
        //~ boost::lock_guard<boost::mutex> lk(log_mutex);
        //~ std::cout << this << " " << this->parent() << " " << this_thread::get_id() << " ****INFO ~transaction" << __FILE__ << "[" << __LINE__ << "]" << BOOST_CURRENT_FUNCTION << std::endl;
    //~ }
    //~ BOOST_STM_INFO<<std::endl;
   // if we're not an inflight transaction - bail
   if (state_ != e_in_flight)
   {
   //~ BOOST_STM_INFO<<std::endl;
      //if (hasLock()) unlock_tx();
      transactions().pop();
      return;
   }

   //~ BOOST_STM_INFO<<std::endl;
    //if (!hasLock())
    {
       synchro::lock_guard<Mutex> lock(*mutex() BOOST_STM_CALL_CONTEXT("mutex"));
   //~ BOOST_STM_INFO<<std::endl;
        abort();
   //~ BOOST_STM_INFO<<std::endl;
    }
   //~ BOOST_STM_INFO<<std::endl;
    transactions().pop();
   //~ BOOST_STM_INFO<<std::endl;

    // BUG not removed from the list because the test is inversed
    //~ if (alreadyRemovedFromInFlight)

    //~ synchro::lock(*inflight_lock());
    // if I'm the last transaction of this thread, reset abort to false
    //~ in_flight_transactions().erase(this);


}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
inline void transaction::no_throw_end()
{
BOOST_STM_TRANSACTION_INVARIANT;
   try { end(); }
   catch (...) {}
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
inline void transaction::end()
{
BOOST_STM_TRANSACTION_INVARIANT;
    // in case this is called multiple times
   if (!in_flight()) return;

   if (direct_updating())
   {
BOOST_STM_TRANSACTION_INVARIANT;
#if PERFORMING_VALIDATION
      validating_direct_end_transaction();
#else
      invalidating_direct_end_transaction();
#endif
   }
   else
   {
BOOST_STM_TRANSACTION_INVARIANT;
#if PERFORMING_VALIDATION
      validating_deferred_end_transaction();
#else
      invalidating_deferred_end_transaction();
#endif
   }
#if defined(BOOST_STM_CACHE_USE_TSS_MONOTONIC_MEMORY_MANAGER)
    context_.mstorage_.reset();
#endif
}

//-----------------------------------------------------------------------------
// lock_and_abort()
//
// used for contention managers when direct updating is happening and the tx
// has already performed some writes. a direct_abort expects the
// transactionMutex_ to be obtained so it can restore the global memory to its
// original state.
//-----------------------------------------------------------------------------
inline void transaction::lock_and_abort()
{
BOOST_STM_TRANSACTION_INVARIANT;
   if (direct_updating())
   {
      synchro::lock_guard_if<Mutex> lock_g(*general_lock(), isWriting() BOOST_STM_CALL_CONTEXT("general_lock"));
      synchro::lock_guard<Mutex> lock_m(*mutex() BOOST_STM_CALL_CONTEXT("mutex"));
      direct_abort();
   }
   else
   {
      synchro::lock_guard<Mutex> lock_m(*mutex() BOOST_STM_CALL_CONTEXT("mutex"));
      deferred_abort();
   }
}

//-----------------------------------------------------------------------------
// invalidating_direct_end_transaction()
//-----------------------------------------------------------------------------
inline void transaction::invalidating_direct_end_transaction()
{
BOOST_STM_TRANSACTION_INVARIANT;
   //--------------------------------------------------------------------------
   // this is an optimization to check forced to abort before obtaining the
   // transaction mutex, so if we do need to abort we do it now
   //--------------------------------------------------------------------------
   if (forced_to_abort())
   {
      lock_and_abort();
      throw aborted_transaction_exception
      ("aborting committing transaction due to contention manager priority inversion");
   }

   synchro::lock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
    //synchro::unique_lock<Mutex> lk_g(*general_lock());
   //lock_tx();
   synchro::lock(*mutex() BOOST_STM_CALL_CONTEXT("mutex"));
    //synchro::lock_guard<Mutex> lk_m(*mutex());
    // direct_abort_if_not_commited_and_not_handoff(this);

   //--------------------------------------------------------------------------
   // erase this from the inflight transactions so processing through the
   // transactionsInFlight is faster. we have removed the check for "this"
   // when iterating through the transactionsInFlight list. additionally,
   // remember that transactionsInFlight has been removed for commit()
   // so commit doesn't need to remove this from the transactionsInFlight
   //--------------------------------------------------------------------------
   if (forced_to_abort())
   {
      //-----------------------------------------------------------------------
      // if it wasn't writing, it doesn't need the transactionMutex any more,
      // so unlock it so we can reduce contention
      //-----------------------------------------------------------------------
      bool wasWriting = isWriting() ? true : false;
      if (!wasWriting) synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock")); // TBR
      //if (!wasWriting) lk_g.unlock();
      direct_abort(); // TBR
      synchro::unlock(*mutex() BOOST_STM_CALL_CONTEXT("mutex")); // TBR

      //-----------------------------------------------------------------------
      // if this tx was writing, unlock the transaction mutex now
      //-----------------------------------------------------------------------
      if (wasWriting) synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));// TBR
      throw aborted_transaction_exception
      ("aborting committing transaction due to contention manager priority inversion");
   }

   lock_all_mutexes_but_this(threadId_);// TBR
   // all_mutexes_but_this all_but_this(this, threadId_);

   synchro::lock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));// TBR
    //synchro::lock_guard<Mutex> lk_i(*inflight_lock());
   {BOOST_STM_TRANSACTION_INVARIANT;
        boost::lock_guard<boost::mutex> lk(log_mutex);
        std::cout << this << " " << this->parent() << " " << this_thread::get_id() << " ****INFO erase" << __FILE__ << "[" << __LINE__ << "]" << BOOST_CURRENT_FUNCTION << std::endl;
   in_flight_transactions().erase(this);
   }
   if (other_in_flight_same_thread_transactions())
   {
      state_ = e_hand_off;
      unlock_all_mutexes();// TBR
      synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));// TBR
      synchro::unlock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));// TBR
      bookkeeping_.inc_handoffs();
   }
   else
   {
      // commit releases the inflight mutex as soon as its done with it
      invalidating_direct_commit();

      if (e_committed == state_)
      {
         unlock_all_mutexes();
         synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
         synchro::unlock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));
      } else {
          BOOST_STM_ERROR<<std::endl;
          std::cout << "invalidating_direct_end_transaction e_committed != state_" << std::endl;
      }
   }
}

//-----------------------------------------------------------------------------
// invalidating_deferred_end_transaction()
//-----------------------------------------------------------------------------
inline void transaction::invalidating_deferred_end_transaction()
{
BOOST_STM_TRANSACTION_INVARIANT;
   //--------------------------------------------------------------------------
   // this is an optimization to check forced to abort before obtaining the
   // transaction mutex, so if we do need to abort we do it now
   //--------------------------------------------------------------------------
#ifndef DELAY_INVALIDATION_DOOMED_TXS_UNTIL_COMMIT
   if (forced_to_abort())
   {
BOOST_STM_TRANSACTION_INVARIANT;
       // BUG inversion
      deferred_abort();
      throw aborted_transaction_exception
      ("aborting committing transaction due to contention manager priority inversion");
   }
#endif

   //--------------------------------------------------------------------------
   // this is a very important and subtle optimization. if the transaction is
   // only reading memory, it does not need to lock the system. it only needs
   // to lock tx in-flight mutex and remove itself from the tx in flight list
   //--------------------------------------------------------------------------
   if (is_only_reading())
   {
        {
        synchro::unique_lock<Mutex> lk_i(*inflight_lock()  BOOST_STM_CALL_CONTEXT("inflight_lock"));

        BOOST_STM_TRANSACTION_INVARIANT;
        //~ {boost::lock_guard<boost::mutex> lk(log_mutex);
        //~ std::cout << this << " " << this->parent() << " " << this_thread::get_id() << " ****INFO erase" << __FILE__ << "[" << __LINE__ << "]" << BOOST_CURRENT_FUNCTION << std::endl;
        //~ }
        in_flight_transactions().erase(this);

#if PERFORMING_COMPOSITION
        if (other_in_flight_same_thread_transactions())
        {

            state_ = e_hand_off;
            bookkeeping_.inc_handoffs();
        }
        else
#endif
        {
            lk_i.unlock();
            tx_type(eNormalTx);
#if PERFORMING_LATM
            get_tx_conflicting_locks().clear();
            clear_latm_obtained_locks();
#endif
            state_ = e_committed;
        }
        }
        ++global_clock();

        return;
    }

    //while (!synchro::try_lock(*general_lock())) {}
    synchro::lock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
    //synchro::unique_lock<Mutex> lk_g(*general_lock());


   //--------------------------------------------------------------------------
   // as much as I'd like to in_flight_transactions().erase() here, we have
   // to do it inside of abort() because the contention managers must abort
   // the txs and therefore must do a in_flight_transactions().erase(this)
   // anyway. so we actually lose performance by doing it here and then
   // doing it again inside abort()
   //--------------------------------------------------------------------------
   if (forced_to_abort())
   {
      synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock")); //TBR
       //lk.g.unlock();
      deferred_abort();
      throw aborted_transaction_exception
      ("aborting committing transaction due to contention manager priority inversion");
   }
   else
   {
      //-----------------------------------------------------------------------
      // erase this from the inflight transactions so processing through the
      // transactionsInFlight is faster. we have removed the check for "this"
      // when iterating through the transactionsInFlight list. additionally,
      // remember that transactionsInFlight has been removed for commit()
      // so commit doesn't need to remove this from the transactionsInFlight
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      // before we get the transactions in flight mutex - get all the locks for
      // all threads, because aborted threads will try to obtain the
      // transactionsInFlightMutex
      //-----------------------------------------------------------------------
      lock_all_mutexes(); //TBR
       //all_mutexes lk_all(this);
      synchro::lock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));
        //synchro::unique_lock<Mutex> lk_i(*inflight_lock());

#if PERFORMING_COMPOSITION
       // BUG
      if (other_in_flight_same_thread_transactions())
      {
        BOOST_STM_TRANSACTION_INVARIANT;
        {boost::lock_guard<boost::mutex> lk(log_mutex);
        std::cout << this << " " << this->parent() << " " << this_thread::get_id() << " ****INFO erase" << __FILE__ << "[" << __LINE__ << "]" << BOOST_CURRENT_FUNCTION << std::endl;
        }
         in_flight_transactions().erase(this);
         state_ = e_hand_off;
         unlock_all_mutexes();//TBR
         synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));//TBR
         synchro::unlock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));//TBR
         bookkeeping_.inc_handoffs();
      }
      else
#endif
      {
         // commit releases the inflight mutex as soon as its done with it
         invalidating_deferred_commit();
      }

      ++global_clock();
   }
}


//-----------------------------------------------------------------------------
// validating_direct_end_transaction()
//-----------------------------------------------------------------------------
inline void transaction::validating_direct_end_transaction()
{
BOOST_STM_TRANSACTION_INVARIANT;
   synchro::lock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
   synchro::lock(*mutex() BOOST_STM_CALL_CONTEXT("mutex"));

   //--------------------------------------------------------------------------
   // can only be called after above transactionMutex_ is called
   //--------------------------------------------------------------------------
   if (cm_abort_before_commit(*this))
   {
      abort();
      //bookkeeping_.inc_abort_perm_denied(threadId_);
      synchro::unlock(*mutex() BOOST_STM_CALL_CONTEXT("mutex"));
      synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
      throw aborted_transaction_exception
      ("aborting commit due to CM priority");
   }

   //--------------------------------------------------------------------------
   // erase this from the inflight transactions so processing through the
   // transactionsInFlight is faster. we have removed the check for "this"
   // when iterating through the transactionsInFlight list. additionally,
   // remember that transactionsInFlight has been removed for commit()
   // so commit doesn't need to remove this from the transactionsInFlight
   //--------------------------------------------------------------------------
   if (forced_to_abort())
   {
      //-----------------------------------------------------------------------
      // if it wasn't writing, it doesn't need the transactionMutex any more,
      // so unlock it so we can reduce contention
      //-----------------------------------------------------------------------
      bool wasWriting = isWriting() ? true : false;
      if (!wasWriting) synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
      direct_abort();
      synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
      //-----------------------------------------------------------------------
      // if this tx was writing, unlock the transaction mutex now
      //-----------------------------------------------------------------------
      if (wasWriting) synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
      throw aborted_transaction_exception
      ("aborting committing transaction due to contention manager priority inversion");
   }

   lock_all_mutexes_but_this(threadId_);

   synchro::lock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));
        {BOOST_STM_TRANSACTION_INVARIANT;
        {boost::lock_guard<boost::mutex> lk(log_mutex);
        std::cout << this << " " << this->parent() << " " << this_thread::get_id() << " ****INFO erase" << __FILE__ << "[" << __LINE__ << "]" << BOOST_CURRENT_FUNCTION << std::endl;
        }
   in_flight_transactions().erase(this);
        }

   if (other_in_flight_same_thread_transactions())
   {
      state_ = e_hand_off;
      unlock_all_mutexes();
      synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
      synchro::unlock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));
      bookkeeping_.inc_handoffs();
   }
   else
   {
      // commit releases the inflight mutex as soon as its done with it
      validating_direct_commit();

      if (e_committed == state_)
      {
         unlock_all_mutexes();
         synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
         synchro::unlock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));
      }
   }
}

//-----------------------------------------------------------------------------
// validating_deferred_end_transaction()
//-----------------------------------------------------------------------------
inline void transaction::validating_deferred_end_transaction()
{
BOOST_STM_TRANSACTION_INVARIANT;
   synchro::lock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
   synchro::lock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));
   synchro::lock(*mutex() BOOST_STM_CALL_CONTEXT("mutex"));

   //--------------------------------------------------------------------------
   // can only be called after above transactionMutex_ is called
   //--------------------------------------------------------------------------
   if (cm_abort_before_commit(*this))
   {
      //bookkeeping_.inc_abort_perm_denied(threadId_);
      synchro::unlock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));
      synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
      synchro::unlock(*mutex() BOOST_STM_CALL_CONTEXT("mutex"));
      throw aborted_transaction_exception
      ("aborting commit due to CM priority");
   }

   // unlock this - we only needed it to check abort_before_commit()
   synchro::unlock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));

   clock_t ms = clock();

   //--------------------------------------------------------------------------
   // as much as I'd like to in_flight_transactions().erase() here, we have
   // to do it inside of abort() because the contention managers must abort
   // the txs and therefore must do a in_flight_transactions().erase(this)
   // anyway. so we actually lose performance by doing it here and then
   // doing it again inside abort()
   //--------------------------------------------------------------------------
   if (forced_to_abort())
   {
      synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
      deferred_abort();
      synchro::unlock(*mutex() BOOST_STM_CALL_CONTEXT("mutex"));
      throw aborted_transaction_exception
      ("aborting committing transaction due to contention manager priority inversion");
   }
   else
   {
      //--------------------------------------------------------------------------
      // this is a very important and subtle optimization. if the transaction is
      // only reading memory, it does not need to lock the system. it only needs
      // to lock itself and the tx in-flight mutex and remove itself from the
      // tx in flight list
      //--------------------------------------------------------------------------
      if (is_only_reading())
      {
        {boost::lock_guard<boost::mutex> lk(log_mutex);
        std::cout << this << " " << this->parent() << " " << this_thread::get_id() << " ****INFO erase" << __FILE__ << "[" << __LINE__ << "]" << BOOST_CURRENT_FUNCTION << std::endl;
        }
         in_flight_transactions().erase(this);

         if (other_in_flight_same_thread_transactions())
         {
            state_ = e_hand_off;
            bookkeeping_.inc_handoffs();
         }
         else
         {
            tx_type(eNormalTx);
#if PERFORMING_LATM
            get_tx_conflicting_locks().clear();
            clear_latm_obtained_locks();
#endif
            state_ = e_committed;
         }

         synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
         synchro::unlock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));

         bookkeeping_.inc_commits();
#ifndef DISABLE_READ_SETS
         bookkeeping_.pushBackSizeOfReadSetWhenCommitting(readList().size());
#endif
         bookkeeping_.inc_commit_time_ms(clock() - ms);
         synchro::unlock(*mutex() BOOST_STM_CALL_CONTEXT("mutex"));
      }

      //-----------------------------------------------------------------------
      // erase this from the inflight transactions so processing through the
      // transactionsInFlight is faster. we have removed the check for "this"
      // when iterating through the transactionsInFlight list. additionally,
      // remember that transactionsInFlight has been removed for commit()
      // so commit doesn't need to remove this from the transactionsInFlight
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      // before we get the transactions in flight mutex - get all the locks for
      // al threads, because aborted threads will try to obtain the
      // transactionsInFlightMutex
      //-----------------------------------------------------------------------
      lock_all_mutexes_but_this(threadId_);

      synchro::lock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));
      {BOOST_STM_TRANSACTION_INVARIANT;

        {boost::lock_guard<boost::mutex> lk(log_mutex);
        std::cout << this << " " << this->parent() << " " << this_thread::get_id() << " ****INFO erase" << __FILE__ << "[" << __LINE__ << "]" << BOOST_CURRENT_FUNCTION << std::endl;
        }
      in_flight_transactions().erase(this);
      }
      if (other_in_flight_same_thread_transactions())
      {
         state_ = e_hand_off;
         unlock_all_mutexes();
         synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
         synchro::unlock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));
         bookkeeping_.inc_handoffs();
      }
      else
      {
         // commit releases the inflight mutex as soon as its done with it
         validating_deferred_commit();

         // if the commit actually worked, then we can release these locks
         // an exception happened which caused the tx to abort and thus
         // unlocked these already
         if (e_committed == state_)
         {
            //unlock_tx();
            synchro::unlock(*mutex() BOOST_STM_CALL_CONTEXT("mutex"));
            synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
            synchro::unlock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));
         }
      }
   }

   bookkeeping_.inc_commit_time_ms(clock() - ms);
}

////////////////////////////////////////////////////////////////////////////
inline void transaction::forceOtherInFlightTransactionsWritingThisWriteMemoryToAbort()
{
BOOST_STM_TRANSACTION_INVARIANT;
#ifndef ALWAYS_ALLOW_ABORT
   std::list<transaction*> aborted;
#endif
   in_flight_trans_cont::iterator next;

   // iterate through all our written memory
   for (WriteContainer::iterator i = writeList().begin(); writeList().end() != i; ++i)
   {
      // iterate through all the in flight transactions
      for (in_flight_trans_cont::iterator j = in_flight_transactions().begin();
      j != in_flight_transactions().end();)
      {
        BOOST_ASSERT(*j!=0);
        BOOST_STM_TRANSACTION_INVARIANT_VAR(*j);
         transaction *t = *j;

         // if we're already aborting for this transaction, skip it
         if (t->forced_to_abort()) {++j; continue;}

         ///////////////////////////////////////////////////////////////////
         // as only one thread can commit at a time, and we know that
         // composed txs, handoff, rather than commit, we should never be
         // in this loop when another inflight tx has our threadid.
         ///////////////////////////////////////////////////////////////////
         assert(t->threadId_ != this->threadId_);

         ///////////////////////////////////////////////////////////////////
         // if t's modifiedList is modifying memory we are also modifying, make t bail
         ///////////////////////////////////////////////////////////////////
#ifdef USE_BLOOM_FILTER
         if (t->bloom().exists((std::size_t)i->first))
#else
         if (t->writeList().end() != t->writeList().find(i->first))
#endif
         {
#if ALWAYS_ALLOW_ABORT
            t->force_to_abort();

            next = j;
            ++next;
            {BOOST_STM_TRANSACTION_INVARIANT;

        {boost::lock_guard<boost::mutex> lk(log_mutex);
        std::cout << j<< " " << j->parent() << " " << this_thread::get_id() << " ****INFO erase" << __FILE__ << "[" << __LINE__ << "]" << BOOST_CURRENT_FUNCTION << std::endl;
        }
            in_flight_transactions().erase(j);
            }
            j = next;

#else
            if (this->irrevocable())
            {
               aborted.push_front(t);
            }
            else if (!t->irrevocable() && cm_permission_to_abort(*this, *t))
            {
               aborted.push_front(t);
            }
            else
            {
               force_to_abort();
               //bookkeeping_.inc_abort_perm_denied(threadId_);
               throw aborted_transaction_exception
               ("aborting committing transaction due to contention manager priority inversion");
            }
#endif
         }

         ++j;
      }
   }

#ifndef ALWAYS_ALLOW_ABORT
   // ok, forced to aborts are allowed, do them
   for (std::list<transaction*>::iterator k = aborted.begin(); k != aborted.end();)
   {
        BOOST_ASSERT(k!=0);
      (*k)->force_to_abort();
    BOOST_STM_TRANSACTION_INVARIANT;

        {boost::lock_guard<boost::mutex> lk(log_mutex);
        std::cout << *k<< " " << (*k)->parent() << " " << this_thread::get_id() << " ****INFO erase" << __FILE__ << "[" << __LINE__ << "]" << BOOST_CURRENT_FUNCTION << std::endl;
        }
      in_flight_transactions().erase(*k);
   }
#endif
}

///////////////////////////////////////////////////////////////////////////////
//
// Two ways for transactions to end:
//
//    abort() - failed transaction
//    commit() - successful transaction
//
///////////////////////////////////////////////////////////////////////////////
inline void transaction::direct_abort
   (bool const &alreadyRemovedFromInFlight) throw()
{
BOOST_STM_TRANSACTION_INVARIANT;

#if LOGGING_COMMITS_AND_ABORTS
   bookkeeping_.pushBackSizeOfReadSetWhenAborting(readList().size());
   bookkeeping_.pushBackSizeOfWriteSetWhenAborting(writeList().size());
#endif

   try
   {
      state_ = e_aborted;

      directAbortWriteList();
#ifndef DISABLE_READ_SETS
      directAbortReadList();
#endif
      directAbortTransactionDeletedMemory();
      directAbortTransactionNewMemory();

      bloom().clear();
#if PERFORMING_WRITE_BLOOM
      wbloom().clear();
#endif
      if (!alreadyRemovedFromInFlight)
      {
         synchro::lock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));
         // if I'm the last transaction of this thread, reset abort to false
                  BOOST_STM_TRANSACTION_INVARIANT;

        {boost::lock_guard<boost::mutex> lk(log_mutex);
        std::cout << this << " " << this->parent() << " " << this_thread::get_id() << " ****INFO erase" << __FILE__ << "[" << __LINE__ << "]" << BOOST_CURRENT_FUNCTION << std::endl;
        }
         in_flight_transactions().erase(this);
      }

#ifdef USING_SHARED_FORCED_TO_ABORT
      if (!other_in_flight_same_thread_transactions())
      {
         unforce_to_abort();
      }
#else
      unforce_to_abort();
#endif
      if (!alreadyRemovedFromInFlight)
      {
         synchro::unlock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));
      }
   }
   catch (...)
   {
       BOOST_STM_ERROR<<std::endl;
      std::cout << "Exception caught in abort - bad" << std::endl;
   }
}

////////////////////////////////////////////////////////////////////////////
inline void transaction::deferred_abort
   (bool const &alreadyRemovedFromInFlight) throw()
{
BOOST_STM_TRANSACTION_INVARIANT;
#if LOGGING_COMMITS_AND_ABORTS
   bookkeeping_.pushBackSizeOfReadSetWhenAborting(readList().size());
   bookkeeping_.pushBackSizeOfWriteSetWhenAborting(writeList().size());
#endif

   state_ = e_aborted;

{BOOST_STM_TRANSACTION_INVARIANT;   deferredAbortWriteList();}
#ifndef DISABLE_READ_SETS
{BOOST_STM_TRANSACTION_INVARIANT;   deferredAbortReadList();}
#endif

{BOOST_STM_TRANSACTION_INVARIANT;
   deferredAbortTransactionDeletedMemory();
}
{BOOST_STM_TRANSACTION_INVARIANT;
   deferredAbortTransactionNewMemory();
}

   bloom().clear();
#if PERFORMING_WRITE_BLOOM
   wbloom().clear();
#endif

   if (!alreadyRemovedFromInFlight)
   {BOOST_STM_TRANSACTION_INVARIANT;
      synchro::lock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));
      // if I'm the last transaction of this thread, reset abort to false
        {boost::lock_guard<boost::mutex> lk(log_mutex);
        std::cout << this << " " << this->parent() << " " << this_thread::get_id() << " ****INFO erase" << __FILE__ << "[" << __LINE__ << "]" << BOOST_CURRENT_FUNCTION << std::endl;
        }
      in_flight_transactions().erase(this);

#ifdef USING_SHARED_FORCED_TO_ABORT
      if (!other_in_flight_same_thread_transactions())
      {
         unforce_to_abort();
      }
#else
      unforce_to_abort();
#endif

      synchro::unlock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));
   }
   else {
       unforce_to_abort();
   }
}

////////////////////////////////////////////////////////////////////////////
inline void transaction::invalidating_direct_commit()
{
BOOST_STM_TRANSACTION_INVARIANT;
   //--------------------------------------------------------------------------
   // transactionMutex must be obtained before calling commit
   //--------------------------------------------------------------------------
   try
   {
#if LOGGING_COMMITS_AND_ABORTS
      bookkeeping_.pushBackSizeOfReadSetWhenCommitting(readList().size());
      bookkeeping_.pushBackSizeOfWriteSetWhenCommitting(writeList().size());
#endif

      bookkeeping_.inc_commits();

      if (!in_flight_transactions().empty())
      {
         forceOtherInFlightTransactionsReadingThisWriteMemoryToAbort();
      }

      directCommitWriteState();
#ifndef DISABLE_READ_SETS
      directCommitReadState();
#endif
      bookkeeping_.inc_del_mem_commits_by(deletedMemoryList().size());
      directCommitTransactionDeletedMemory();
      bookkeeping_.inc_new_mem_commits_by(newMemoryList().size());
      directCommitTransactionNewMemory();

      tx_type(eNormalTx);
#if PERFORMING_LATM
      get_tx_conflicting_locks().clear();
      clear_latm_obtained_locks();
#endif
      state_ = e_committed;
   }
   //-----------------------------------------------------------------------
   // aborted exceptions can be thrown from the forceOtherInFlight ...
   // if one is thrown it means this transaction was preempted by cm
   //-----------------------------------------------------------------------
   catch (aborted_transaction_exception&)
   {
      unlock_all_mutexes_but_this(threadId_);
      synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
      synchro::unlock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));

      direct_abort();
      synchro::unlock(*mutex() BOOST_STM_CALL_CONTEXT("mutex"));

      throw;
   }
   //-----------------------------------------------------------------------
   // still - what do we do in the event the exception was the commitState()?
   //-----------------------------------------------------------------------
   catch (...)
   {
      unlock_all_mutexes_but_this(threadId_);
      synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
      synchro::unlock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));

      direct_abort();
      synchro::unlock(*mutex() BOOST_STM_CALL_CONTEXT("mutex"));

      throw;
   }
}

////////////////////////////////////////////////////////////////////////////
inline void transaction::invalidating_deferred_commit()
{
BOOST_STM_TRANSACTION_INVARIANT;
   //--------------------------------------------------------------------------
   // transactionMutex must be obtained before calling commit
   //--------------------------------------------------------------------------

   try
   {
#if LOGGING_COMMITS_AND_ABORTS
      bookkeeping_.pushBackSizeOfReadSetWhenCommitting(readList().size());
      bookkeeping_.pushBackSizeOfWriteSetWhenCommitting(writeList().size());
#endif

      //-----------------------------------------------------------------------
      // optimization:
      //
      // whenever multiple transactions are running, if one goes to commit, it
      // never commits, but instead hands-off and exits. thus, from this we
      // know that when a tx is committing it means only one tx for that thread
      // is running.
      //
      // therefore, we can remove our tx from the inflight list at commit time
      // and know that if the in flight tx list is > 1, other threads are
      // currently processing and we must force their memory to abort.
      //
      // this opti gains significant performance improvement since many times
      // only a single tx is processing - so save many cycles from having to
      // walk its read and write list
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      // remember commit can only be called from end_transaction() and
      // end_transaction() has already removed "this" from the
      // transactionsInFlight_ set
      //-----------------------------------------------------------------------
      // commit() already has the transactionsInFlightMutex

      if (in_flight_transactions().size() > 1)
      {
         static int stalling_ = 0;

         bool wait = stalling_ * stalls_ < global_clock();
         transaction *stallingOn = 0;
         //int iter = 1;

#if USE_BLOOM_FILTER
         while (!forceOtherInFlightTransactionsAccessingThisWriteMemoryToAbort(wait, stallingOn))
         {
            ++stalling_;
            clock_t local_clock = global_clock();

            synchro::unlock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));
            synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
            unlock_all_mutexes();

            for (;;)
            {
               while (local_clock == global_clock())// && sleepTime < maxSleep)
               {
                  SLEEP(1);
               }

               if (forced_to_abort())
               {
                  --stalling_;
                  deferred_abort();
                  throw aborted_transaction_exception_no_unlocks();
               }

               synchro::lock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
               synchro::lock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));

               // if our stalling on tx is gone, continue
               if (in_flight_transactions().end() == in_flight_transactions().find(stallingOn))
               {
                  --stalling_;
                  wait = stalling_ * stalls_ < global_clock();
                  //std::cout << "stalling : stalls : commits: " << stalling_ << " : " << stalls_ << " : " << global_clock() << std::endl;
                  //set_priority(priority() + reads());
                  break;
               }

               synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
               synchro::unlock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));
            }

            lock_all_mutexes();

            if (forced_to_abort())
            {
               deferred_abort();
               unlock_all_mutexes(); // i added this after the fact, is this right?
               throw aborted_transaction_exception
               ("aborting committing transaction due to contention manager priority inversion");
            }
         }
#else
         forceOtherInFlightTransactionsWritingThisWriteMemoryToAbort();
         forceOtherInFlightTransactionsReadingThisWriteMemoryToAbort();
#endif
      }

      //BUG
      //~ in_flight_transactions().erase(this);
      //~ synchro::unlock(*inflight_lock());
        {boost::lock_guard<boost::mutex> lk(log_mutex);
        std::cout << this << " " << this->parent() << " " << this_thread::get_id() << " ****INFO erase" << __FILE__ << "[" << __LINE__ << "]" << BOOST_CURRENT_FUNCTION << std::endl;
        }
      in_flight_transactions().erase(this);
      synchro::unlock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));
      synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));

      deferredCommitWriteState();

      if (!newMemoryList().empty())
      {
         bookkeeping_.inc_new_mem_commits_by(newMemoryList().size());
         deferredCommitTransactionNewMemory();
      }

      //-----------------------------------------------------------------------
      // if the commit actually worked, then we can release these locks
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      // once our write state is committed and our new memory has been cleared,
      // we can allow the other threads to make forward progress ... so unlock
      // them all now
      //-----------------------------------------------------------------------
      unlock_all_mutexes();

      if (!deletedMemoryList().empty())
      {
         bookkeeping_.inc_del_mem_commits_by(deletedMemoryList().size());
         deferredCommitTransactionDeletedMemory();
      }

      // finally set the state to committed
      bookkeeping_.inc_commits();
      tx_type(eNormalTx);
#if PERFORMING_LATM
      get_tx_conflicting_locks().clear();
      clear_latm_obtained_locks();
#endif
      state_ = e_committed;
   }
   //-----------------------------------------------------------------------
   // aborted exceptions can be thrown from the forceOtherInFlight ...
   // if one is thrown it means this transaction was preempted by cm
   //-----------------------------------------------------------------------
   catch (aborted_transaction_exception_no_unlocks&)
   {
      throw aborted_transaction_exception("whatever");
   }
   catch (aborted_transaction_exception&)
   {
      unlock_all_mutexes_but_this(threadId_);
      synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
      synchro::unlock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));
      deferred_abort();
      synchro::unlock(*mutex() BOOST_STM_CALL_CONTEXT("mutex"));

      throw;
   }
   //-----------------------------------------------------------------------
   // copy constructor failures can cause ..., catch, unlock and re-throw
   //-----------------------------------------------------------------------
   catch (...)
   {
      unlock_all_mutexes_but_this(threadId_);
      synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
      synchro::unlock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));
      deferred_abort();
      synchro::unlock(*mutex() BOOST_STM_CALL_CONTEXT("mutex"));

      throw;
   }
}


#if PERFORMING_VALIDATION
////////////////////////////////////////////////////////////////////////////
inline void transaction::validating_direct_commit()
{
BOOST_STM_TRANSACTION_INVARIANT;
   throw not_implemented("not implemented yet: validating_direct_commit";






   //--------------------------------------------------------------------------
   // transactionMutex must be obtained before calling commit
   //--------------------------------------------------------------------------
   try
   {
#if LOGGING_COMMITS_AND_ABORTS
      bookkeeping_.pushBackSizeOfReadSetWhenCommitting(readList().size());
      bookkeeping_.pushBackSizeOfWriteSetWhenCommitting(writeList().size());
#endif

      bookkeeping_.inc_commits();

      if (!in_flight_transactions().empty())
      {
         forceOtherInFlightTransactionsReadingThisWriteMemoryToAbort();
      }

      directCommitWriteState();
      directCommitReadState();
      bookkeeping_.inc_del_mem_commits_by(deletedMemoryList().size());
      directCommitTransactionDeletedMemory();
      bookkeeping_.inc_new_mem_commits_by(newMemoryList().size());
      directCommitTransactionNewMemory();

      //~ tx_type_ptr() = eNormalTx;
      tx_type(eNormalTx);
#if PERFORMING_LATM
      get_tx_conflicting_locks().clear();
      /* BACKTRACE
#2  0x00459309 in std::set<__pthread_mutex_t**, std::less<__pthread_mutex_t**>,
std::allocator<__pthread_mutex_t**> >::clear (this=0x0)
    at /usr/lib/gcc/i686-pc-cygwin/3.4.4/include/c++/bits/stl_set.h:413
#3  0x0041e8c1 in boost::stm::transaction::invalidating_deferred_commit (
    this=0x191ccc40) at ../../../boost/stm/detail/transaction_impl.hpp:1593
#4  0x0041f707 in boost::stm::transaction::invalidating_deferred_end_transaction
 (this=0x191ccc40) at ../../../boost/stm/detail/transaction_impl.hpp:1002
#5  0x0041fa0c in boost::stm::transaction::end (this=0x191ccc40)
    at ../../../boost/stm/detail/transaction_impl.hpp:796
#6  0x0040142e in account_withdraw_thr ()
    at C:\cygwin\sandbox\stm\branches\vbe\libs\stm\example\bank.cpp:144
      */
      clear_latm_obtained_locks();
#endif
      state_ = e_committed;
   }
   //-----------------------------------------------------------------------
   // aborted exceptions can be thrown from the forceOtherInFlight ...
   // if one is thrown it means this transaction was preempted by cm
   //-----------------------------------------------------------------------
   catch (aborted_transaction_exception&)
   {
      unlock_all_mutexes_but_this(threadId_);
      synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
      synchro::unlock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));

      direct_abort();
      synchro::unlock(*mutex() BOOST_STM_CALL_CONTEXT("mutex"));

      throw;
   }
   //-----------------------------------------------------------------------
   // still - what do we do in the event the exception was the commitState()?
   //-----------------------------------------------------------------------
   catch (...)
   {
      unlock_all_mutexes_but_this(threadId_);
      synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
      synchro::unlock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));

      direct_abort();
      synchro::unlock(*mutex() BOOST_STM_CALL_CONTEXT("mutex"));

      throw;
   }
}
#endif

#if PERFORMING_VALIDATION
////////////////////////////////////////////////////////////////////////////
inline void transaction::validating_deferred_commit()
{
BOOST_STM_TRANSACTION_INVARIANT;
   //--------------------------------------------------------------------------
   // transactionMutex must be obtained before calling commit
   //--------------------------------------------------------------------------

   try
   {
#if LOGGING_COMMITS_AND_ABORTS
      bookkeeping_.pushBackSizeOfReadSetWhenCommitting(readList().size());
      bookkeeping_.pushBackSizeOfWriteSetWhenCommitting(writeList().size());
#endif

      //-----------------------------------------------------------------------
      // optimization:
      //
      // whenever multiple transactions are running, if one goes to commit, it
      // never commits, but instead hands-off and exits. thus, from this we
      // know that when a tx is committing it means only one tx for that thread
      // is running.
      //
      // therefore, we can remove our tx from the inflight list at commit time
      // and know that if the in flight tx list is > 1, other threads are
      // currently processing and we must force their memory to abort.
      //
      // this opti gains significant performance improvement since many times
      // only a single tx is processing - so save many cycles from having to
      // walk its read and write list
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      // remember commit can only be called from end_transaction() and
      // end_transaction() has already removed "this" from the
      // transactionsInFlight_ set
      //-----------------------------------------------------------------------
      // commit() already has the transactionsInFlightMutex

      verifyReadMemoryIsValidWithGlobalMemory();
      verifyWrittenMemoryIsValidWithGlobalMemory();

      if (this->isWriting())
      {
         deferredCommitWriteState();
      }

      if (!newMemoryList().empty())
      {
         bookkeeping_.inc_new_mem_commits_by(newMemoryList().size());
         deferredCommitTransactionNewMemory();
      }

      //-----------------------------------------------------------------------
      // once our write state is committed and our new memory has been cleared,
      // we can allow the other threads to make forward progress ... so unlock
      // them all now
      //-----------------------------------------------------------------------
      unlock_all_mutexes_but_this(threadId_);

      if (!deletedMemoryList().empty())
      {
         bookkeeping_.inc_del_mem_commits_by(deletedMemoryList().size());
         deferredCommitTransactionDeletedMemory();
      }

      // finally set the state to committed
      bookkeeping_.inc_commits();
      //~ tx_type_ptr() = eNormalTx;
      tx_type(eNormalTx);
#if PERFORMING_LATM
      get_tx_conflicting_locks().clear();
      clear_latm_obtained_locks();
#endif
      state_ = e_committed;
   }

   //-----------------------------------------------------------------------
   // aborted exceptions can be thrown from the forceOtherInFlight ...
   // if one is thrown it means this transaction was preempted by cm
   //-----------------------------------------------------------------------
   catch (aborted_transaction_exception&)
   {
      unlock_all_mutexes_but_this(threadId_);
      synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
      synchro::unlock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));
      deferred_abort();
      synchro::unlock(*mutex() BOOST_STM_CALL_CONTEXT("mutex"));

      throw;
   }
   //-----------------------------------------------------------------------
   // copy constructor failures can cause ..., catch, unlock and re-throw
   //-----------------------------------------------------------------------
   catch (...)
   {
      unlock_all_mutexes_but_this(threadId_);
      synchro::unlock(*general_lock() BOOST_STM_CALL_CONTEXT("general_lock"));
      synchro::unlock(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));
      deferred_abort();
      synchro::unlock(*mutex() BOOST_STM_CALL_CONTEXT("mutex"));

      throw;
   }
}
#endif

////////////////////////////////////////////////////////////////////////////
inline void transaction::unlockAllLockedThreads(LockedTransactionContainer &l)
{
BOOST_STM_TRANSACTION_INVARIANT;
   for (LockedTransactionContainer::iterator i = l.begin(); i != l.end(); ++i) {
        BOOST_ASSERT(*i!=0);

       (*i)->unlock_tx();
   }
}

////////////////////////////////////////////////////////////////////////////
inline void transaction::directAbortWriteList()
{
BOOST_STM_TRANSACTION_INVARIANT;
   //--------------------------------------------------------------------------
   // copy the newObject into the oldObject, updating the real data back to
   // what it was before we changed it
   //--------------------------------------------------------------------------
   for (WriteContainer::iterator i = writeList().begin(); writeList().end() != i; ++i)
   {
      //-----------------------------------------------------------------------
      // i->second == 0 will happen when a transaction has added a piece of
      // memory to its writeList_ that it is deleting (not writing to).
      // Thus, when seeing 0 as the second value, it signifies that this
      // memory is being destroyed, not updated. Do not perform copy_cache()
      // on it.
      //
      // However, deleted memory MUST reset its invalid_thread_id()
      // transaction_thread (which is performed in    void directAbortTransactionDeletedMemory() throw();

      //-----------------------------------------------------------------------
      if (0 == i->second) continue;

        BOOST_ASSERT(i->first!=0);

      if (using_move_semantics()) i->first->move_state(i->second);
      else i->first->copy_cache(*i->second);
      i->first->transaction_thread(invalid_thread_id());

      cache_release(i->second);
   }

   writeList().clear();
}

////////////////////////////////////////////////////////////////////////////
inline void transaction::directAbortTransactionDeletedMemory() throw()
{
BOOST_STM_TRANSACTION_INVARIANT;
   for (MemoryContainerList::iterator j = deletedMemoryList().begin();
   j != deletedMemoryList().end(); ++j)
   {
      detail::reset(*j);
   }

   deletedMemoryList().clear();
}

////////////////////////////////////////////////////////////////////////////
inline void transaction::deferredAbortWriteList() throw()
{
BOOST_STM_TRANSACTION_INVARIANT;
   for (WriteContainer::iterator i = writeList().begin(); writeList().end() != i; ++i)
   {
      //BUGdelete
       cache_release(i->second); // delete all the temporary memory
   }

   //BUGdelete
   writeList().clear();
}

//----------------------------------------------------------------------------
inline clock_t transaction::earliest_start_time_of_inflight_txes()
{
BOOST_STM_TRANSACTION_INVARIANT;
   synchro::lock_guard<Mutex> a(*inflight_lock() BOOST_STM_CALL_CONTEXT("inflight_lock"));

   clock_t secs = 0xffffffff;

   for (in_flight_trans_cont::iterator j = in_flight_transactions().begin();
   j != in_flight_transactions().end(); ++j)
   {
        BOOST_ASSERT(*j!=0);
        BOOST_STM_TRANSACTION_INVARIANT_VAR(*j);
      transaction *t = *j;
      //-----------------------------------------------------------------------
      // since this is called while direct_writes are occurring, the transaction
      // calling it will be inflight, so we have to check for it and skip it
      //-----------------------------------------------------------------------
      if (t->startTime_ < secs) secs = t->startTime_;
   }

   return secs;
}

//----------------------------------------------------------------------------
inline void transaction::doIntervalDeletions()
{
BOOST_STM_TRANSACTION_INVARIANT;
   using namespace boost::stm;

   clock_t earliestInFlightTx = earliest_start_time_of_inflight_txes();

   synchro::lock_guard<Mutex> a(deletionBufferMutex_ BOOST_STM_CALL_CONTEXT("deletionBufferMutex_"));

   for (DeletionBuffer::iterator i = deletionBuffer_.begin(); i != deletionBuffer_.end();)
   {
      if (earliestInFlightTx > i->first)
      {
         for (MemoryContainerList::iterator j = i->second.begin(); j != i->second.end(); ++j)
         {
            detail::release(*j);
            // delete *j;
         }
         deletionBuffer_.erase(i);
         i = deletionBuffer_.begin();
      }
      // getting to the else means there is an inflight
      // tx that is older than the first entry to delete,
      // so exit, since no other deletions will succeed
      else break;
   }
}

//----------------------------------------------------------------------------
inline void transaction::directCommitTransactionDeletedMemory() throw()
{
BOOST_STM_TRANSACTION_INVARIANT;
   using namespace boost::stm;

   if (!deletedMemoryList().empty())
   {
      synchro::lock_guard<Mutex> a(deletionBufferMutex_ BOOST_STM_CALL_CONTEXT("deletionBufferMutex_"));
      deletionBuffer_.insert( std::pair<clock_t, MemoryContainerList>
         (time(0), deletedMemoryList()) );
      deletedMemoryList().clear();
   }
}

//----------------------------------------------------------------------------
inline void transaction::deferredCommitTransactionDeletedMemory() throw()
{
BOOST_STM_TRANSACTION_INVARIANT;
   for (MemoryContainerList::iterator i = deletedMemoryList().begin();
      i != deletedMemoryList().end(); ++i)
   {
      detail::release(*i);
      //delete *i;
   }

   deletedMemoryList().clear();
}

////////////////////////////////////////////////////////////////////////////
inline void transaction::deferredAbortTransactionNewMemory() throw()
{
BOOST_STM_TRANSACTION_INVARIANT;
   for (MemoryContainerList::iterator i = newMemoryList().begin(); i != newMemoryList().end(); ++i)
   {
       BOOST_ASSERT(*i);
       //BUGdelete
       detail::release(*i);
      //delete *i;
   }

   //BUGdelete
   newMemoryList().clear();
}

////////////////////////////////////////////////////////////////////////////
inline void transaction::deferredCommitTransactionNewMemory()
{
BOOST_STM_TRANSACTION_INVARIANT;
   for (MemoryContainerList::iterator i = newMemoryList().begin(); i != newMemoryList().end(); ++i)
   {
       BOOST_ASSERT(*i);
      detail::reset(*i);
   }

   newMemoryList().clear();
}

////////////////////////////////////////////////////////////////////////////
inline void transaction::directCommitWriteState()
{
BOOST_STM_TRANSACTION_INVARIANT;
   // direct commit for writes just deletes the backup and changes global memory
   // so its no longer flagged as being transactional
   for (WriteContainer::iterator i = writeList().begin(); writeList().end() != i; ++i)
   {
      //-----------------------------------------------------------------------
      // i->second == 0 will happen when a transaction has added a piece of
      // memory to its writeList_ that it is deleting (not writing to).
      // Thus, when seeing 0 as the second value, it signifies that this
      // memory is being destroyed, not updated. Do not perform copyState()
      // on it.
      //-----------------------------------------------------------------------
        BOOST_ASSERT(i->first!=0);

      i->first->transaction_thread(invalid_thread_id());
      i->first->new_memory(0);

      //-----------------------------------------------------------------------
      // it is true that i->second can be null, but C++ allows delete on null
      // faster to just always delete
      //-----------------------------------------------------------------------
      //delete i->second;
      cache_release(i->second); // delete all the temporary memory
   }

   writeList().clear();
}

////////////////////////////////////////////////////////////////////////////
inline void transaction::deferredCommitWriteState()
{
BOOST_STM_TRANSACTION_INVARIANT;
   // copy the newObject into the oldObject, updating the real data
   for (WriteContainer::iterator i = writeList().begin(); writeList().end() != i; ++i)
   {
      //-----------------------------------------------------------------------
      // i->second == 0 will happen when a transaction has added a piece of
      // memory to its writeList_ that it is deleting (not writing to).
      // Thus, when seeing 0 as the second value, it signifies that this
      // memory is being destroyed, not updated. Do not perform copyState()
      // on it.
      //-----------------------------------------------------------------------
      if (0 == i->second)
      {
         continue;
      }

        BOOST_ASSERT(i->first!=0);

      if (using_move_semantics()) i->first->move_state(i->second);
      else i->first->copy_cache(*i->second);

      i->first->transaction_thread(invalid_thread_id());
      i->first->new_memory(0);

#if PERFORMING_VALIDATION
      i->first->version_++;
#endif
      cache_release(i->second);
   }

   writeList().clear();
}

#if PERFORMING_VALIDATION
//----------------------------------------------------------------------------
inline void transaction::verifyReadMemoryIsValidWithGlobalMemory()
{
BOOST_STM_TRANSACTION_INVARIANT;
   // copy the newObject into the oldObject, updating the real data
   for (ReadContainer::iterator i = readList().begin(); readList().end() != i; ++i)
   {
        BOOST_ASSERT(i->first!=0);

      if (i->first->version_ != i->second)
      {
         bookkeeping_.inc_read_aborts();
         throw aborted_transaction_exception
         ("aborting committing transaction due to invalid read");
      }
   }
}

//----------------------------------------------------------------------------
inline void transaction::verifyWrittenMemoryIsValidWithGlobalMemory()
{
BOOST_STM_TRANSACTION_INVARIANT;
   // copy the newObject into the oldObject, updating the real data
   for (WriteContainer::iterator i = writeList().begin(); writeList().end() != i; ++i)
   {
        BOOST_ASSERT(i->first!=0);

      if (0 == i->second) continue;
      if (i->first->version_ != i->second->version_)
      {
         bookkeeping_.inc_write_aborts();
         throw aborted_transaction_exception
         ("aborting committing transaction due to invalid write");
      }
   }
}
#endif

////////////////////////////////////////////////////////////////////////////
inline bool transaction::otherInFlightTransactionsWritingThisMemory(base_transaction_object *obj)
{
BOOST_STM_TRANSACTION_INVARIANT;
   // iterate through all the in flight transactions
   for (in_flight_trans_cont::iterator j = in_flight_transactions().begin();
   j != in_flight_transactions().end(); ++j)
   {
        BOOST_ASSERT(*j!=0);
        BOOST_STM_TRANSACTION_INVARIANT_VAR(*j);
      transaction *t = *j;
      //-----------------------------------------------------------------------
      // since this is called while direct_writes are occurring, the transaction
      // calling it will be inflight, so we have to check for it and skip it
      //-----------------------------------------------------------------------
      if (t->threadId_ == this->threadId_) continue;

      // if we're already aborting for this transaction, skip it
      if (!t->isWriting()) continue;
      if (t->forced_to_abort()) continue;

      // if t's writing to this memory return true
      if (t->writeList().end() != t->writeList().find(obj)) return true;
   }

   return false;
}

////////////////////////////////////////////////////////////////////////////
inline bool transaction::forceOtherInFlightTransactionsAccessingThisWriteMemoryToAbort
      (bool allow_stall, transaction* &stallingOn)
{
BOOST_STM_TRANSACTION_INVARIANT;
   static std::list<transaction*> aborted;
   //static bool initialized = false;

   if (writes() > 3) allow_stall = false;

   //--------------------------------------------------------------------------
   // FOR THE TIME BEING, DO NOT ALLOW STALLS AT ALL! Stalls somehow cause
   // Sebastian's account code to break ... we need to investigate why and fix it.
   //
   // Until such time, stalling txes for increased concurrency MUST be disabled.
   //--------------------------------------------------------------------------

   allow_stall = false;

   //--------------------------------------------------------------------------
   // FOR THE TIME BEING, DO NOT ALLOW STALLS AT ALL! Stalls somehow cause
   // Sebastian's account code to break ... we need to investigate why and fix it.
   //
   // Until such time, stalling txes for increased concurrency MUST be disabled.
   //--------------------------------------------------------------------------


   // warm up the cache with this transaction's bloom filter
#if PERFORMING_WRITE_BLOOM
   bloom_filter &wbloom = this->wbloom();
   bool singleWriteComp = writeList().size() < 20;
#else
   bool const singleWriteComp = true;
#endif

   // iterate through all the in flight transactions
   for (in_flight_trans_cont::iterator j = in_flight_transactions().begin();
   j != in_flight_transactions().end(); ++j)
   {
        BOOST_ASSERT(*j!=0);
        BOOST_STM_TRANSACTION_INVARIANT_VAR(*j);
      transaction *t = *j;
      // if we're already aborting for this transaction, skip it
      if (t->forced_to_abort()) continue;

      //////////////////////////////////////////////////////////////////////
      // as only one thread can commit at a time, and we know that
      // composed txs, handoff, rather than commit, we should never be
      // in this loop when another inflight tx has our threadid.
      //////////////////////////////////////////////////////////////////////
      if (t->threadId_ == this->threadId_) continue;

      if (singleWriteComp)
      {
         // iterate through all our written memory
         for (WriteContainer::iterator i = writeList().begin(); writeList().end() != i; ++i)
         {
            //////////////////////////////////////////////////////////////////////
            // if t's readList is reading memory we are modifying, make t bail
            //////////////////////////////////////////////////////////////////////
            if (t->bloom().exists((std::size_t)i->first))
            {
               if (allow_stall && t->is_only_reading())// && t->reads() > work)
               {
                  ++stalls_;
                  stallingOn = t;
                  return false;
               }
               // if the conflict is not a write-write conflict, stall
#if 0
               if (allow_stall && !t->wbloom().exists((std::size_t)i->first))
               {
                  ++stalls_;
                  stallingOn = t;
                  return false;
               }
#endif
#if PERFORMING_LATM
               if (this->irrevocable())
               {
                  aborted.push_front(t);
               }
               else if (!t->irrevocable() && cm_permission_to_abort(*this, *t))
               {
                  aborted.push_front(t);
               }
               else
               {
                  force_to_abort();
                  throw aborted_transaction_exception
                  ("aborting committing transaction due to contention manager priority inversion");
               }
#else
               aborted.push_front(t);
#endif
            }
         }
      }
#if PERFORMING_WRITE_BLOOM
      else if (wbloom.intersection(t->bloom()))
      {
         if (allow_stall && t->is_only_reading())// && t->reads() > work)
         {
            ++stalls_;
            stallingOn = t;
            return false;
         }
#if PERFORMING_LATM
         if (this->irrevocable())
         {
            aborted.push_front(t);
         }
         else if (!t->irrevocable() && cm_permission_to_abort(*this, *t))
         {
            aborted.push_front(t);
         }
         else
         {
            force_to_abort();
            throw aborted_transaction_exception
            ("aborting committing transaction due to contention manager priority inversion");
         }
#else
         aborted.push_front(t);
#endif
      }
#endif
   }

   if (!aborted.empty())
   {
      if (cm_permission_to_abort(*this, aborted))
      {
         // ok, forced to aborts are allowed, do them
         for (std::list<transaction*>::iterator k = aborted.begin();
              k != aborted.end(); ++k)
         {
        BOOST_ASSERT(*k!=0);

            (*k)->force_to_abort();
         }

         aborted.clear();
      }
      else
      {
         force_to_abort();
         throw aborted_transaction_exception
         ("aborting committing transaction by contention manager");
      }
   }

   return true;
}

////////////////////////////////////////////////////////////////////////////
inline void transaction::forceOtherInFlightTransactionsReadingThisWriteMemoryToAbort()
{
BOOST_STM_TRANSACTION_INVARIANT;
   std::list<transaction*> aborted;

   // iterate through all the in flight transactions
   for (in_flight_trans_cont::iterator j = in_flight_transactions().begin();
   j != in_flight_transactions().end(); ++j)
   {
        BOOST_ASSERT(*j!=0);
        BOOST_STM_TRANSACTION_INVARIANT_VAR(*j);
      transaction *t = *j;
      // if we're already aborting for this transaction, skip it
#ifndef DISABLE_READ_SETS
      if (!t->isReading()) continue;
#endif
      if (t->forced_to_abort()) continue;

      //////////////////////////////////////////////////////////////////////
      // as only one thread can commit at a time, and we know that
      // composed txs, handoff, rather than commit, we should never be
      // in this loop when another inflight tx has our threadid.
      //////////////////////////////////////////////////////////////////////
      assert(t->threadId_ != this->threadId_);

      // iterate through all our written memory
      for (WriteContainer::iterator i = writeList().begin(); writeList().end() != i; ++i)
      {

         //////////////////////////////////////////////////////////////////////
         // if t's readList is reading memory we are modifying, make t bail
         //////////////////////////////////////////////////////////////////////
#ifdef USE_BLOOM_FILTER
         if (t->bloom().exists((std::size_t)i->first))
#else
         if (t->readList().end() != t->readList().find(i->first))
#endif
         {
            if (this->irrevocable())
            {
               aborted.push_front(t);
            }
            else if (!t->irrevocable() && cm_permission_to_abort(*this, *t))
            {
               aborted.push_front(t);
            }
            else
            {
               force_to_abort();
               //bookkeeping_.inc_abort_perm_denied(threadId_);
               throw aborted_transaction_exception
               ("aborting committing transaction due to contention manager priority inversion");
            }
         }
      }
   }

   // ok, forced to aborts are allowed, do them
   for (std::list<transaction*>::iterator k = aborted.begin(); k != aborted.end(); ++k)
   {
        BOOST_ASSERT(*k!=0);

      (*k)->force_to_abort();
      //bookkeeping_.inc_abort_perm_denied((*k)->threadId_);
   }
}

//-----------------------------------------------------------------------------
// IMPORTANT ASSUMPTION:
//
// "this" must not be on the list. Thus, otherInFlightTransactionsOfSameThread
// must be checked BEFORE "this" is inserted into the transactionsInFlight_ if
// checked at ctor or begin_transaction().
//
// Or it must be checked AFTER "this" is removed from the transactionsInFlight_
//
//-----------------------------------------------------------------------------
inline bool transaction::other_in_flight_same_thread_transactions() const throw()
{
BOOST_STM_TRANSACTION_INVARIANT;
   for (in_flight_trans_cont::iterator i = in_flight_transactions().begin();
      i != in_flight_transactions().end(); ++i)
   {
        BOOST_ASSERT(*i!=0);
        BOOST_STM_TRANSACTION_INVARIANT_VAR(*i);
      if (*i == this) continue;
      if (*i == 0) continue;
      // if this is not our threadId or this thread is not composable, skip it
      if ((*i)->threadId_ != this->threadId_) continue;
      return true;
   }

   return false;
}

inline bool transaction::otherInFlightTransactionsOfSameThreadNotIncludingThis(transaction const * const rhs)
{
BOOST_STM_TRANSACTION_INVARIANT;
   //////////////////////////////////////////////////////////////////////
   for (in_flight_trans_cont::iterator i = in_flight_transactions().begin(); i != in_flight_transactions().end(); ++i)
   {
        BOOST_ASSERT(*i!=0);
        BOOST_STM_TRANSACTION_INVARIANT_VAR(*i);
      if (*i == rhs) continue;
      // if this is not our threadId or this thread is not composable, skip it
      if ((*i)->threadId_ != this->threadId_) continue;
      return true;
   }

   return false;
}

}}
#endif // TRANSACTION_IMPL_H

