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
#include <boost/stm/contention_managers/except_and_back_off_on_abort_notice_cm.hpp>

namespace boost { namespace stm {


void except_and_back_off_on_abort_notice_cm::abort_on_write
(transaction &t, base_transaction_object const &in)
{
   //SLEEP(initialSleepTime_);
   t.lock_and_abort();
   throw aborted_transaction_exception("aborting transaction");
}

/////////////////////////////////////////////////////////////////////////
void except_and_back_off_on_abort_notice_cm::abort_on_read
(transaction const &constT, base_transaction_object const &in)
{
   transaction &t = const_cast<transaction&>(constT);

   //SLEEP(initialSleepTime_);
   t.lock_and_abort();
   throw aborted_transaction_exception("aborting transaction");
}

/////////////////////////////////////////////////////////////////////////
void except_and_back_off_on_abort_notice_cm::abort_on_delete
(transaction const &constT, base_transaction_object const &in)
{
   transaction &t = const_cast<transaction&>(constT);

   //SLEEP(initialSleepTime_);
   t.lock_and_abort();
   throw aborted_transaction_exception("aborting transaction");
}

/////////////////////////////////////////////////////////////////////////
void except_and_back_off_on_abort_notice_cm::abort_on_new(transaction const &constT)
{
   transaction &t = const_cast<transaction&>(constT);

   //SLEEP(initialSleepTime_);
   t.lock_and_abort();
   throw aborted_transaction_exception("aborting transaction");
}

   ////////////////////////////////////////////////////////////////////////////
   void except_and_back_off_on_abort_notice_cm::perform_isolated_tx_wait_priority_promotion(transaction &lhs)
   {
      lhs.raise_priority();
   }

   ////////////////////////////////////////////////////////////////////////////
   void except_and_back_off_on_abort_notice_cm::perform_irrevocable_tx_wait_priority_promotion(transaction &lhs)
   {
      lhs.raise_priority();
   }

   //--------------------------------------------------------------------------
   // this code is only ever called if "validation" is on not "invalidation"
   // so don't worry about this code EVER stopping invalidation from committing
   //--------------------------------------------------------------------------
   bool except_and_back_off_on_abort_notice_cm::abort_before_commit(transaction const &t)
   {
      using namespace boost::stm;

      for (transaction::in_flight_trans_cont::const_iterator i =
           transaction::in_flight_transactions().begin();
      i != transaction::in_flight_transactions().end(); ++i)
      {
         if (t.priority() < (*i)->priority()) return true;
      }

      return false;
   }

   bool except_and_back_off_on_abort_notice_cm::permission_to_abort
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
      return true;
   }

   bool except_and_back_off_on_abort_notice_cm::permission_to_abort(transaction const &lhs, std::list<transaction*> &rhs)
   {
#ifdef JUST_PRIORITY
      int setSize = (lhs.writes() * lhs.priority()) +
                    (lhs.reads() * lhs.priority());
      double abortSetSize = 0;
      double abortPriority = 0;
      double decrementing = 1.0;

      double highestPriority = 0;

      bool hasLargestReadSet = true;

      for (std::list<core::transaction*>::iterator iter = rhs.begin();
           iter != rhs.end(); ++iter)
      {
         if ((*iter)->priority() > highestPriority)
         {
            highestPriority = (*iter)->priority();
         }

         if ((*iter)->reads() > lhs.reads()) hasLargestReadSet = false;
         if ((*iter)->writes() > 0) return true;

        abortSetSize += (double)(*iter)->reads() / decrementing;
        abortPriority += (double)(*iter)->priority() / decrementing;
        decrementing += 0.5;
      }

      if (lhs.priority() >= highestPriority) return true;

      if (hasLargestReadSet) return true;

      if (setSize >= abortPriority + abortSetSize)
      {
         return true;
      }
      else
      {
         return false;
      }

#else
      double setSize = (lhs.writes() * lhs.priority()) +
         (lhs.reads() * lhs.priority());
      double abortSetSize = 0;
      double abortPriority = 0;
      bool hasLargestReadSet = true;

      std::size_t mem = lhs.reads() + (lhs.writes() * 10);

      double decrementing = 1.0;
      for (std::list<boost::stm::transaction*>::iterator iter = rhs.begin();
           iter != rhs.end(); ++iter)
      {
         if ((*iter)->reads() > mem) hasLargestReadSet = false;

         if ((*iter)->writes() > 0) return true;

         if (lhs.reads() < (*iter)->reads() / 8 &&
             lhs.priority() * 100 < (*iter)->priority()) return false;

        abortSetSize += (double)(*iter)->reads() / decrementing;
        abortPriority += (double)(*iter)->priority() / decrementing;
        decrementing += 0.5;
      }

      if (hasLargestReadSet) return true;

      if (setSize >=
         (abortPriority / setSize) + (abortSetSize / setSize))
      {
         return true;
      }
      else
      {
         return false;
      }
#endif
   }

   bool except_and_back_off_on_abort_notice_cm::allow_lock_to_abort_tx
   (int const & lockWaitTime, int const &lockAborted, bool txTryingToAbortIsIrrevocable, transaction const &rhs)
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

}}
