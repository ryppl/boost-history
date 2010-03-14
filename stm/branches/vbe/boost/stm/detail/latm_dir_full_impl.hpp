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
#ifndef BOOST_STM_TRANSACTION_LOCK_AWARE_DIR_IMPL_H
#define BOOST_STM_TRANSACTION_LOCK_AWARE_DIR_IMPL_H


#if PERFORMING_LATM

#include <boost/stm/detail/latm_dir_tx_impl.hpp>
#include <boost/stm/detail/latm_dir_tm_impl.hpp>

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
inline bool transaction::dir_do_core_full_pthread_lock_mutex
(latm::mutex_type* mutex, int lockWaitTime, int lockAborted)
{
    BOOST_STM_CALL_CONTEXT_DCL_INST(0);

   //--------------------------------------------------------------------------
   // if the lock-aware tm lock set is empty, lock the in-flight trans mutex
   // so we can abort all the in-flight transactions
   //--------------------------------------------------------------------------
   if (latm::instance().latmLockedLocks_.empty())
   {

        {
        synchro::lock_guard<Mutex> lk_g(*general_lock()  BOOST_STM_CALL_CONTEXT("general_lock"));
        synchro::lock_guard<Mutex> lk_i(*inflight_lock()  BOOST_STM_CALL_CONTEXT("inflight_lock"));

        std::list<transaction*> txList;
        for (in_flight_trans_cont::iterator i = in_flight_transactions().begin();
            i != in_flight_transactions().end(); ++i)
        {
            BOOST_ASSERT(*i!=0);
            //~ (*i)->assert_tx_type();
            transaction *t = *i;
            BOOST_STM_TRANSACTION_INVARIANT_VAR(t);

            if (!t->irrevocable() &&
                cm_allow_lock_to_abort_tx(lockWaitTime, lockAborted, false, *t))
            {
                txList.push_back(t);
            }
            else
            {
                return false;
            }
        }

        for (std::list<transaction*>::iterator it = txList.begin(); txList.end() != it; ++it)
        {
            BOOST_ASSERT(*it!=0);

            (*it)->force_to_abort();
        }

        //-----------------------------------------------------------------------
        // block all threads, since we have to unlock the in-flight mutex
        //-----------------------------------------------------------------------
        thread_conflicting_mutexes_set_all(true);

        }

        //-----------------------------------------------------------------------
        // now we must stall until all in-flight transactions are gone, otherwise
        // global memory may still be in an inconsistent state
        //-----------------------------------------------------------------------
        while (!in_flight_transactions().empty()) { SLEEP(10); }
    }

    try {
        latm::instance().latmLockedLocks_.insert(mutex); }
    catch (...)
    {
       // BUG ???
      //synchro::unlock(*general_lock());
      //synchro::unlock(*inflight_lock());
       // BUG ???
        throw;
    }

    return true;
}

//----------------------------------------------------------------------------
// only allow one thread to execute any of these methods at a time
//----------------------------------------------------------------------------
template <typename M>
inline void transaction::dir_full_lock(M& m, latm::mutex_type& mutex)
{
    BOOST_STM_CALL_CONTEXT_DCL_INST(0);
    if (transaction* t = get_inflight_tx_of_same_thread(false))
    {
        BOOST_STM_TRANSACTION_INVARIANT_VAR(t);

        t->make_isolated();

        bool hadLock = t->is_currently_locked_lock(&mutex);
        t->add_to_currently_locked_locks(&mutex);
        t->add_to_obtained_locks(&mutex);

        wait_until_all_locks_are_released_and_set(&mutex);

        if (hadLock) return;
        else synchro::lock(m BOOST_STM_CALL_CONTEXT("glatm_lock"));
        return;
    }

    int waitTime = 0, aborted = 0;
    for (;;)
    {
        {
        synchro::unique_lock<M> lk(m  BOOST_STM_CALL_CONTEXT("glatm_lock"));
        synchro::lock_guard<Mutex> lk_l(latm::instance().latmMutex_  BOOST_STM_CALL_CONTEXT("latm_lock"));

        //--------------------------------------------------------------------
        // if we are able to do the core lock work, break
        //--------------------------------------------------------------------
        if (dir_do_core_full_pthread_lock_mutex(&mutex, waitTime, aborted)) {
            latm::instance().latmLockedLocksOfThreadMap_[&mutex] = this_thread::get_id();
            lk.release();
            return;
        }

        //-----------------------------------------------------------------------
        // we weren't able to do the core lock work, unlock our mutex and sleep
        //-----------------------------------------------------------------------
        }

        SLEEP(cm_lock_sleep_time());
        waitTime += cm_lock_sleep_time();
        ++aborted;
    }

}

//----------------------------------------------------------------------------
// only allow one thread to execute any of these methods at a time
//----------------------------------------------------------------------------
template <typename M>
inline bool transaction::dir_full_try_lock(M& m, latm::mutex_type& mutex)
{
    BOOST_STM_CALL_CONTEXT_DCL_INST(0);
    if (transaction* t = get_inflight_tx_of_same_thread(false))
    {
        BOOST_STM_TRANSACTION_INVARIANT_VAR(t);

        t->make_isolated();

        bool hadLock = t->is_currently_locked_lock(&mutex);
        t->add_to_currently_locked_locks(&mutex);
        t->add_to_obtained_locks(&mutex);

        wait_until_all_locks_are_released_and_set(&mutex);

        if (hadLock) return true;
        else return synchro::try_lock(m);
    }

    synchro::unique_lock<M> lk(m, synchro::try_to_lock  BOOST_STM_CALL_CONTEXT("glatm_lock"));
    if (!lk) return false;
    synchro::lock_guard<Mutex> lk_l(latm::instance().latmMutex_  BOOST_STM_CALL_CONTEXT("latm_lock"));

    //-----------------------------------------------------------------------
    // if !core done, since trylock, we cannot stall & retry - just exit
    //-----------------------------------------------------------------------
    if (!dir_do_core_full_pthread_lock_mutex(&mutex, 0, 0))
    {
         return false;
    }

    latm::instance().latmLockedLocksOfThreadMap_[&mutex] = this_thread::get_id();
    // note: we do not release the transactionsInFlightMutex - this will prevents
    // new transactions from starting until this lock is released
    lk.release();
    return true;
}

//----------------------------------------------------------------------------
// only allow one thread to execute any of these methods at a time
//----------------------------------------------------------------------------
template <typename M>
inline void transaction::dir_full_unlock(M& m, latm::mutex_type& mutex)
{
    BOOST_STM_CALL_CONTEXT_DCL_INST(0);
    bool hasLock = true;
    {
    synchro::lock_guard<Mutex> lk_l(latm::instance().latmMutex_  BOOST_STM_CALL_CONTEXT("latm_lock"));

    if (transaction* t = get_inflight_tx_of_same_thread(false))
    {
        BOOST_STM_TRANSACTION_INVARIANT_VAR(t);

        if (!t->is_on_obtained_locks_list(&mutex))
        {
        // this is illegal, it means the transaction is unlocking a lock
        // it did not obtain (e.g., early release) while the transaction
        // is still in-flight. Throw exception
            BOOST_ASSERT(false&& "lock released for transaction that did not obtain it");
        }

        if (!t->is_currently_locked_lock(&mutex)) hasLock = false;
        t->remove_from_currently_locked_locks(&mutex);
    }

    latm::instance().latmLockedLocks_.erase(&mutex);

    if (latm::instance().latmLockedLocks_.empty())
    {
        synchro::lock_guard<Mutex> lk_g(*general_lock()  BOOST_STM_CALL_CONTEXT("general_lock"));
        thread_conflicting_mutexes_set_all(false);
    }

    latm::instance().latmLockedLocksOfThreadMap_.erase(&mutex);
    }
    if (hasLock) synchro::unlock(m BOOST_STM_CALL_CONTEXT("glatm_lock"));
    return;
}

}}

#endif

#endif // TRANSACTION_LOCK_AWARE_DIR_IMPL_H

