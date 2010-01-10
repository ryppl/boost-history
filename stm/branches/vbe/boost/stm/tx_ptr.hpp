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

#ifndef BOOST_STM_TX_PTR__HPP
#define BOOST_STM_TX_PTR__HPP

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <boost/stm/transaction.hpp>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
namespace boost { namespace stm {

template <typename T, typename Poly=static_poly>
class write_ptr;

template <typename T, typename Poly=static_poly>
class read_ptr
{
    typedef read_ptr<T, Poly> this_type;
public:

   inline read_ptr(transaction &t, T const &tx_obj) :
      t_(t), tx_ptr_(&const_cast<T&>(t_.read(tx_obj))), written_(false)
   {}

   inline read_ptr(transaction &t, T const *tx_ptr) :
      t_(t), tx_ptr_(const_cast<T*>(t_.read_ptr(tx_ptr))), written_(false)
   {}

    inline read_ptr<T,Poly> & operator=(read_ptr<T,Poly> const& rhs) { // never throws
        if (this!=&rhs) {
            tx_ptr_=rhs.tx_ptr_;
        }
        return *this;
    }
    inline read_ptr<T,Poly> & operator=(T const * ptr) { // never throws
        tx_ptr_=const_cast<T*>(ptr);
        return *this;
    }

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

   inline T const & operator*() const { return *get(); }
   inline T const * operator->() const { return get(); }

   inline transaction &trans() { return t_; }

   T* write_ptr()
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
      else
      {
         tx_ptr_ = t_.write_ptr_poly<Poly>(tx_ptr_);
         written_ = true;
      }

      return tx_ptr_;
   }

    typedef T* this_type::*unspecified_bool_type;

    inline operator unspecified_bool_type() const {
        return tx_ptr_ == 0? 0: &this_type::tx_ptr_;
    }

private:

   mutable transaction &t_;
   mutable T *tx_ptr_;
   mutable bool written_;
    template <typename X, typename P> friend  class write_ptr;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template <typename T, typename Poly>
class write_ptr
{
public:

   inline write_ptr(transaction &t, T & tx_obj) :
      t_(t), tx_obj_(t_.write_poly<Poly>(tx_obj))
   {}

   inline write_ptr(transaction &t, T* ptr) :
      t_(t), tx_obj_(*t_.write_ptr_poly<Poly>(ptr))
   {}

   inline write_ptr(transaction &t, read_ptr<T> & tx_obj) :
      t_(t), tx_obj_(*t_.write_ptr_poly<Poly>(tx_obj.tx_ptr_))
   {}

   inline write_ptr(read_ptr<T,Poly> & ptr) :
      t_(ptr.trans()), tx_obj_(*ptr.write_ptr())
   {}

    write_ptr& operator=(T const* ptr) {
        tx_obj_=*t_.write_ptr(ptr);
        return *this;
    }
    write_ptr& operator=(read_ptr<T,Poly> & tx_obj) {
        tx_obj_=*t_.write_ptr_poly<Poly>(tx_obj.tx_ptr_);
        return *this;
    }
    T* get() const
    {
      if (t_.forced_to_abort())
      {
         t_.lock_and_abort();
         throw aborted_transaction_exception("aborting transaction");
      }
      return &tx_obj_;
    }
    T& operator*() { return *get(); }
    T* operator->() { return get(); }

private:
   mutable transaction &t_;
   mutable T &tx_obj_;
};

template <typename Poly, typename T>
inline write_ptr<T, Poly> make_write_ptr(transaction& tx, T const* ptr) {
    return write_ptr<T, Poly>(tx, ptr);
}

template <typename Poly, typename T>
inline write_ptr<T,Poly> make_write_ptr(transaction& tx, T* ptr) {
    return write_ptr<T, Poly>(tx, ptr);
}

template <typename Poly, typename T>
inline write_ptr<T, Poly> make_write_ptr(read_ptr<T, Poly>& ptr) {
    return write_ptr<T, Poly>(ptr);
}

template <typename T, typename Poly>
void delete_ptr(transaction& tx, read_ptr<T, Poly>& ptr) {
    delete_ptr(tx, ptr.get());
}
template <typename T, typename Poly>
void delete_ptr(transaction& tx, write_ptr<T, Poly>& ptr) {
    delete_ptr(tx, ptr.get());
}
}}
#endif


