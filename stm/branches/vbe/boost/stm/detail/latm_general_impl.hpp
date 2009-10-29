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
// TransactionLockAwareImpl.h
//
// This file contains method implementations for transaction.hpp (specifically for
// enabling lock aware transactions). The main purpose of this file is to reduce
// the complexity of the transaction class by separating its implementation into
// a secondary .h file.
//
// Do NOT place these methods in a .cc/.cpp/.cxx file. These methods must be
// inlined to keep DracoSTM performing fast. If these methods are placed in a
// C++ source file they will incur function call overhead - something we need
// to reduce to keep performance high.
//
//-----------------------------------------------------------------------------
#ifndef BOOST_STM_TRANSACTION_LOCK_AWARE_GENERAL_IMPL_H
#define BOOST_STM_TRANSACTION_LOCK_AWARE_GENERAL_IMPL_H


#if PERFORMING_LATM
namespace boost { namespace stm {

//-----------------------------------------------------------------------------
//
//
//
//                            GENERAL LATM INTERFACES
//
//
//
//-----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
inline void boost::stm::transaction::wait_until_all_locks_are_released(bool keepLatmLocked)
{
   while (true)
   {
      synchro::lock(*latm_lock());
      if (latm::instance().latmLockedLocks_.empty()) break;
      synchro::unlock(*latm_lock());
      SLEEP(10);
   }

   if (!keepLatmLocked) synchro::unlock(*latm_lock());
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
inline void boost::stm::transaction::add_to_obtained_locks(latm::mutex_type* m)
{
   obtainedLocksRef().insert(m);

#if LOGGING_BLOCKS
   logFile_ << "----------------------\ntx has obtained mutex: " << m << endl << endl;
#endif
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
inline bool boost::stm::transaction::is_on_obtained_locks_list(latm::mutex_type* m)
{
   return obtainedLocksRef().find(m) != obtainedLocksRef().end();
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
inline bool boost::stm::transaction::is_currently_locked_lock(latm::mutex_type* m)
{
   return currentlyLockedLocksRef().find(m) != currentlyLockedLocksRef().end();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
inline void boost::stm::transaction::add_to_currently_locked_locks(latm::mutex_type* m)
{
   currentlyLockedLocksRef().insert(m);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
inline void boost::stm::transaction::remove_from_currently_locked_locks(latm::mutex_type* m)
{
   currentlyLockedLocksRef().erase(m);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
inline void boost::stm::transaction::clear_latm_obtained_locks()
{
   for (latm::mutex_set::iterator i = obtainedLocksRef().begin(); i != obtainedLocksRef().end();)
   {
      latm::mutex_type* m = *i;
      obtainedLocksRef().erase(i);
      i = obtainedLocksRef().begin();

#if LOGGING_BLOCKS
      logFile_ << "----------------------\nbefore tx release unlocked mutex: " << m << endl << endl;
      logFile_ << outputBlockedThreadsAndLockedLocks() << endl;
#endif

      unblock_conflicting_threads(m);

#if LOGGING_BLOCKS
      logFile_ << "----------------------\nafter tx release unlocked mutex: " << m << endl << endl;
      logFile_ << outputBlockedThreadsAndLockedLocks() << endl;
#endif
   }

   unblock_threads_if_locks_are_empty();

   currentlyLockedLocksRef().clear();
}

//----------------------------------------------------------------------------
//
// PRE-CONDITION: latm_lock(), general_lock() and inflight_lock() are obtained
//                prior to calling this method.
//
//----------------------------------------------------------------------------
inline bool boost::stm::transaction::mutex_is_on_obtained_tx_list(latm::mutex_type* mutex)
{
#ifdef BOOST_STM_HAVE_SINGLE_TSS_CONTEXT_MAP
   for (tss_context_map_type::iterator iter = tss_context_map_.begin();
   tss_context_map_.end() != iter; ++iter)
   {
      if (iter->second->obtainedLocks_.find(mutex) != iter->second->obtainedLocks_.end())
      {
         return true;
      }
   }
#else
   for (latm::thread_id_mutex_set_map::iterator iter = threadObtainedLocks_.begin();
   threadObtainedLocks_.end() != iter; ++iter)
   {
      if (iter->second->find(mutex) != iter->second->end())
      {
         return true;
      }
   }
#endif
   return false;
}

//----------------------------------------------------------------------------
//
// PRE-CONDITION: latm_lock(), general_lock() and inflight_lock() are obtained
//                prior to calling this method.
//
//----------------------------------------------------------------------------
inline void boost::stm::transaction::unblock_conflicting_threads(latm::mutex_type* mutex)
{
   // if the mutex is on the latm locks map, we can't unblock yet
   if (latm::instance().latmLockedLocksAndThreadIdsMap_.find(mutex) != latm::instance().latmLockedLocksAndThreadIdsMap_.end())
   {
#if LOGGING_BLOCKS
      logFile_ << "\ncannot unlock <" << mutex << ">, in latmLockedLocksAndThreadIdsMap_" << endl << endl;
#endif
      return;
   }

   // if the mutex is in any tx threads, we can't unblock yet
   if (mutex_is_on_obtained_tx_list(mutex))
   {
#if LOGGING_BLOCKS
      logFile_ << "\ncannot unlock <" << mutex << ">, in mutex_is_on_obtained_tx_list" << endl << endl;
#endif
      return;
   }

   thread_conflicting_mutexes_set_all_cnd(mutex, false);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
inline void boost::stm::transaction::unblock_threads_if_locks_are_empty()
{
#if 0
   // if the size is 0, unblock everybody
   if (latm::instance().latmLockedLocksOfThreadMap_.empty())
   {
      for (latm::thread_id_mutex_set_map::iterator it = threadObtainedLocks_.begin();
      it != threadObtainedLocks_.end(); ++it)
      {
         if (!it->second->empty()) return;
      }
      thread_conflicting_mutexes_set_all(false);
   }
#endif
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

inline void boost::stm::transaction::tm_lock_conflict(latm::mutex_type* inLock)
{
#if 1
    latm::instance().tm_lock_conflict(inLock);
#else    
   if (!latm::instance().doing_tm_lock_protection()) return;

   //synchro::lock(latmMutex_);
   synchro::lock_guard<Mutex> lock_l(latmMutex_);

   //-------------------------------------------------------------------------
   // insert can throw an exception
   //-------------------------------------------------------------------------
   //try { 
       tmConflictingLocks_.insert(inLock); 
    //}
   //catch (...)
   //{
     // synchro::unlock(latmMutex_);
      //throw;
   //}
   //synchro::unlock(latmMutex_);
#endif   
}

#if 0

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
inline void boost::stm::transaction::clear_tm_conflicting_locks()
{
   synchro::lock_guard<Mutex> lock_l(latmMutex_);
   //synchro::lock(latmMutex_);
   tmConflictingLocks_.clear();
   //synchro::unlock(latmMutex_);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
inline void boost::stm::transaction::must_be_in_tm_conflicting_lock_set(latm::mutex_type* inLock)
{
   if (tmConflictingLocks_.find(inLock) == tmConflictingLocks_.end())
   {
      throw "lock not in tx conflict lock set, use add_tm_conflicting_lock";
   }
}
#endif

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
inline void boost::stm::transaction::must_be_in_conflicting_lock_set(latm::mutex_type* inLock)
{
   if (get_tx_conflicting_locks().find(inLock) == get_tx_conflicting_locks().end())
   {
      throw "lock not in tx conflict lock set, use add_tx_conflicting_lock";
   }
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
inline void boost::stm::transaction::add_tx_conflicting_lock(latm::mutex_type* inLock)
{
   if (!latm::instance().doing_tx_lock_protection()) return;

   {
      synchro::lock_guard<Mutex> autolock_l(*latm_lock());
      synchro::lock_guard<Mutex> autolock_g(*general_lock());
      synchro::lock_guard<Mutex> autolock_i(*inflight_lock());

      if (get_tx_conflicting_locks().find(inLock) != get_tx_conflicting_locks().end()) return;
      get_tx_conflicting_locks().insert(inLock);

      if (irrevocable()) return;

      see_if_tx_must_block_due_to_tx_latm();
   }

   if (blocked())
   {
      lock_and_abort();
      throw aborted_transaction_exception("aborting transaction");
   }
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
inline void boost::stm::transaction::clear_tx_conflicting_locks()
{
   synchro::lock_guard<Mutex> lock_l(*general_lock());
   //synchro::lock(*general_lock());
   get_tx_conflicting_locks().clear();
   //synchro::unlock(*general_lock());
}

//----------------------------------------------------------------------------
//
// Exposed client interfaces that act as forwarding calls to the real
// implementation based on the specific type of lock-aware transactions
// the client chose
//
//----------------------------------------------------------------------------
inline void boost::stm::transaction::pthread_lock(latm::mutex_type* mutex)
{
   //using namespace boost::stm;

   switch (latm::instance().protection())
   {
   case eFullLatmProtection:
      if (direct_updating())  {dir_full_pthread_lock_mutex(mutex); return;}
      else {def_full_pthread_lock_mutex(mutex);return;}
   case eTmConflictingLockLatmProtection:
      if (direct_updating()) {dir_tm_conflicting_lock_pthread_lock_mutex(mutex);return;}
      else {def_tm_conflicting_lock_pthread_lock_mutex(mutex);return;}
   case eTxConflictingLockLatmProtection:
      if (direct_updating()) {dir_tx_conflicting_lock_pthread_lock_mutex(mutex);return;}
      else {def_tx_conflicting_lock_pthread_lock_mutex(mutex);return;}
   default:
      throw "invalid LATM type";
   }
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
#if 0
inline int boost::stm::transaction::pthread_trylock(latm::mutex_type* mutex)
{
   //using namespace boost::stm;

   switch (latm::instance().protection())
   {
   case eFullLatmProtection:
      if (direct_updating()) return dir_full_pthread_trylock_mutex(mutex);
      else return def_full_pthread_trylock_mutex(mutex);
   case eTmConflictingLockLatmProtection:
      if (direct_updating()) return dir_tm_conflicting_lock_pthread_trylock_mutex(mutex);
      else return def_tm_conflicting_lock_pthread_trylock_mutex(mutex);
   case eTxConflictingLockLatmProtection:
      if (direct_updating()) return dir_tx_conflicting_lock_pthread_trylock_mutex(mutex);
      else return def_tx_conflicting_lock_pthread_trylock_mutex(mutex);
   default:
      throw "invalid LATM type";
   }
}
#endif
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
inline void boost::stm::transaction::pthread_unlock(latm::mutex_type* mutex)
{
   //using namespace boost::stm;

   switch (latm::instance().protection())
   {
   case eFullLatmProtection:
      if (direct_updating())  {dir_full_pthread_unlock_mutex(mutex);return;}
      else {def_full_pthread_unlock_mutex(mutex);return;}
   case eTmConflictingLockLatmProtection:
      if (direct_updating()) {dir_tm_conflicting_lock_pthread_unlock_mutex(mutex);return;}
      else {def_tm_conflicting_lock_pthread_unlock_mutex(mutex); return;}
   case eTxConflictingLockLatmProtection:
      if (direct_updating()) {dir_tx_conflicting_lock_pthread_unlock_mutex(mutex);return;}
      else {def_tx_conflicting_lock_pthread_unlock_mutex(mutex);return;}
   default:
      throw "invalid LATM type";
   }
}

//-----------------------------------------------------------------------------
//
//
//
//                TRANSACTION CONFLICTING LOCK LATM PROTECTION METHODS
//
//
//
//-----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//
// ASSUMPTION: latmMutex_ MUST BE OBTAINED BEFORE CALLING THIS METHOD
//
//----------------------------------------------------------------------------
inline void boost::stm::transaction::see_if_tx_must_block_due_to_tx_latm()
{
   //--------------------------------------------------------------------------
   // iterate through all currently locked locks
   //--------------------------------------------------------------------------
   for (latm::mutex_thread_id_set_map::iterator iter = latm::instance().latmLockedLocksAndThreadIdsMap_.begin();
   latm::instance().latmLockedLocksAndThreadIdsMap_.end() != iter; ++iter)
   {
      //-----------------------------------------------------------------------
      // iterate through this transaction's conflicting mutex ref - if one of
      // the currently locked locks is in this tx's conflicting mutex set,
      // we need to block this tx
      //-----------------------------------------------------------------------
      if (get_tx_conflicting_locks().find(iter->first) != get_tx_conflicting_locks().end())
      {
         this->block(); break;
      }
   }

   block_if_conflict_mutex();

   for (latm::mutex_set::iterator k = get_tx_conflicting_locks().begin(); k != get_tx_conflicting_locks().end(); ++k)
   {
      // if it is locked by our thread, it is ok ... otherwise it is not
      latm::mutex_thread_id_map::iterator l = latm::instance().latmLockedLocksOfThreadMap_.find(*k);

      if (l != latm::instance().latmLockedLocksOfThreadMap_.end() &&
         this_thread::get_id() != l->second)
      {
         latm::mutex_thread_id_set_map::iterator locksAndThreadsIter = latm::instance().latmLockedLocksAndThreadIdsMap_.find(*k);

         if (locksAndThreadsIter == latm::instance().latmLockedLocksAndThreadIdsMap_.end())
         {
            ThreadIdSet s;
            s.insert(this_thread::get_id());

            latm::instance().latmLockedLocksAndThreadIdsMap_.insert
            (std::make_pair<latm::mutex_type*, ThreadIdSet>(*k, s));
         }
         else
         {
            locksAndThreadsIter->second.insert(this_thread::get_id());
         }

         this->block(); break;
      }
   }

}

//----------------------------------------------------------------------------
//
// ASSUMPTION: latmMutex_ MUST BE OBTAINED BEFORE CALLING THIS METHOD
//
//----------------------------------------------------------------------------
inline int boost::stm::transaction::
thread_id_occurance_in_locked_locks_map(thread_id_t threadId)
{
   int count = 0;

   for (latm::mutex_thread_id_set_map::iterator iter = latm::instance().latmLockedLocksAndThreadIdsMap_.begin();
      latm::instance().latmLockedLocksAndThreadIdsMap_.end() != iter; ++iter)
   {
      if (iter->second.find(threadId) != iter->second.end()) ++count;
   }

   return count;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
inline boost::stm::transaction* boost::stm::transaction::get_inflight_tx_of_same_thread
(bool hasTxInFlightMutex)
{
   synchro::lock_guard_if<Mutex> lock_l(*general_lock(), !hasTxInFlightMutex);
   //if (!hasTxInFlightMutex) synchro::lock(*inflight_lock());

   for (InflightTxes::iterator i = transactionsInFlight_.begin();
      i != transactionsInFlight_.end(); ++i)
   {
      transaction *t = (transaction*)*i;

      //--------------------------------------------------------------------
      // if this tx's thread is the same thread iterating through the in-flight
      // txs, then this lock is INSIDE this tx - don't abort the tx, just
      // make it isolated and ensure it is performing direct updating
      //--------------------------------------------------------------------
      if (t->thread_id() == this_thread::get_id())
      {
         //if (!hasTxInFlightMutex) synchro::unlock(*inflight_lock());
         return t;
      }
   }

   //if (!hasTxInFlightMutex) synchro::unlock(*inflight_lock());
   return 0;
}
}}

#include <boost/stm/detail/latm_def_full_impl.hpp>
#include <boost/stm/detail/latm_dir_full_impl.hpp>

#endif

#endif // TRANSACTION_LOCK_AWARE_GENERAL_IMPL_H

