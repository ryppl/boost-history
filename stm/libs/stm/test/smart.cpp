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

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <boost/stm/transaction.hpp>
#include "main.h"

using namespace boost::stm;

int kMaxOuterLoops = 10000;
int kMaxInnerLoops = 50000;

native_trans<int> txInt = 0, txInt2, txInt3 = 0;


template <typename T>
class tx_wptr
{
public:
 
   tx_wptr(boost::stm::transaction &t, T & tx_obj) : 
      t_(t), tx_obj_(t_.write(tx_obj))
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

#if 0
   T& w()
   {
      if (t_.forced_to_abort()) 
      {
         t_.lock_and_abort();
         throw aborted_transaction_exception("aborting transaction");
      }
      if (written_) return *tx_ptr_;
 
      // recheck initialization ...
      initialize();
 
      if (written_) return *tx_ptr_;
 
      // if it's only been read, a write will construct a new obj
      tx_ptr_ = &t_.write(*tx_ptr_);
      written_ = true;
      return *tx_ptr_;
   }
 
   T const & r() const
   {
      if (t_.forced_to_abort()) 
      {
         t_.lock_and_abort();
         throw aborted_transaction_exception("aborting transaction");
      }
      if (written_) return *tx_ptr_;
 
      // recheck initialization ...
      initialize();
 
      if (written_) return *tx_ptr_;
 
      if (read_) return *tx_ptr_;
 
      t_.read(*tx_ptr_);
      read_ = true;
      return *tx_ptr_;
   }
 
private:
 
   void initialize() const
   {
      // check transaction to see if tx_obj is
      // already in a transaction for this thread
      T* temp = t_.get_written(*tx_ptr_);
 
      // if we found something, store this as the tx_ptr_
      if (0 != temp)
      {
         tx_ptr_ = temp;
         written_ = true;
      }
      else
      {
         read_ = t_.has_been_read(*tx_ptr_);
      }
   }
#endif
   
   mutable boost::stm::transaction &t_;
   mutable T &tx_obj_;
};

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
static void test_nested()
{
   atomic(t)
   {
      tx_wptr<native_trans<int> > tx(t, txInt);
      ++*tx;
   } end_atom
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
static void test_parent()
{
   atomic(t)
   {
      tx_wptr<native_trans<int> > tx(t, txInt);
      if (0 == *tx) test_nested();
      cout << *tx << endl;
   } end_atom
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
static void test()
{
   int x = 0, y = 0, z = 0;

   for (int i = 0; i < kMaxOuterLoops; ++i)
   {
      atomic(t)
      {
         for (int j = 0; j < kMaxInnerLoops; ++j)
         {
            tx_wptr<native_trans<int> > tx(t, txInt);

            x = *tx;
            y = *tx;

            ++*tx;

            z = *tx;
         }

      } end_atom
   }
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
static void test_no_private()
{
   int x = 0, y = 0, z = 0;

   for (int i = 0; i < kMaxOuterLoops; ++i)
   {
      atomic(t)
      {
         for (int j = 0; j < kMaxInnerLoops; ++j)
         {
            x = t.r(txInt);
            y = t.r(txInt);

            ++t.w(txInt);

            z = t.r(txInt);
         }

      } end_atom
   }
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
static void test2()
{
   atomic(t)
   {
      native_trans<int> const loc = t.r(txInt);

      ++t.w(txInt);

   } end_atom

   int y = 0;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void test_smart()
{
   boost::stm::transaction::initialize();
   boost::stm::transaction::initialize_thread();

   boost::stm::transaction::do_direct_updating();

   test_parent();

   int const kMaxTestLoops = 4;

   test();

   for (int i = 0; i < kMaxTestLoops; ++i)
   {
      txInt = 0;

      cout << "outer loop #: " << kMaxOuterLoops << endl;
      cout << "inner loop #: " << kMaxInnerLoops << endl << endl;

      startTimer = time(NULL);
      test_no_private();
      endTimer = time(NULL);

      cout << "no_privatization time: " << endTimer - startTimer << endl;
      cout << "                txInt: " << txInt << endl << endl;


      startTimer = time(NULL);
      test();
      endTimer = time(NULL);

      cout << "   privatization time: " << endTimer - startTimer << endl;
      cout << "                txInt: " << txInt << endl << endl;

      cout << "---------------------------------" << endl;

      kMaxOuterLoops /= 10;
      kMaxInnerLoops *= 10;
   }

}



