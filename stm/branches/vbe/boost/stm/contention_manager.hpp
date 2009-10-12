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

#ifndef BOOST_STM_CONTENTION_MANAGER_H
#define BOOST_STM_CONTENTION_MANAGER_H

#include <boost/stm/transaction.hpp>

namespace boost { namespace stm {


    //-----------------------------------------------------------------------------
class DefaultContentionManager : public base_contention_manager
{
public:
   //--------------------------------------------------------------------------
   void abort_on_new(transaction const &t);
   void abort_on_delete(transaction const &t,
      base_transaction_object const &in);

   void abort_on_read(transaction const &t,
      base_transaction_object const &in);
   void abort_on_write(transaction &t,
      base_transaction_object const &in);

   virtual bool abort_before_commit(transaction const &t)
   {
      return false;
   }

   virtual bool permission_to_abort
      (transaction const &lhs, transaction const &rhs)
   { return true; }

   virtual bool allow_lock_to_abort_tx(int const & lockWaitTime, int const &lockAborted,
      bool txIsIrrevocable, transaction const &rhs);

   virtual void perform_isolated_tx_wait_priority_promotion(transaction &);
   virtual void perform_irrevocable_tx_wait_priority_promotion(transaction &);
};

////////////////////////////////////////////////////////////////////////////
//
// this class does nothing on abort notices for writes and reads
//
// this is an important concept as sometimes the overhead from the exception
// handling is more severe than simply aborting at "commit" time.
//
////////////////////////////////////////////////////////////////////////////
class NoExceptionOnAbortNoticeOnReadWritesCM : public base_contention_manager
{
public:
   /////////////////////////////////////////////////////////////////////////
   // do nothing for both of these interfaces
   /////////////////////////////////////////////////////////////////////////
   void abort_on_write(transaction &t,
      base_transaction_object const &in) {}
   void abort_on_read(transaction const &t,
      base_transaction_object const &in) {}
   void abort_on_delete(transaction const &t,
      base_transaction_object const &in) {}
   void abort_on_new(transaction const &t) {}
};

////////////////////////////////////////////////////////////////////////////
//
// this class backs off (and sleeps) when aborting. this is commonly known
// as "exponential backoff" for locking mechanisms.
//
////////////////////////////////////////////////////////////////////////////
class ExceptAndBackOffOnAbortNoticeCM : public base_contention_manager
{
public:

   ExceptAndBackOffOnAbortNoticeCM(int const initialSleepTime, int const sleepIncrease,
      int const maxIncreases)
      : sleepTime_(initialSleepTime), kSleepFactorIncrease_(sleepIncrease),
        kMaxIncreases_(maxIncreases), initialSleepTime_(initialSleepTime)
   {
      kMaxSleepTime_ = kSleepFactorIncrease_ * sleepTime_ * kMaxIncreases_;
   }

   ////////////////////////////////////////////////////////////////////////////
   void abort_on_write(transaction &t,
      base_transaction_object const &in);
   void abort_on_read(transaction const &t,
      base_transaction_object const &in);
   void abort_on_delete(transaction const &t,
      base_transaction_object const &in);
   void abort_on_new(transaction const &t);

   ////////////////////////////////////////////////////////////////////////////
   void perform_isolated_tx_wait_priority_promotion(transaction &lhs)
   {
      lhs.raise_priority();
   }

   ////////////////////////////////////////////////////////////////////////////
   void perform_irrevocable_tx_wait_priority_promotion(transaction &lhs)
   {
      lhs.raise_priority();
   }

   //--------------------------------------------------------------------------
   // this code is only ever called if "validation" is on not "invalidation"
   // so don't worry about this code EVER stopping invalidation from committing
   //--------------------------------------------------------------------------
   virtual bool abort_before_commit(transaction const &t)
   {

      for (transaction::in_flight_trans_cont::const_iterator i =
           transaction::in_flight_transactions().begin();
      i != transaction::in_flight_transactions().end(); ++i)
      {
         if (t.priority() < (*i)->priority()) return true;
      }

      return false;
   }

   virtual bool permission_to_abort
      (transaction const &lhs, transaction const &rhs)
   {
      return true;
#if 0
      if (lhs.priority() + 100 > rhs.priority()) return true;

      rhs.set_priority(rhs.priority() + 1000);
      return false;
#endif
      //return lhs.writes() * lhs.priority() >= rhs.writes() * rhs.priority();
      //return lhs.writes() + lhs.reads() >= rhs.writes() + rhs.reads();
   }

   virtual bool allow_lock_to_abort_tx
   (int const & lockWaitTime, int const &lockAborted,
   bool txTryingToAbortIsIrrevocable, transaction const &rhs)
   {
      if (txTryingToAbortIsIrrevocable) return true;

#ifndef DISABLE_READ_SETS
      if ((size_t)lockWaitTime > rhs.read_set_size() + 100 * rhs.writes())
#else
      if ((size_t)lockWaitTime > 100 * rhs.writes())
#endif
      {
         return true;
      }
      else return false;
   }

private:

   int sleepTime_;
   int const kSleepFactorIncrease_;
   int kMaxSleepTime_;
   int const kMaxIncreases_;
   int const initialSleepTime_;
};

}}
#endif // CONTENTION_MANAGER_H
