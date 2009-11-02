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
// LatmDirTxImlp.h
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
#ifndef BOOST_STM_LATM_DIR_TX_IMPL_H
#define BOOST_STM_LATM_DIR_TX_IMPL_H

#if PERFORMING_LATM

#include <fstream>


namespace boost { namespace stm {


//----------------------------------------------------------------------------
//
// ASSUMPTION: latmMutex_ MUST BE OBTAINED BEFORE CALLING THIS METHOD
//
//----------------------------------------------------------------------------
inline bool transaction::dir_do_core_tx_conflicting_lock_pthread_lock_mutex
(latm::mutex_type* mutex, int lockWaitTime, int lockAborted, bool txIsIrrevocable)
{
   //--------------------------------------------------------------------------
   // see if this mutex is part of any of the in-flight transactions conflicting
   // mutex set. if it is, stop that transaction and add it to the latm conflicting
   // set. do not keep in-flight transactions blocked once the transactions have
   // been processed.
   //--------------------------------------------------------------------------
   //synchro::lock(*general_lock());
    synchro::unique_lock<Mutex> lk_g(*general_lock());
   //synchro::lock(*inflight_lock());
    synchro::unique_lock<Mutex> lk_i(*inflight_lock());

   std::list<transaction *> txList;
   std::set<thread_id_t> txThreadId;
   //transaction *txToMakeIsolated = 0;

   for (InflightTxes::iterator i = transactionsInFlight_.begin();
      i != transactionsInFlight_.end(); ++i)
   {
      transaction *t = (transaction*)*i;

      // if this tx is part of this thread, skip it (it's an LiT)
      if (t->threadId_ == this_thread::get_id()) continue;

      if (t->get_tx_conflicting_locks().find(mutex) != t->get_tx_conflicting_locks().end())
      {
         if (txIsIrrevocable || (!t->irrevocable() &&
            cm_allow_lock_to_abort_tx(lockWaitTime, lockAborted, txIsIrrevocable, *t)))
         {
            txList.push_back(t);
         }
         else
         {
            //synchro::unlock(*general_lock());
            //synchro::unlock(*inflight_lock());
            return false;
         }
      }
   }

   if (!txList.empty())
   {
      for (std::list<transaction*>::iterator it = txList.begin(); txList.end() != it; ++it)
      {
         transaction *t = (transaction*)*it;

         t->force_to_abort();
         t->block();
         txThreadId.insert(t->threadId_);
      }

      try
      {
         latm::instance().latmLockedLocksAndThreadIdsMap_.insert
         (std::make_pair<latm::mutex_type*, ThreadIdSet>(mutex, txThreadId));
         latm::instance().latmLockedLocksOfThreadMap_[mutex] = this_thread::get_id();
      }
      catch (...)
      {
         for (std::set<thread_id_t>::iterator it = txThreadId.begin();
         txThreadId.end() != it; ++it)
         {
            if (0 == thread_id_occurance_in_locked_locks_map(*it))
            {
               blocked(*it) = false;
            }
         }
         throw;
      }

      //synchro::unlock(*general_lock());
      lk_g.unlock();
      //synchro::unlock(*inflight_lock());
      lk_i.unlock();

      //-----------------------------------------------------------------------
      // now wait until all the txs which conflict with this mutex are no longer
      // in-flight
      //-----------------------------------------------------------------------
      for (;;)
      {
         bool conflictingTxInFlight = false;

          {
         //synchro::lock(*general_lock());
         synchro::unique_lock<Mutex> lk_g(*general_lock());
         //synchro::lock(*inflight_lock());
         synchro::unique_lock<Mutex> lk_i(*inflight_lock());

         for (InflightTxes::iterator i = transactionsInFlight_.begin();
            i != transactionsInFlight_.end(); ++i)
         {
            transaction *t = (transaction*)*i;

            if (t->get_tx_conflicting_locks().find(mutex) != t->get_tx_conflicting_locks().end())
            {
               conflictingTxInFlight = true;
            }
         }

         //synchro::unlock(*general_lock());
         //synchro::unlock(*inflight_lock());
        }
         if (conflictingTxInFlight) SLEEP(10);
         else return true;
      }
   } else {

   //synchro::unlock(*general_lock());
   //synchro::unlock(*inflight_lock());

   return true;
   }
}

//----------------------------------------------------------------------------
// only allow one thread to execute any of these methods at a time
//----------------------------------------------------------------------------
// Protected by: mutex is locked 
// Postcondition: mutex is locked 
//----------------------------------------------------------------------------
inline void transaction::dir_tx_conflicting_lock_pthread_lock_mutex(latm::mutex_type* mutex)
{
   int waitTime = 0, aborted = 0;

   //--------------------------------------------------------------------------
   // this is the most complex code in the entire system. it is highly order
   // dependent.
   //
   // if an in-flight transaction is present when this lock is attempted to be
   // obtained:
   //
   // (1) make sure the lock is in the conflicting set, otherwise throw
   // (2) make the tx irrevocable - THIS MUST BE DONE BEFORE THE LOCK IS OBTAINED
   //                               otherwise the system can deadlock by obtaining
   //                               the lock and then failing to become irrevocable
   //                               while another irrevocable tx needs the lock we
   //                               took.
   // (3) obtain the lock
   // (4) add the lock to the tx's obtained locked list ONLY after it has been
   //     obtained. adding it before the lock is obtained can lead to deadlock
   //     as another thread which is releasing the lock will not unblock txs
   //     that it has blocked due to our claiming to have obtained the lock
   // (5) abort all the in-flight conflicting txes and return
   //--------------------------------------------------------------------------
   if (transaction* t = get_inflight_tx_of_same_thread(false))
   {
      t->must_be_in_conflicting_lock_set(mutex);
      t->make_irrevocable();

      if (!t->is_currently_locked_lock(mutex))
      {
         // TBR if (0 != lock(mutex)) return -1;
         synchro::lock(*mutex);
      }

      t->add_to_currently_locked_locks(mutex);
      t->add_to_obtained_locks(mutex);

      //synchro::lock(latm::instance().latmMutex_);
      synchro::lock_guard<Mutex> lk_l(latm::instance().latmMutex_);
      def_do_core_tx_conflicting_lock_pthread_lock_mutex(mutex, 0, 0, true);
      //synchro::unlock(latm::instance().latmMutex_);

      return;
   }

   for (;;)
   {
      // TBR int val = lock(mutex);
      // TBR if (0 != val) return val;
      {//synchro::lock(*mutex);
      synchro::unique_lock<latm::mutex_type> lk(*mutex);

      //synchro::lock(latm::instance().latmMutex_);
      synchro::lock_guard<Mutex> lk_l(latm::instance().latmMutex_);

      //try
      //{
         //--------------------------------------------------------------------
         // if we are able to do the core lock work, break
         //--------------------------------------------------------------------
         if (dir_do_core_tx_conflicting_lock_pthread_lock_mutex
            (mutex, waitTime, aborted, false)) {
            latm::instance().latmLockedLocksOfThreadMap_[mutex] = this_thread::get_id();
            lk.release();
            return;                
         }
      //}
      //catch (...)
      //{
         //synchro::unlock(*mutex);
         //synchro::unlock(latm::instance().latmMutex_);
      //   throw;
      //}

      //-----------------------------------------------------------------------
      // we weren't able to do the core lock work, unlock our mutex and sleep
      //-----------------------------------------------------------------------
      //synchro::unlock(*mutex);
      //synchro::unlock(latm::instance().latmMutex_);
      }
      SLEEP(cm_lock_sleep_time());
      waitTime += cm_lock_sleep_time();
      ++aborted;
   }

   //latm::instance().latmLockedLocksOfThreadMap_[mutex] = this_thread::get_id();
   //synchro::unlock(latm::instance().latmMutex_);

   // note: we do not release the transactionsInFlightMutex - this will prevents
   // new transactions from starting until this lock is released
   return;
}

//----------------------------------------------------------------------------
// only allow one thread to execute any of these methods at a time
//----------------------------------------------------------------------------
inline bool transaction::dir_tx_conflicting_lock_pthread_trylock_mutex(latm::mutex_type* mutex)
{
   //--------------------------------------------------------------------------
   throw "might not be possible to implement trylock for this";

   bool txIsIrrevocable = false;

   //int val = synchro::try_lock(*mutex);
   //if (0 != val) return val;
   synchro::unique_lock<latm::mutex_type> lk(*mutex, synchro::try_to_lock);
   if (!lk) return false;
   //if (!synchro::try_lock(*mutex)) return false;

   //synchro::lock(latm::instance().latmMutex_);
   synchro::lock_guard<Mutex> lk_l(latm::instance().latmMutex_);

   if (transaction* t = get_inflight_tx_of_same_thread(false))
   {
      txIsIrrevocable = true;
      t->must_be_in_conflicting_lock_set(mutex);
      t->make_irrevocable();
      t->add_to_obtained_locks(mutex);
   }

   //try
   //{
      //-----------------------------------------------------------------------
      // if !core done, since trylock, we cannot stall & retry - just exit
      //-----------------------------------------------------------------------
      if (!dir_do_core_tx_conflicting_lock_pthread_lock_mutex(mutex, 0, 0, txIsIrrevocable))
      {
         //synchro::unlock(*mutex);
         //synchro::unlock(latm::instance().latmMutex_);
         //return -1;
         return false;
      }
   //}
   //catch (...)
   //{
      //synchro::unlock(*mutex);
      //synchro::unlock(latm::instance().latmMutex_);
   //   throw;
   //}

   latm::instance().latmLockedLocksOfThreadMap_[mutex] = this_thread::get_id();
   //synchro::unlock(latm::instance().latmMutex_);
   lk.release();

   // note: we do not release the transactionsInFlightMutex - this will prevents
   // new transactions from starting until this lock is released
   return true;
}

//----------------------------------------------------------------------------
// only allow one thread to execute any of these methods at a time
//----------------------------------------------------------------------------
inline void transaction::dir_tx_conflicting_lock_pthread_unlock_mutex(latm::mutex_type* mutex)
{
   synchro::lock_guard<Mutex> lk_l(*latm_lock());
   synchro::lock_guard<Mutex> lk_g(*general_lock());
   synchro::lock_guard<Mutex> lk_i(*inflight_lock());
   bool hasLock = true;

   if (transaction* t = get_inflight_tx_of_same_thread(true))
   {
      if (!t->is_on_obtained_locks_list(mutex))
      {
         // this is illegal, it means the transaction is unlocking a lock
         // it did not obtain (e.g., early release) while the transaction
         // is still in-flight. Throw exception
         throw "lock released for transaction that did not obtain it";
      }

      if (!t->is_currently_locked_lock(mutex)) hasLock = false;
      t->remove_from_currently_locked_locks(mutex);
   }

   //--------------------------------------------------------------------------
   // if this mutex is on the tmConflictingLocks_ set, then we need to remove
   // it from the latmLocks and any txs on the full thread list that are
   // blocked because of this lock being locked should be unblocked
   //--------------------------------------------------------------------------
   if (latm::instance().latmLockedLocksAndThreadIdsMap_.find(mutex) != latm::instance().latmLockedLocksAndThreadIdsMap_.end())
   {
      latm::instance().latmLockedLocksAndThreadIdsMap_.erase(mutex);
      unblock_conflicting_threads(mutex);
   }

   latm::instance().latmLockedLocksOfThreadMap_.erase(mutex);
   unblock_threads_if_locks_are_empty();

   // TBR if (hasLock) return unlock(mutex);
   // TBR else return 0;
   if (hasLock) synchro::unlock(*mutex);
   return;
}
}}

#endif

#endif // LATM_DIR_TX_IMPL_H

