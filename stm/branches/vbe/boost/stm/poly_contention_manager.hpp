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
// forward declaration
//-----------------------------------------------------------------------------
template <class ContentionManagerPolicy>
class basic_transaction;

class poly_contention_manager;

//-----------------------------------------------------------------------------
class base_contention_manager
{
public:
   virtual void abort_on_new(basic_transaction<poly_contention_manager> const &t) = 0;
   virtual void abort_on_delete(basic_transaction<poly_contention_manager> const &t,
      base_transaction_object const &in) = 0;

   virtual void abort_on_read(basic_transaction<poly_contention_manager> const &t,
      base_transaction_object const &in) = 0;
   virtual void abort_on_write(basic_transaction<poly_contention_manager> &t,
      base_transaction_object const &in) = 0;

   virtual bool abort_before_commit(basic_transaction<poly_contention_manager> const &t) = 0;

   virtual bool permission_to_abort
      (basic_transaction<poly_contention_manager> const &lhs, basic_transaction<poly_contention_manager> const &rhs) = 0;

   virtual bool allow_lock_to_abort_tx(int const & lockWaitTime, int const &lockAborted,
      bool txIsIrrevocable, basic_transaction<poly_contention_manager> const &rhs) = 0;

   virtual int lock_sleep_time() =0;

   virtual void perform_isolated_tx_wait_priority_promotion(basic_transaction<poly_contention_manager> &) = 0;
   virtual void perform_irrevocable_tx_wait_priority_promotion(basic_transaction<poly_contention_manager> &) = 0;

   virtual ~base_contention_manager() {};
};

class poly_contention_manager
{
protected:
   static base_contention_manager *cm_;

public:
    static void contention_manager(base_contention_manager *rhs) { delete cm_; cm_ = rhs; }
    static base_contention_manager* get_contention_manager() { return cm_; }

    void abort_on_new(basic_transaction<poly_contention_manager> const &t)
        { cm_->abort_on_new(t);}
    void abort_on_delete(basic_transaction<poly_contention_manager> const &t,
                            base_transaction_object const &in)
        { cm_->abort_on_delete(t,in);}

    void abort_on_read(basic_transaction<poly_contention_manager> const &t,
                        base_transaction_object const &in)
        { cm_->abort_on_read(t,in);}
    void abort_on_write(basic_transaction<poly_contention_manager> &t,
                        base_transaction_object const &in)
        { cm_->abort_on_write(t,in);}

    bool abort_before_commit(basic_transaction<poly_contention_manager> const &t)
        { return cm_abort_before_commit(t);}

    bool permission_to_abort(basic_transaction<poly_contention_manager> const &lhs,
                                basic_transaction<poly_contention_manager> const &rhs)
        { return cm_->permission_to_abort(lhs,rhs);}

    bool allow_lock_to_abort_tx(int const & lockWaitTime, int const &lockAborted, bool txIsIrrevocable,
                                basic_transaction<poly_contention_manager> const &rhs)
        { return cm_->allow_lock_to_abort_tx(lockWaitTime,lockAborted,txIsIrrevocable,rhs);}

    int lock_sleep_time()
        { return cm_->lock_sleep_time();}

    void perform_isolated_tx_wait_priority_promotion(basic_transaction<poly_contention_manager> &t)
        { cm_->perform_isolated_tx_wait_priority_promotion(t);}
    void perform_irrevocable_tx_wait_priority_promotion(basic_transaction<poly_contention_manager> &t)
        { cm_->perform_irrevocable_tx_wait_priority_promotion(t);}

};

//-----------------------------------------------------------------------------
template <typename Base>
class default_contention_manager : public Base
{
public:
   //--------------------------------------------------------------------------
   void abort_on_new(basic_transaction<poly_contention_manager> const &t);
   void abort_on_delete(basic_transaction<poly_contention_manager> const &t,
      base_transaction_object const &in);

   void abort_on_read(basic_transaction<poly_contention_manager> const &t,
      base_transaction_object const &in);
   void abort_on_write(basic_transaction<poly_contention_manager> &t,
      base_transaction_object const &in);

   bool abort_before_commit(basic_transaction<poly_contention_manager> const &t)
   {
      return false;
   }

   bool permission_to_abort
      (basic_transaction<poly_contention_manager> const &lhs, basic_transaction<poly_contention_manager> const &rhs)
   { return true; }

   bool allow_lock_to_abort_tx(int const & lockWaitTime, int const &lockAborted,
      bool txIsIrrevocable, basic_transaction<poly_contention_manager> const &rhs);

   int lock_sleep_time() { return 10; }
   void perform_isolated_tx_wait_priority_promotion(basic_transaction<poly_contention_manager> &);
   void perform_irrevocable_tx_wait_priority_promotion(basic_transaction<poly_contention_manager> &);
};


#ifndef BOOST_STM_CONTENTION_MANAGER
#define BOOST_STM_CONTENTION_MANAGER ::boost::stm::poly_contention_manager
#define BOOST_STM_CONTENTION_MANAGER_POLY
#endif

typedef basic_transaction<BOOST_STM_CONTENTION_MANAGER> transaction;
