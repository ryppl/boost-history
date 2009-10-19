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

#ifndef BOOST_STM_EXCEPT_AND_BACK_OFF_ABORT_NOTICE_CM_H
#define BOOST_STM_EXCEPT_AND_BACK_OFF_ABORT_NOTICE_CM_H

namespace boost { namespace stm {
    class transaction;

////////////////////////////////////////////////////////////////////////////
//
// this class backs off (and sleeps) when aborting. this is commonly known
// as "exponential backoff" for locking mechanisms.
//
////////////////////////////////////////////////////////////////////////////
class except_and_back_off_on_abort_notice_cm //: public base_contention_manager
{
public:

#if 0
   static void init(int const initialSleepTime, int const sleepIncrease,
      int const maxIncreases)
   {
        sleepTime_=initialSleepTime;
        kSleepFactorIncrease_=sleepIncrease;
        kMaxIncreases_=maxIncreases;
        initialSleepTime_=initialSleepTime;
        kMaxSleepTime_ = kSleepFactorIncrease_ * sleepTime_ * kMaxIncreases_;
   }
#endif
   ////////////////////////////////////////////////////////////////////////////
   static void abort_on_write(transaction &t,
      base_transaction_object const &in);
   static void abort_on_read(transaction const &t,
      base_transaction_object const &in);
   static void abort_on_delete(transaction const &t,
      base_transaction_object const &in);
   static void abort_on_new(transaction const &t);

   ////////////////////////////////////////////////////////////////////////////
   static void perform_isolated_tx_wait_priority_promotion(transaction &lhs);

   ////////////////////////////////////////////////////////////////////////////
   static void perform_irrevocable_tx_wait_priority_promotion(transaction &lhs);
   inline static int lock_sleep_time() { return 10; }

   //--------------------------------------------------------------------------
   // this code is only ever called if "validation" is on not "invalidation"
   // so don't worry about this code EVER stopping invalidation from committing
   //--------------------------------------------------------------------------
   static bool abort_before_commit(transaction const &t);

   static bool permission_to_abort(transaction const &lhs, transaction const &rhs);
   static bool permission_to_abort(transaction const &lhs, std::list<transaction*> &rhs);

   static bool allow_lock_to_abort_tx(int const & lockWaitTime, int const &lockAborted,
                                bool txTryingToAbortIsIrrevocable, transaction const &rhs);

//private:

   static int sleepTime_;
   static int const kSleepFactorIncrease_;
   static int kMaxSleepTime_;
   static int const kMaxIncreases_;
   static int const initialSleepTime_;
};

typedef except_and_back_off_on_abort_notice_cm contention_manager_type;

#define BOOST_STM_CM_STATIC() \
   int boost::stm::except_and_back_off_on_abort_notice_cm::sleepTime_=BOOST_STM_CM_STATIC_INITIAL_SLEEP_TIME; \
   int const boost::stm::except_and_back_off_on_abort_notice_cm::kSleepFactorIncrease_=BOOST_STM_CM_STATIC_SLEEP_INCREASE; \
   int const boost::stm::except_and_back_off_on_abort_notice_cm::kMaxIncreases_=BOOST_STM_CM_STATIC_MAX_INCREASES; \
   int const boost::stm::except_and_back_off_on_abort_notice_cm::initialSleepTime_=BOOST_STM_CM_STATIC_INITIAL_SLEEP_TIME; \
   int boost::stm::except_and_back_off_on_abort_notice_cm::kMaxSleepTime_=BOOST_STM_CM_STATIC_SLEEP_INCREASE*BOOST_STM_CM_STATIC_INITIAL_SLEEP_TIME*BOOST_STM_CM_STATIC_MAX_INCREASES

       
}}
#endif
