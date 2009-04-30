//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Justin E. Gottchlich 2009. 
// (C) Copyright Vicente J. Botet Escriba 2009. 
// Distributed under the Boost
// Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or 
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_STM_TX_PTR__HPP
#define BOOST_STM_TX_PTR__HPP

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <boost/stm/transaction.hpp>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
namespace boost { namespace stm {

template <typename T>
class tx_rd_ptr
{
public:
 
   tx_rd_ptr(transaction &t, T const &tx_obj) : 
      t_(t), tx_ptr_(&const_cast<T&>(t_.read(tx_obj))), written_(false)
   {}

   T* get() const
   {
      if (t_.forced_to_abort()) 
      {
         t_.lock_and_abort();
         throw aborted_transaction_exception("aborting transaction");
      }

      // we are already holding the written object
      if (written_) return tx_ptr_;

      T* temp = t_.get_written(*tx_ptr_);
 
      // if we found something, store this as the tx_ptr_
      if (0 != temp)
      {
         tx_ptr_ = temp;
         written_ = true;
      }

      return tx_ptr_;
   }

   T const & operator*() const { return *get(); }
   T const * operator->() const { return get(); }

private:
    
   mutable transaction &t_;
   mutable T *tx_ptr_;
   mutable bool written_;
};

template <typename T>
class tx_wr_ptr;

template <typename T>
class tx_upgrd_ptr
{
public:
 
   tx_upgrd_ptr(transaction &t, T & tx_obj) : 
      t_(t), tx_obj_(const_cast<T&>(t_.read(tx_obj))), upgraded_(false)
   {}

   const T* get()
   {
      if (t_.forced_to_abort()) 
      {
         t_.lock_and_abort();
         throw aborted_transaction_exception("aborting transaction");
      }
      if (!upgraded_) {
          T &new_tx_obj_=const_cast<T&>(t_.read(tx_obj_));
          if (&new_tx_obj_!=&tx_obj_) {
              tx_obj_=new_tx_obj_;
              upgraded_=true;
          }
      }
      return &tx_obj_;
   }

   const T& operator*()
   {
      return *get();
   }

   const T* operator->()
   {
      return get();
   }


   T& upgrade() {
       tx_obj_=t_.write(tx_obj_);
       upgraded_=true;
       return tx_obj_;
   }

private:
   //template <typename TT> 
   friend class tx_wr_ptr;

   mutable transaction &t_;
   mutable T &tx_obj_;
   bool upgraded_;
};


template <typename T>
class tx_wr_ptr
{
public:
 
   tx_wr_ptr(transaction &t, T & tx_obj) : 
      t_(t), tx_obj_(t_.write(tx_obj))
   {}

   tx_wr_ptr(tx_upgrd_ptr<T> &upgrd) : 
      t_(upgrd.t_), tx_obj_(upgrd.upgrade())
   {}

   T& operator*()
   {
      if (t_.forced_to_abort()) 
      {
         t_.lock_and_abort();
         throw aborted_transaction_exception("aborting transaction");
      }
      return tx_obj_;
   }

   T* operator->()
   {
      if (t_.forced_to_abort()) 
      {
         t_.lock_and_abort();
         throw aborted_transaction_exception("aborting transaction");
      }
      return &tx_obj_;
   }

private:   
   mutable boost::stm::transaction &t_;
   mutable T &tx_obj_;
};


}}
#endif


