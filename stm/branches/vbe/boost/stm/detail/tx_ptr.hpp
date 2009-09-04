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
#include <assert.h>
#include <boost/stm/transaction.hpp>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
namespace boost { namespace stm {

template <typename T>
class rd_ptr;
template <typename T>
class wr_ptr;
template <typename T>
class upgrd_ptr;
    
    
//-----------------------------------------------------------------------------
// a tx_ptr<T> is an smart pointer to a transactional_object<T> (which contains an instance of T). 
// Reference fields in linked structures should always be tx_ptrs.
// The result of derreferencing it will be the pointer to the T instance
// When this pointer is derreference on a transaction the transactional_object<T> is set a written and 
// the transaction specific storage will be used
// Otherwise the shared storage is used.
// Used to initialize a rd_ptr<T>, wr_ptr<T>, or upgrd_ptr<T>.    
//-----------------------------------------------------------------------------
    
template <typename T>
class tx_ptr {
public:
    typedef tx_ptr<T> this_type;
    transactional_object<T> * ptr_;

    tx_ptr() : ptr_(0) {}
    template<class Y> 
    explicit tx_ptr(Y * ptr) : ptr_(new transactional_object<Y>(ptr)) {}   
        
    explicit tx_ptr(transactional_object<T>* ptr) : ptr_(ptr) {}

    template<class Y> 
    tx_ptr(tx_ptr<Y> const& r) : ptr_(r.ptr_) {}// never throws
    
    template<class Y> 
    tx_ptr(rd_ptr<Y> const & r) : ptr_(r.ptr_) {}// never throws

    template<class Y> 
    tx_ptr(wr_ptr<Y> const & r) : ptr_(r.ptr_) {}// never throws

    template<class Y> 
    tx_ptr(upgrd_ptr<Y> const & r) : ptr_(r.ptr_) {}// never throws

    template<class Y> 
    tx_ptr & operator=(tx_ptr<Y> const & r) { // never throws        
        //this_type(r).swap(*this);
        ptr_=r.ptr_;
        return *this;
    }
    template<class Y> 
    tx_ptr & operator=(transactional_object<Y>* ptr)  { // never throws        
        ptr_=ptr;
        return *this;
    }
    
    // two transactional pointers are equal if they point to the same cache on the current transaction.    
    bool operator==(const tx_ptr<T>& rhs) {
        return ptr_==rhs.ptr_ || this->get()==rhs.ptr_ || ptr_==rhs.get();
    }
    
    T* operator->() const {
        return this->get();
    }
    T& operator*() const {
        return *this->get();
    }
    T* get() const {
        if (0==ptr_) return 0;
        transaction* tx=transaction::current_transaction();
        if (tx!=0) {
            if (tx->forced_to_abort()) {
                    tx->lock_and_abort();
                    throw aborted_transaction_exception("aborting transaction");
            }
            return &(tx->write_ptr(ptr_)->value);
        }
        return &ptr_->value;
    }
    operator T*() const { return this->get(); }
    
    typedef transactional_object<T>* this_type::*unspecified_bool_type;

    #if 0
    operator unspecified_bool_type() const 
    {
        return ptr_ == 0? 0: &this_type::ptr_;
    }
    #endif
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
void delete_ptr(tx_ptr<T> ptr) {
    if (ptr.ptr_==0) return;
    if (ptr.ptr_->transaction_==0) delete ptr.ptr_;      
    ptr.ptr_->transaction_->delete_tx_ptr(ptr.ptr_);
}


template <typename T>    
void delete_ptr(transaction& tx, tx_ptr<T> ptr) {
    if (ptr.ptr_==0) return;
    tx.delete_tx_ptr(ptr.ptr_);
}

template <typename T, typename U>
static tx_ptr<T> tx_static_cast(tx_ptr<U> ptr) {
    return tx_ptr<T>(tx_static_cast<T>(ptr->ptr_));
}
    
template <typename T, typename U>
static tx_ptr<T>* tx_dynamic_cast(tx_ptr<U>* ptr) {
    return tx_ptr<T>(tx_dynamic_cast<T>(ptr->ptr_));
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

//-----------------------------------------------------------------------------
// A rd_ptr<T> ("read pointer") points to an object that the current 
// transaction has opened for read only access. 
// You can only call a const method through a read pointer. 
// A rd_ptr<T> is constructed from an tx_ptr<T> through an explicit constructor. 
// Once a rd_ptr<T> has been constructed, an tx_ptr<T> can be opened for 
// reading simply by assignment (operator=()) into the constructed rd_ptr<T>. 
// It is not safe to derreference a rd_ptr<T> after having assigned the same 
// tx_ptr<T> to a wr_ptr<T>. If this is the case the readen value do not match 
// the writen one. If it is possible to write on the same transaction use 
// upgrd_ptr instead which is safe.
//-----------------------------------------------------------------------------
    
template <typename T>
class rd_ptr {
    typedef tx_ptr<T> this_type;
public:
 
    inline rd_ptr(transaction &t, tx_ptr<T> tx_obj) : 
        ptr_(&t.insert_and_return_read_memory(*tx_obj.ptr_))
    {}

    template<class Y> 
    rd_ptr & operator=(tx_ptr<Y> const & r) { // never throws        
        //this_type(r).swap(*this);
        ptr_=r.ptr_;
        return *this;
    }
    
    const T* get() const {
        //if (ptr_->transaction_->forced_to_abort()) {
        //    ptr_->transaction_->lock_and_abort();
        //    throw aborted_transaction_exception("aborting transaction");
        //}

        return &ptr_->value;
    }

    inline const T & operator*() const { return *get(); }
    inline const T* operator->() const { return get(); }
    
    operator const T*() const { return get(); }
    
    typedef transactional_object<T>* this_type::*unspecified_bool_type;

    #if 0
    operator unspecified_bool_type() const 
    {
        return ptr_ == 0? 0: &this_type::ptr_;
    }
    #endif    
//private:
    mutable transactional_object<T>* ptr_;
};

template <typename T>    
rd_ptr<T> make_rd_ptr(transaction& tx, tx_ptr<T>& ptr) {
    return rd_ptr<T>(tx, ptr);
}

template <typename T>    
rd_ptr<T> make_rd_ptr(tx_ptr<T>& ptr) {
    transaction* tx = transaction::current_transaction();
    assert(tx==0);
    return rd_ptr<T>(*tx, ptr);
}

template <typename T>    
void delete_ptr(rd_ptr<T> ptr) {
    if (ptr.ptr_==0) return;
    if (ptr.ptr_->transaction_==0) delete ptr.ptr_;      
    ptr.ptr_->transaction_->delete_tx_ptr(ptr.ptr_);
}

template <typename T>    
void delete_ptr(transaction& tx, rd_ptr<T> ptr) {
    if (ptr.ptr_==0) return;
    tx.delete_tx_ptr(ptr.ptr_);
}

//-----------------------------------------------------------------------------
// A upgrd_ptr<T> ("upgradable pointer") points to an object that the current 
// transaction has opened for read only access. 
// You can only call a const method through a read pointer. 
// A upgrd_ptr<T> is constructed from an tx_ptr<T> through a constructor 
// having also the transaction as parameter. 
// Once a rd_ptr<T> has been constructed, an tx_ptr<T> can be opened for 
// reading simply by assignment (operator=()) into the constructed rd_ptr<T>. 
// It is safe to derreference a rd_ptr<T> after having assigned the same 
// tx_ptr<T> to a wr_ptr<T>. 
// A upgrd_ptr<T> can be upgraded to a wr_ptr<T> through a constructor. 
//-----------------------------------------------------------------------------

template <typename T>
class upgrd_ptr {
    typedef tx_ptr<T> this_type;
public:
 
    inline upgrd_ptr() : tx_(0) {}
    inline upgrd_ptr(transaction &t, tx_ptr<T> tx_obj) : tx_(&t),
        ptr_(const_cast<transactional_object<T>*>(t.read_ptr(tx_obj.ptr_))), written_(false)    {}

    template<class Y> 
    upgrd_ptr & operator=(tx_ptr<Y> const&  r) { // never throws        
        //this_type(r).swap(*this);
        ptr_=r.ptr_;
        written_=false;
        return *this;
    }

    const T* get() const {
        if (tx_->forced_to_abort()) {
            tx_->lock_and_abort();
            throw aborted_transaction_exception("aborting transaction");
        }

        // we don't hold the written object and 
        // we have alrerady written an object on this transaction maybe is this one
        if (!written_ && tx_->written()) {
            transactional_object<T>* temp = tx_->get_written(*ptr_);
 
            // if we found something, store this as the tx_ptr_
            if (0 != temp) {
                ptr_ = temp;
                written_ = true;
            }
        }

        return &ptr_->value;
    }

    inline T const & operator*() const { return *get(); }
    inline T const * operator->() const { return get(); }

    //operator const T*() const { return get(); }
    
    typedef transactional_object<T>* this_type::*unspecified_bool_type;

    #if 1
    operator unspecified_bool_type() const 
    {
        return ptr_ == 0? 0: &this_type::ptr_;
    }
    #endif
    void write_ptr(transactional_object<T>* ptr) {
        ptr_ = ptr;
        written_ = true;
    }

    T* write_ptr() {
        if (tx_->forced_to_abort()) {
            tx_->lock_and_abort();
            throw aborted_transaction_exception("aborting transaction");
        }

        // we are already holding the written object
        if (written_) return ptr_;

        transactional_object<T>* temp = tx_->get_written(ptr_);
 
        // if we found something, store this as the tx_ptr_
        if (0 != temp) {
            ptr_ = temp;
            written_ = true;
        } else {
            ptr_ = tx_->write_ptr(ptr_);
            written_ = true;
        }

        return ptr_;
    }

//private:
    mutable transaction* tx_;
    mutable transactional_object<T>* ptr_;
    mutable bool written_;
};

template <typename T>    
void delete_ptr(upgrd_ptr<T> ptr) {
    if (ptr.ptr_==0) return;
    if (ptr.ptr_->transaction_==0) delete ptr.ptr_;      
    ptr.ptr_->transaction_->delete_tx_ptr(ptr.ptr_);
}

//-----------------------------------------------------------------------------
// A wr_ptr<T> ("write pointer") points to a shared object that the current transaction has opened for writing. 
// A wr_ptr<T> is initialized explicitly from an sh_ptr<T>. 
// A wr_ptr<T> can also be explicitly constructed from a upgrd_ptr<T> as an upgrade-to-writable operation. 
//-----------------------------------------------------------------------------

template <typename T>
class wr_ptr {
    typedef tx_ptr<T> this_type;
public:
 
    inline wr_ptr(transaction &t, tx_ptr<T> tx_obj) : 
        ptr_(t.write_ptr(tx_obj.ptr_))
    {}

    inline wr_ptr(transaction &t, upgrd_ptr<T> tx_obj) : 
        ptr_(t.write_ptr(tx_obj.ptr_))
    {
        tx_obj.write_ptr(ptr_);
    }

    T* get() {
        if (ptr_->transaction_->forced_to_abort()) {
            ptr_->transaction_->lock_and_abort();
            throw aborted_transaction_exception("aborting transaction");
        }
        return &ptr_->value;
    }
    
    inline T& operator*() { return *get(); }
    inline T* operator->() { return get(); }

    operator const T*() const { return get(); }
    
    typedef transactional_object<T>* this_type::*unspecified_bool_type;

    #if 0
    operator unspecified_bool_type() const 
    {
        return ptr_ == 0? 0: &this_type::ptr_;
    }
    #endif
//private:   
    mutable transactional_object<T>* ptr_;
};

template <typename T>    
wr_ptr<T> make_wr_ptr(transaction& tx, tx_ptr<T>& ptr) {
    return wr_ptr<T>(tx, ptr);
}


template <typename T>    
wr_ptr<T> make_wr_ptr(tx_ptr<T>& ptr) {
    transaction* tx = transaction::current_transaction();
    if (tx==0) throw "error";
    return wr_ptr<T>(*tx, ptr);
}

template <typename T>    
void delete_ptr(wr_ptr<T> ptr) {
    if (ptr.ptr_==0) return;
    if (ptr.ptr_->transaction_==0) delete ptr.ptr_;      
    ptr.ptr_->transaction_->delete_tx_ptr(ptr.ptr_);
}


//=========================

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


