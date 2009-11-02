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
// LatmDefTmImlp.h
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
#ifndef BOOST_STM_LATM_DEF_TM_IMPL_H
#define BOOST_STM_LATM_DEF_TM_IMPL_H


#if PERFORMING_LATM
namespace boost { namespace stm {

//-----------------------------------------------------------------------------
//
//
//
//                  GLOBAL CONFLICTING LOCK LATM PROTECTION METHODS
//
//
//
//-----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//
// ASSUMPTION: latmMutex_ MUST BE OBTAINED BEFORE CALLING THIS METHOD
//
//----------------------------------------------------------------------------
inline bool transaction::def_do_core_tm_conflicting_lock_pthread_lock_mutex
(latm::mutex_type* mutex, int lockWaitTime, int lockAborted)
{
   //--------------------------------------------------------------------------
   // if this mutex is on the tmConflictingLocks_ set, then we need to stop
   // transactions which are in-flight.
   //
   // if it isn't we don't interrupt any in-flight transactions
   //--------------------------------------------------------------------------
   if (latm::instance().tmConflictingLocks_.find(mutex) != latm::instance().tmConflictingLocks_.end())
   {
      // if the lock-aware tm lock set is empty, lock the in-flight trans mutex
      // so we can abort all the in-flight transactions
      if (latm::instance().latmLockedLocks_.empty())
      {
         synchro::lock_guard<Mutex> lk_g(*general_lock());
         synchro::lock_guard<Mutex> lk_i(*inflight_lock());

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
            else return false;
         }

         for (std::list<transaction*>::iterator it = txList.begin(); txList.end() != it; ++it)
         {
            (*it)->force_to_abort();
         }
      }

      latm::instance().latmLockedLocks_.insert(mutex);
   }

   return true;
}

//----------------------------------------------------------------------------
// only allow one thread to execute any of these methods at a time
//----------------------------------------------------------------------------
inline void transaction::def_tm_conflicting_lock_pthread_lock_mutex(latm::mutex_type* mutex)
{
   if (transaction* t = get_inflight_tx_of_same_thread(false))
   {
      latm::instance().must_be_in_tm_conflicting_lock_set(mutex);
      t->make_isolated();
      t->commit_deferred_update_tx();

      bool hadLock = t->is_currently_locked_lock(mutex);
      t->add_to_currently_locked_locks(mutex);

      {
          synchro::lock_guard<Mutex> lk_l(*latm_lock());
      //synchro::lock(*latm_lock());
      latm::instance().latmLockedLocksOfThreadMap_[mutex] = this_thread::get_id();
      //synchro::unlock(*latm_lock());
      }

      if (hadLock) return;
      else synchro::lock(*mutex);
      return;
   }

   int waitTime = 0, aborted = 0;

   for (;;)
   {
       {
       //synchro::lock(*mutex);
         synchro::unique_lock<latm::mutex_type> lk(*mutex);
           
       //synchro::lock(latm::instance().latmMutex_);
       synchro::lock_guard<Mutex> lk_l(*latm_lock());

      //try
      //{
         //--------------------------------------------------------------------
         // if we are able to do the core lock work, break
         //--------------------------------------------------------------------
         if (def_do_core_tm_conflicting_lock_pthread_lock_mutex
            (mutex, waitTime, aborted)) {
                latm::instance().latmLockedLocksOfThreadMap_[mutex] = this_thread::get_id();
                //synchro::unlock(latm::instance().latmMutex_);
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

   return;
}

//----------------------------------------------------------------------------
// only allow one thread to execute any of these methods at a time
//----------------------------------------------------------------------------
inline bool transaction::def_tm_conflicting_lock_pthread_trylock_mutex(latm::mutex_type* mutex)
{
   if (transaction* t = get_inflight_tx_of_same_thread(false))
   {
      latm::instance().must_be_in_tm_conflicting_lock_set(mutex);
      t->make_isolated();
      t->commit_deferred_update_tx();

      bool hadLock = t->is_currently_locked_lock(mutex);
      t->add_to_currently_locked_locks(mutex);

      {
      synchro::lock_guard<Mutex> lk_l(*latm_lock());
      //synchro::lock(*latm_lock());
      latm::instance().latmLockedLocksOfThreadMap_[mutex] = this_thread::get_id();
      //synchro::unlock(*latm_lock());
      }

      if (hadLock) return true;
      else return synchro::try_lock(*mutex);
   }
  
   //if (!synchro::try_lock(*mutex)) return false;
   synchro::unique_lock<latm::mutex_type> lk(*mutex, synchro::try_to_lock);
   if (!lk) return false;

   //synchro::lock(latmMutex_);
   synchro::lock_guard<Mutex> lk_l(latm::instance().latmMutex_);

   //try
   //{
      //-----------------------------------------------------------------------
      // if !core done, since trylock, we cannot stall & retry - just exit
      //-----------------------------------------------------------------------
      if (!def_do_core_tm_conflicting_lock_pthread_lock_mutex(mutex, 0, 0))
      {
         //synchro::unlock(*mutex);
         //synchro::unlock(latmMutex_);
         //return -1;
         return false;
      }
   //}
   //catch (...)
   //{
      //synchro::unlock(*mutex);
      //synchro::unlock(latmMutex_);
   //   throw;
   //}

   latm::instance().latmLockedLocksOfThreadMap_[mutex] = this_thread::get_id();
   //synchro::unlock(latmMutex_);
   // note: we do not release the transactionsInFlightMutex - this will prevents
   // new transactions from starting until this lock is released
   lk.release();
   return true;
}

//----------------------------------------------------------------------------
// only allow one thread to execute any of these methods at a time
//----------------------------------------------------------------------------
inline void transaction::def_tm_conflicting_lock_pthread_unlock_mutex(latm::mutex_type* mutex)
{
   bool hasLock = true;
   {
   synchro::lock_guard<Mutex> lk_l(latm::instance().latmMutex_);
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

   //--------------------------------------------------------------------------
   // if this mutex is on the tmConflictingLocks_ set, then we need to remove
   // it from the latmLocks and check to see if we allow transactions to
   // continue.
   //--------------------------------------------------------------------------
   if (latm::instance().tmConflictingLocks_.find(mutex) != latm::instance().tmConflictingLocks_.end())
   {
      latm::instance().latmLockedLocks_.erase(mutex);

      if (latm::instance().latmLockedLocks_.empty()) synchro::unlock(*inflight_lock());
   }

   latm::instance().latmLockedLocksOfThreadMap_.erase(mutex);
   //synchro::unlock(latmMutex_);
   }

   if (hasLock) synchro::unlock(*mutex);
   return;
}

}}
#endif

#endif // LATM_DIR_TM_IMPL_H


