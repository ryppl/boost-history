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

#ifndef BOOST_STM_NON_TX_SMART_PTR__HPP
#define BOOST_STM_NON_TX_SMART_PTR__HPP

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <assert.h>
#include <map>
#include <boost/stm/transaction.hpp>
#include <boost/stm/non_tx/detail/cache_map.hpp>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
namespace boost { namespace stm {

namespace non_tx {

template <typename T>
class rd_ptr;
template <typename T>
class wr_ptr;
template <typename T>
class upgrd_ptr;

//-----------------------------------------------------------------------------
// A rd_ptr<T> ("read pointer") points to an object that the current
// transaction has opened for read only access.
// You can only call a const method through a read pointer.
// A rd_ptr<T> is constructed from an T pointer or reference.
// Once a rd_ptr<T> has been constructed, an cache is opened for
// reading.
// It is not safe to derreference a rd_ptr<T> after having assigned the same
// tx_ptr<T> to a wr_ptr<T>. If this is the case the readen value do not match
// the writen one. If it is possible to write on the same transaction use
// upgrd_ptr instead which is safe.
//-----------------------------------------------------------------------------


template <typename T>
class rd_ptr {
    typedef rd_ptr<T> this_type;
public:
    mutable transaction& tx_;
    mutable detail::cache<T>* ptr_;

    inline rd_ptr(transaction &t, T const * ptr) : tx_(t),
        ptr_(&t.insert_and_return_read_memory(*detail::cache_map::get(ptr)))
    {}

    inline rd_ptr(transaction &t, T const & obj) : tx_(t),
        ptr_(&t.insert_and_return_read_memory(*detail::cache_map::get(&obj)))
    {}

    template<class Y>
    inline rd_ptr & operator=(Y const* ptr) { // never throws
        ptr_=tx_.insert_and_return_read_memory(*detail::cache_map::get(ptr));
        return *this;
    }

    template<class Y>
    inline rd_ptr & operator=(Y const& ref) { // never throws
        ptr_=tx_.insert_and_return_read_memory(*detail::cache_map::get(&ref));
        return *this;
    }

    const T* get() const {
        if (tx_.forced_to_abort()) {
            tx_.lock_and_abort();
            throw aborted_transaction_exception("aborting transaction");
        }
        return ptr_->get();
    }

    inline const T & operator*() const { return *get(); }
    inline const T* operator->() const { return get(); }


    typedef detail::cache<T>* this_type::*unspecified_bool_type;

    inline operator unspecified_bool_type() const {
        return ptr_ == 0? 0: &this_type::ptr_;
    }

};

template <typename T>
inline rd_ptr<T> make_rd_ptr(transaction& tx, T* ptr) {
    return rd_ptr<T>(tx, ptr);
}

template <typename T>
inline rd_ptr<T> make_rd_ptr(transaction& tx, T& ref) {
    return rd_ptr<T>(tx, ref);
}

template <typename T>
inline rd_ptr<T> make_rd_ptr(T* ptr) {
    transaction* tx = current_transaction();
    assert(tx==0);
    return rd_ptr<T>(*tx, ptr);
}

template <typename T>
inline rd_ptr<T> make_rd_ptr(T& ref) {
    transaction* tx = current_transaction();
    assert(tx==0);
    return rd_ptr<T>(*tx, ref);
}

template <typename T>
inline void delete_ptr(rd_ptr<T> ptr) {
    if (ptr.ptr_==0) return;
    ptr.tx_->delete_tx_ptr(ptr.ptr_);
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
    typedef upgrd_ptr<T> this_type;
public:
    mutable transaction* tx_;
    mutable detail::cache<T>* ptr_;
    mutable bool written_;

    inline upgrd_ptr(transaction &t, T* ptr) : tx_(&t),
        ptr_(const_cast<detail::cache<T>*>(t.read_ptr(detail::cache_map::get(ptr)))), written_(false)    {}

    inline upgrd_ptr(transaction &t, T& ref) : tx_(&t),
        ptr_(const_cast<detail::cache<T>*>(t.read_ptr(detail::cache_map::get(&ref)))), written_(false)    {}

    template<class Y>
    inline upgrd_ptr & operator=(Y* ptr) {
        ptr_=const_cast<detail::cache<T>*>(tx_->read_ptr(detail::cache_map::get(ptr)));
        return *this;
    }

    template<class Y>
    inline upgrd_ptr & operator=(Y& ref) {
        ptr_=const_cast<detail::cache<T>*>(tx_->read_ptr(detail::cache_map::get(&ref)));
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
            detail::cache<T>* temp = tx_->get_written(*ptr_);

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

    typedef detail::cache<T>* this_type::*unspecified_bool_type;

    inline operator unspecified_bool_type() const {
        return ptr_ == 0? 0: &this_type::ptr_;
    }
    inline void write_ptr(detail::cache<T>* ptr) {
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

        detail::cache<T>* temp = tx_->get_written(ptr_);

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
inline void delete_ptr(upgrd_ptr<T> const& ptr) {
    if (ptr.ptr_==0) return;
    if (ptr.tx_==0) delete ptr.ptr_;
    ptr.tx_->delete_tx_ptr(ptr.ptr_);
}

template <typename T>
inline void delete_ptr(transaction& tx, upgrd_ptr<T> const& ptr) {
    if (ptr.ptr_==0) return;
    tx.delete_tx_ptr(ptr.ptr_);
}

template <typename T>
class wr_ptr {
    typedef wr_ptr<T> this_type;
public:
    mutable transaction& tx_;
    mutable detail::cache<T>* ptr_;

    inline wr_ptr(transaction &t, T* ptr) : tx_(t),
        ptr_(t.write_ptr(detail::cache_map::get(ptr)))
    {}

    inline wr_ptr(transaction &t, T& obj) : tx_(t),
        ptr_(t.write_ptr(detail::cache_map::get(&obj)))
    {}


    T* get() {
        if (tx_.forced_to_abort()) {
            tx_.lock_and_abort();
            throw aborted_transaction_exception("aborting transaction");
        }
        return ptr_->get();
    }

    inline T& operator*() { return *get(); }
    inline T* operator->() { return get(); }

    typedef detail::cache<T>* this_type::*unspecified_bool_type;

    inline operator unspecified_bool_type() const {
        return ptr_ == 0? 0: &this_type::ptr_;
    }
};


}
}}
#endif


