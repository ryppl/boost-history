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
#ifndef BOOST_STM_TRANSACTION_LOCK_AWARE_DEF_IMPL_H
#define BOOST_STM_TRANSACTION_LOCK_AWARE_DEF_IMPL_H


#if PERFORMING_LATM

#include <boost/stm/detail/latm_def_tx_impl.hpp>
#include <boost/stm/detail/latm_def_tm_impl.hpp>

namespace boost { namespace stm {

//-----------------------------------------------------------------------------
//
//
//
//                         FULL LATM PROTECTION METHODS
//
//
//
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
inline bool transaction::def_do_core_full_pthread_lock_mutex
(latm_mutex mutex, int lockWaitTime, int lockAborted)
{
   //--------------------------------------------------------------------------
   // if the lock-aware tm lock set is empty, lock the in-flight trans mutex
   // so we can abort all the in-flight transactions
   //--------------------------------------------------------------------------
   if (latmLockedLocks_.empty())
   {
      synchro::lock_guard<Mutex> lock_g(*general_lock());
      //synchro::lock(*general_lock());
      synchro::lock_guard<Mutex> lock_i(*inflight_lock());
      //synchro::lock(*inflight_lock());

      std::list<transaction*> txList;
      for (InflightTxes::iterator i = transactionsInFlight_.begin();
         i != transactionsInFlight_.end(); ++i)
      {
         transaction *t = (transaction*)*i;

         if (!t->irrevocable() &&
            cm_allow_lock_to_abort_tx(lockWaitTime, lockAborted, false, *t))
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

      for (std::list<transaction*>::iterator it = txList.begin(); txList.end() != it; ++it)
      {
         (*it)->force_to_abort();
      }

      //synchro::unlock(*general_lock());
      //synchro::unlock(*inflight_lock());
   }

   try { latmLockedLocks_.insert(mutex); }
   catch (...)
   {
      synchro::unlock(*inflight_lock());
      throw;
   }

   return true;
}

//----------------------------------------------------------------------------
// only allow one thread to execute any of these methods at a time
//----------------------------------------------------------------------------
inline int transaction::def_full_pthread_lock_mutex(latm_mutex mutex)
{
   if (transaction* t = get_inflight_tx_of_same_thread(false))
   {
      t->make_isolated();

      bool hadLock = t->is_currently_locked_lock(mutex);
      t->add_to_currently_locked_locks(mutex);
      t->add_to_obtained_locks(mutex);

      t->commit_deferred_update_tx();
       { synchro::lock_guard<Mutex> lock_l(*latm_lock());
      //synchro::lock(*latm_lock());
      latmLockedLocksOfThreadMap_[mutex] = this_thread::get_id();
      //synchro::unlock(*latm_lock());
       }

      // TBR if (hadLock) return 0;
      // TBR else return lock(mutex);
      if (hadLock) return 0;
      else synchro::lock(*mutex);
      return 0;
   }

   int waitTime = 0, aborted = 0;
   for (;;)
   {
      // TBR int val = lock(mutex);
      // TBR if (0 != val) return val;
      //{
      synchro::lock(*mutex);
      //synchro::lock_guard<Mutex> lock_m(*mutex);
      synchro::lock(latmMutex_);
      //synchro::lock_guard<Mutex> lock_l(latmMutex_);
      try
      {
         //--------------------------------------------------------------------
         // if we are able to do the core lock work, break
         //--------------------------------------------------------------------
         if (def_do_core_full_pthread_lock_mutex(mutex, waitTime, aborted)) break;
      }
      catch (...)
      {
         synchro::unlock(*mutex);
         synchro::unlock(latmMutex_);
         throw;
      }

      //-----------------------------------------------------------------------
      // we weren't able to do the core lock work, unlock our mutex and sleep
      //-----------------------------------------------------------------------
      synchro::unlock(*mutex);
      synchro::unlock(latmMutex_);
      //}

      SLEEP(cm_lock_sleep_time());
      waitTime += cm_lock_sleep_time();
      ++aborted;
   }

   latmLockedLocksOfThreadMap_[mutex] = this_thread::get_id();
   synchro::unlock(latmMutex_);
   return 0;
}

//----------------------------------------------------------------------------
// only allow one thread to execute any of these methods at a time
//----------------------------------------------------------------------------
inline int transaction::def_full_pthread_trylock_mutex(latm_mutex mutex)
{
   if (transaction* t = get_inflight_tx_of_same_thread(false))
   {
      t->make_isolated();

      bool hadLock = t->is_currently_locked_lock(mutex);
      t->add_to_currently_locked_locks(mutex);
      t->add_to_obtained_locks(mutex);

      t->commit_deferred_update_tx();
       { synchro::lock_guard<Mutex> lock_l(*latm_lock());
      //synchro::lock(*latm_lock());
      latmLockedLocksOfThreadMap_[mutex] = this_thread::get_id();
      //synchro::unlock(*latm_lock());
       }

      if (hadLock) return 0;
      else return synchro::try_lock(*mutex)?0:1;
   }

   //int val = synchro::trylock(*mutex);
   //if (0 != val) return val;
   if (!synchro::try_lock(*mutex)) return 1;

   //synchro::lock(latmMutex_);
    synchro::lock_guard<Mutex> lock_l(latmMutex_);

   try
   {
      //-----------------------------------------------------------------------
      // if !core done, since trylock, we cannot stall & retry - just exit
      //-----------------------------------------------------------------------
      if (!def_do_core_full_pthread_lock_mutex(mutex, 0, 0))
      {
         synchro::unlock(*mutex);
         //synchro::unlock(latmMutex_);
         return -1;
      }
   }
   catch (...)
   {
      synchro::unlock(*mutex);
      //synchro::unlock(latmMutex_);
      throw;
   }

   latmLockedLocksOfThreadMap_[mutex] = this_thread::get_id();
   //synchro::unlock(latmMutex_);
   // note: we do not release the transactionsInFlightMutex - this will prevents
   // new transactions from starting until this lock is released
   return 0;
}

//----------------------------------------------------------------------------
// only allow one thread to execute any of these methods at a time
//----------------------------------------------------------------------------
inline int transaction::def_full_pthread_unlock_mutex(latm_mutex mutex)
{
   bool hasLock = true;
   { synchro::lock_guard<Mutex> lock_l(latmMutex_);
    //synchro::lock(latmMutex_);

   if (transaction* t = get_inflight_tx_of_same_thread(false))
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

   latmLockedLocks_.erase(mutex);

   if (latmLockedLocks_.empty()) synchro::unlock(*inflight_lock());

   latmLockedLocksOfThreadMap_.erase(mutex);
   //synchro::unlock(latmMutex_);
   }

   // TBR if (hasLock) return unlock(mutex);
   // TBR else return 0;
   if (hasLock) synchro::unlock(*mutex);
   return 0;
}

}}
#endif

#endif // TRANSACTION_IMPL_H

