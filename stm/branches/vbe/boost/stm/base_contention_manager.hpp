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

#ifndef BOOST_STM_BASE_CONTENTION_MANAGER__HPP
#define BOOST_STM_BASE_CONTENTION_MANAGER__HPP

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <list>
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <boost/stm/detail/config.hpp>
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
namespace boost { namespace stm {

//-----------------------------------------------------------------------------
// forward declarations
//-----------------------------------------------------------------------------
class transaction;
class base_transaction_object;
    
//-----------------------------------------------------------------------------
class base_contention_manager
{
public:
   virtual void abort_on_new(transaction const &t) = 0;
   virtual void abort_on_delete(transaction const &t,
      base_transaction_object const &in) = 0;

   virtual void abort_on_read(transaction const &t,
      base_transaction_object const &in) = 0;
   virtual void abort_on_write(transaction &t,
      base_transaction_object const &in) = 0;

   virtual bool abort_before_commit(transaction const &t) = 0;

   virtual bool permission_to_abort
      (transaction const &lhs, transaction const &rhs) = 0;

   virtual bool permission_to_abort
      (transaction const &lhs, std::list<transaction*> &rhs)
   { return true; }

   virtual bool allow_lock_to_abort_tx(int const & lockWaitTime, int const &lockAborted,
      bool txIsIrrevocable, transaction const &rhs) = 0;

   virtual int lock_sleep_time() { return 10; }

   virtual void perform_isolated_tx_wait_priority_promotion(transaction &) = 0;
   virtual void perform_irrevocable_tx_wait_priority_promotion(transaction &) = 0;

   virtual ~base_contention_manager() {};
};


} // namespace core
}
#endif // BOOST_STM_BASE_CONTENTION_MANAGER__HPP


