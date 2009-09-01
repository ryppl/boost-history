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
class tx_ptr {
public:
    typedef tx_ptr<T> this_type;
    mutable transactional_object<T> * ptr_;

    tx_ptr() : ptr_(0) {}
    //explicit tx_ptr(T* ptr) : ptr_(new transactional_object<T>(ptr)) {}   
    template<class Y> 
    explicit tx_ptr(Y * ptr) : ptr_(new transactional_object<Y>(ptr)) {}   
        
    explicit tx_ptr(transactional_object<T>* ptr) : ptr_(ptr) {}

    template<class Y> 
    tx_ptr(tx_ptr<Y> const & r) : ptr_(r.ptr_) {}// never throws
    
    template<class Y> 
    tx_ptr & operator=(tx_ptr<Y> const & r)  // never throws        
    {
        this_type(r).swap(*this);
        return *this;
    }
        
    bool operator==(const tx_ptr<T>& rhs) {
        return this->get()==rhs.get();
    }
    T* operator->() const {
        return this->get();
    }
    T& operator*() const {
        return *this->get();
    }
    #if 0
    T const & operator*() const {
        return *this->get();
    }
    T const * operator->() const {
        return this->get();
    }
    #endif
    T* get() const {
        if (0==ptr_) return 0;
        if (0==ptr_->transaction_) { 
            transaction* tx=transaction::current_transaction();
            if (tx!=0) {
                if (tx->forced_to_abort()) {
                     tx->lock_and_abort();
                     throw aborted_transaction_exception("aborting transaction");
                }
                ptr_=tx->write_ptr(ptr_);
            }
        }
        return &ptr_->value;
    }
    T const * get2() const {
        if (0==ptr_) return 0;
        if (0==ptr_->transaction_) { 
            transaction* tx=transaction::current_transaction();
            if (tx!=0) {
                if (tx->forced_to_abort()) {
                     tx->lock_and_abort();
                     throw aborted_transaction_exception("aborting transaction");
                }
                ptr_=const_cast<transactional_object<T>*>(tx->read_ptr(ptr_));
            }
        }
        return &ptr_->value;
    }
	operator T*() const { return this->get(); }
	//operator T const*() const { return this->get(); }
    
    typedef transactional_object<T>* this_type::*unspecified_bool_type;

    operator unspecified_bool_type() const 
    {
        return ptr_ == 0? 0: &this_type::ptr_;
    }
    // operator! is redundant, but some compilers need it
    bool operator! () const // never throws
    {
        return ptr_ == 0;
    }
    void swap(tx_ptr & other) // never throws
    {
        std::swap(ptr_, other.ptr_);
    }
};    

template <typename T, typename U>
bool operator==(const tx_ptr<T>& lhs, const tx_ptr<U>& rhs) {
    return lhs.get()==rhs.get();
}

template <typename T, typename U>
bool operator!=(const tx_ptr<T>& lhs, const tx_ptr<U>& rhs) {
    return lhs.get()!=rhs.get();
}


template<class T> inline void swap(tx_ptr<T> & a, tx_ptr<T> & b)
{
    a.swap(b);
}

template <typename T>    
tx_ptr<T> make_tx_ptr() {
    return tx_ptr<T>(new transactional_object<T>());
}

template <typename T, typename A1>    
tx_ptr<T> make_tx_ptr(A1 const &a1) {
    return tx_ptr<T>(new transactional_object<T>(a1));
}

template <typename T>
class tx_obj {
public:
    transactional_object<T> obj_;
    transactional_object<T> * ptr_;

    tx_obj() : obj_(), ptr_(&obj_) {}
    template <typename T1>
    tx_obj(T1 p1) : obj_(p1), ptr_(&obj_) {}
        
    bool operator==(const tx_obj<T>& rhs) {
        return *this->get()==*rhs.get();
    }
    T* operator->() {
        return this->get();
    }
    T& operator*() {
        return *this->get();
    }
    T* get() {
        if (0==ptr_->transaction_) { 
            transaction* tx=transaction::current_transaction();
            if (tx!=0) {
                if (tx->forced_to_abort()) {
                     tx->lock_and_abort();
                     throw aborted_transaction_exception("aborting transaction");
                }
                ptr_=tx->write_ptr(ptr_);
            }
        }
        return &ptr_->value;
    }
};    

template <typename T>
class rd_ptr {
public:
 
    inline rd_ptr(transaction &t, tx_ptr<T> const & tx_obj) : 
        //tx_obj_p_(t.read_ptr(tx_obj.ptr_))
        tx_obj_p_(&t.insert_and_return_read_memory(*tx_obj.ptr_))

{}

    const T* get() const {
        //if (tx_obj_p_->transaction_->forced_to_abort()) {
        //    tx_obj_p_->transaction_->lock_and_abort();
        //    throw aborted_transaction_exception("aborting transaction");
        //}

        return &tx_obj_p_->value;
    }

    inline const T & operator*() const { return *get(); }
    inline const T* operator->() const { return &tx_obj_p_->value; }
private:
    const transactional_object<T> * tx_obj_p_;
};

template <typename T>
class upgrd_ptr {
public:
 
    inline upgrd_ptr(transaction &t, tx_ptr<T> const &tx_obj) : 
        tx_ptr_(t.read_ptr(tx_obj.ptr_)), written_(false)
    {}

    const T* get() const {
        if (tx_ptr_->transaction_->forced_to_abort()) {
            tx_ptr_->transaction_->lock_and_abort();
            throw aborted_transaction_exception("aborting transaction");
        }

        // we don't hold the written object and 
        // we have alrerady written an object on this transaction maybe is this one
        if (!written_ && tx_ptr_->transaction_->written()) {
            transactional_object<T>* temp = tx_ptr_->transaction_->get_written(*tx_ptr_);
 
            // if we found something, store this as the tx_ptr_
            if (0 != temp) {
                tx_ptr_ = temp;
                written_ = true;
            }
        }

        return &tx_ptr_->value;
    }

    inline T const & operator*() const { return *get(); }
    inline T const * operator->() const { return get(); }

    //inline transaction &trans() { return t_; }

    T* write_ptr() {
        if (tx_ptr_->transaction_->forced_to_abort()) {
            tx_ptr_->transaction_->lock_and_abort();
            throw aborted_transaction_exception("aborting transaction");
        }

        // we are already holding the written object
        if (written_) return tx_ptr_;

        transactional_object<T>* temp = tx_ptr_->transaction_->get_written(*tx_ptr_);
 
        // if we found something, store this as the tx_ptr_
        if (0 != temp) {
            tx_ptr_ = temp;
            written_ = true;
        } else {
            tx_ptr_ = &tx_ptr_->transaction_->write(*tx_ptr_);
            written_ = true;
        }

        return tx_ptr_;
    }

private:
    mutable transactional_object<T> const *tx_ptr_;
    mutable bool written_;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template <typename T>
class wr_ptr {
public:
 
    inline wr_ptr(transaction &t, tx_ptr<T> & tx_obj) : 
        tx_obj_(t.write(*tx_obj.ptr_))
    {}

    inline T& operator*() {
        if (tx_obj_.transaction_->forced_to_abort()) {
            tx_obj_.transaction_->lock_and_abort();
            throw aborted_transaction_exception("aborting transaction");
        }
        return tx_obj_.value;
    }

    inline T* operator->() {
        if (tx_obj_.transaction_->forced_to_abort()) {
            tx_obj_.transaction_->lock_and_abort();
            throw aborted_transaction_exception("aborting transaction");
        }
        return &tx_obj_.value;
    }

private:   
    mutable transactional_object<T> &tx_obj_;
};


template <typename T>
class read_ptr
{
public:
 
   inline read_ptr(transaction &t, T const &tx_obj) : 
      t_(t), tx_ptr_(&const_cast<T&>(t_.read(tx_obj))), written_(false)
   {}

   const T* get() const
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
         tx_ptr_ = &t_.write(*tx_ptr_);
         written_ = true;
      }

      return tx_ptr_;
   }

private:
    
   mutable transaction &t_;
   mutable T *tx_ptr_;
   mutable bool written_;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template <typename T>
class write_ptr
{
public:
 
   inline write_ptr(transaction &t, T & tx_obj) : 
      t_(t), tx_obj_(t_.write(tx_obj))
   {}

   inline T& operator*()
   {
      if (t_.forced_to_abort()) 
      {
         t_.lock_and_abort();
         throw aborted_transaction_exception("aborting transaction");
      }
      return tx_obj_;
   }

   inline T* operator->()
   {
      if (t_.forced_to_abort()) 
      {
         t_.lock_and_abort();
         throw aborted_transaction_exception("aborting transaction");
      }
      return &tx_obj_;
   }

private:   
   mutable transaction &t_;
   mutable T &tx_obj_;
};


}}
#endif


