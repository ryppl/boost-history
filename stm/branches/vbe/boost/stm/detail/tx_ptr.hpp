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
// class tx_obj wraps a transactional_object providing builting operators
//-----------------------------------------------------------------------------

template <typename T>
class tx_obj {
public:
    transactional_object<T> obj_;

    //-----------------------------------------------------------------------------
    // default constructor valid ony if T has a default constructor

    tx_obj() : obj_() {}

    //
    template<class Y>
    tx_obj(tx_obj<Y> const& r) : obj_(r.ref()) {} // throws only if obj_ contructor throws

    template <typename T1>
    tx_obj(T1 p1) : obj_(p1) {}

    #if 0
    bool operator==(const tx_obj<T>& rhs) const {
        return this->ref()==rhs.ref();
    }

    bool operator==(const T& rhs) const {
        return this->ref()==rhs;
    }
    #endif

    //operator T() const { return *this->get(); }

    T* operator->() {
        return this->get();
    }
    const T* operator->() const {
        return this->get();
    }
    T& operator*() {
        return this->ref();
    }
    const T& operator*() const {
        return this->ref();
    }
    T* get() {
        return &this->ref();
    }
    const T* get() const {
        return &this->ref();
    }
    T& ref() {
        transaction* tx=transaction::current_transaction();
        if (tx!=0) {
            if (tx->forced_to_abort()) {
                tx->lock_and_abort();
                throw aborted_transaction_exception("aborting transaction");
            }
            return tx->write(obj_).value;
        }
        return obj_.value;
    }

    const T& ref() const {
        transaction* tx=transaction::current_transaction();
        if (tx!=0) {
            if (tx->forced_to_abort()) {
                tx->lock_and_abort();
                throw aborted_transaction_exception("aborting transaction");
            }
            return tx->read(obj_).value;
        }
        return obj_.value;
    }


    tx_obj& operator--() { --ref(); return *this; }
    T operator--(int) { T n = obj_.value_; --ref(); return n; }

    tx_obj& operator++() { ++ref(); return *this; }
    T operator++(int) { T n = obj_.value_; ++ref(); return n; }

    tx_obj& operator+=(T const &rhs) {
        ref() += rhs;
        return *this;
    }

    T operator+(T const &rhs) const {
        return ref()+rhs;
    }

    void swap(tx_obj & other) { // never throws
        std::swap(obj_, other.obj_);
    }

};

// two transactional pointers are equal if they point to the same cache on the current transaction.
template <typename T, typename U>
bool operator==(const tx_obj<T>& lhs, const tx_obj<U>& rhs) {
    return lhs.ref()==rhs.ref();
}

template <typename T, typename U>
bool operator==(const T& lhs, const tx_obj<U>& rhs) {
    return lhs==rhs.ref();
}

template <typename T, typename U>
bool operator==(const tx_obj<T>& lhs, const U& rhs) {
    return lhs.ref()==rhs;
}

template <typename T, typename U>
bool operator!=(const tx_obj<T>& lhs, const tx_obj<U>& rhs) {
    return lhs.ref()!=rhs.ref();
}

template<class T> inline void swap(tx_obj<T> & a, tx_obj<T> & b) {
    a.swap(b);
}

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

    tx_ptr(tx_obj<T>& r) : ptr_(r->obj_) {}

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
    tx_ptr& operator=(transactional_object<Y>* ptr)  { // never throws
        ptr_=ptr;
        return *this;
    }

    //bool operator==(const tx_ptr<T>& rhs) const {
    //    return ptr_==rhs.ptr_ || this->get()==rhs.ptr_ || ptr_==rhs.get();
    //}

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

    typedef transactional_object<T>* this_type::*unspecified_bool_type;

    operator unspecified_bool_type() const {
        return ptr_ == 0? 0: &this_type::ptr_;
    }
    void swap(tx_ptr & other) { // never throws
        std::swap(ptr_, other.ptr_);
    }
};

// two transactional pointers are equal if they point to the same cache on the current transaction.
template <typename T, typename U>
bool operator==(const tx_ptr<T>& lhs, const tx_ptr<U>& rhs) {
        return lhs.ptr_==rhs.ptr_ || lhs.get()==rhs.ptr_ || lhs.ptr_==rhs.get();
}

template <typename T, typename U>
bool operator!=(const tx_ptr<T>& lhs, const tx_ptr<U>& rhs) {
    return lhs.ptr_!=rhs.ptr_ && lhs.get()!=rhs.ptr_ && lhs.ptr_!=rhs.get();
}

template<class T> inline void swap(tx_ptr<T> & a, tx_ptr<T> & b) {
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
    transaction* tx=transaction::current_transaction();
    if (tx==0) delete ptr.ptr_;
    tx->delete_tx_ptr(ptr.ptr_);
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
static tx_ptr<T>* tx_dynamic_cast(tx_ptr<U> ptr) {
    return tx_ptr<T>(tx_dynamic_cast<T>(ptr->ptr_));
}


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
    typedef rd_ptr<T> this_type;
public:
    mutable transactional_object<T>* ptr_;

    inline rd_ptr(transaction &t, tx_ptr<T> tx_obj) :
        ptr_(&t.insert_and_return_read_memory(*tx_obj.ptr_))
    {}

    inline rd_ptr(transaction &t, tx_obj<T> const& tx_obj) :
        ptr_(&t.insert_and_return_read_memory(tx_obj.obj_))
    {}

    template<class Y>
    rd_ptr & operator=(tx_ptr<Y> r) { // never throws
        //this_type(r).swap(*this);
        ptr_=r.ptr_;
        return *this;
    }

    template<class Y>
    rd_ptr& operator=(tx_obj<Y> const & r) { // never throws
        //this_type(r).swap(*this);
        ptr_=r.get();
        return *this;
    }

    const T* get() const {
        return &ptr_->value;
    }

    inline const T & operator*() const { return *get(); }
    inline const T* operator->() const { return get(); }


    typedef transactional_object<T>* this_type::*unspecified_bool_type;

    operator unspecified_bool_type() const
    {
        return ptr_ == 0? 0: &this_type::ptr_;
    }

};

template <typename T>
rd_ptr<T> make_rd_ptr(transaction& tx, tx_ptr<T> ptr) {
    return rd_ptr<T>(tx, ptr);
}
template <typename T>
rd_ptr<T> make_rd_ptr(transaction& tx, tx_obj<T> const & ref) {
    return rd_ptr<T>(tx, ref);
}

template <typename T>
rd_ptr<T> make_rd_ptr(tx_ptr<T> ptr) {
    transaction* tx = transaction::current_transaction();
    assert(tx==0);
    return rd_ptr<T>(*tx, ptr);
}

template <typename T>
rd_ptr<T> make_rd_ptr(tx_obj<T> const & ref) {
    transaction* tx = transaction::current_transaction();
    assert(tx==0);
    return rd_ptr<T>(*tx, ref);
}

template <typename T>
void delete_ptr(rd_ptr<T> ptr) {
    if (ptr.ptr_==0) return;
    transaction* tx=transaction::current_transaction();
    if (tx==0) delete ptr.ptr_;
    tx->delete_tx_ptr(ptr.ptr_);
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
    mutable transaction* tx_;
    mutable transactional_object<T>* ptr_;
    mutable bool written_;

    //inline upgrd_ptr() : tx_(0) {}
    inline upgrd_ptr(transaction &t, tx_ptr<T> tx_obj) : tx_(&t),
        ptr_(const_cast<transactional_object<T>*>(t.read_ptr(tx_obj.ptr_))), written_(false)    {}

    template<class Y>
    upgrd_ptr & operator=(tx_ptr<Y> const&  r) { // never throws
        //this_type(r).swap(*this);
        transaction* tx=transaction::current_transaction();
        if (tx==0) throw "error";
        tx_=tx;
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

    operator unspecified_bool_type() const
    {
        return ptr_ == 0? 0: &this_type::ptr_;
    }
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

};

template <typename T>
void delete_ptr(upgrd_ptr<T> const& ptr) {
    if (ptr.ptr_==0) return;
    if (ptr.tx_==0) delete ptr.ptr_;
    ptr.tx_->delete_tx_ptr(ptr.ptr_);
}

template <typename T>
void delete_ptr(transaction& tx, upgrd_ptr<T> const& ptr) {
    if (ptr.ptr_==0) return;
    tx.delete_tx_ptr(ptr.ptr_);
}

//-----------------------------------------------------------------------------
// A wr_ptr<T> ("write pointer") points to a shared object that the current transaction has opened for writing.
// A wr_ptr<T> is initialized explicitly from an tx_ptr<T>.
// A wr_ptr<T> can also be explicitly constructed from a upgrd_ptr<T> as an upgrade-to-writable operation.
//-----------------------------------------------------------------------------

template <typename T>
class wr_ptr {
    typedef tx_ptr<T> this_type;
public:
    mutable transaction& tx_;
    mutable transactional_object<T>* ptr_;

    inline wr_ptr(transaction &t, tx_ptr<T> tx_obj) : tx_(t),
        ptr_(t.write_ptr(tx_obj.ptr_))
    {}

    inline wr_ptr(transaction &t, upgrd_ptr<T> tx_obj) : tx_(t),
        ptr_(t.write_ptr(tx_obj.ptr_))
    {
        tx_obj.write_ptr(ptr_);
    }

    T* get() {
        if (tx_.forced_to_abort()) {
            tx_.lock_and_abort();
            throw aborted_transaction_exception("aborting transaction");
        }
        return &ptr_->value;
    }

    inline T& operator*() { return *get(); }
    inline T* operator->() { return get(); }

    typedef transactional_object<T>* this_type::*unspecified_bool_type;

    operator unspecified_bool_type() const
    {
        return ptr_ == 0? 0: &this_type::ptr_;
    }
};

template <typename T>
wr_ptr<T> make_wr_ptr(transaction& tx, tx_ptr<T>& ptr) {
    return wr_ptr<T>(tx, ptr);
}


template <typename T>
wr_ptr<T> make_wr_ptr(tx_ptr<T>& ptr) {
    transaction* tx = transaction::current_transaction();
    return wr_ptr<T>(*tx, ptr);
}

template <typename T>
void delete_ptr(wr_ptr<T> ptr) {
    if (ptr.ptr_==0) return;
    ptr.tx_.delete_tx_ptr(ptr.ptr_);
}

template <typename T>
void delete_ptr(transaction& tx, wr_ptr<T> const& ptr) {
    if (ptr.ptr_==0) return;
    tx.delete_tx_ptr(ptr.ptr_);
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


