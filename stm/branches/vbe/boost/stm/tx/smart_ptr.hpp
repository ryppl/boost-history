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

#ifndef BOOST_STM_TX_SMART_PTR__HPP
#define BOOST_STM_TX_SMART_PTR__HPP

//-----------------------------------------------------------------------------
#include <boost/stm/tx_ptr.hpp>
namespace boost { namespace stm { namespace tx {

namespace detail {
    template <typename T, bool IsConst>
    struct open;

    template <typename T>
    struct open<T, true> {
        T* apply(transaction& tx, T * ptr) {
            return tx.read_ptr(ptr);
        }
    };
    template <typename T>
    struct open<T, false> {
        T* apply(transaction& tx, T * ptr) {
            return tx.write_ptr(ptr);
        }
    };
}
template <typename T>
class tx_ptr {
public:
    typedef tx_ptr<T> this_type;
    T * ptr_;

    tx_ptr() : ptr_(0) {}
    template<class Y>
    explicit tx_ptr(Y * ptr) : ptr_(ptr) {}

    template<class Y>
    tx_ptr(tx_ptr<Y> const& r) : ptr_(r.ptr_) {}// never throws

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
        transaction* tx=current_transaction();
        if (tx!=0) {
            if (tx->forced_to_abort()) {
                    tx->lock_and_abort();
                    throw aborted_transaction_exception("aborting transaction");
            }
            return detail::open<T>::apply(*tx,ptr_);
        }
        return ptr_;
    }

    typedef transactional_object<T>* this_type::*unspecified_bool_type;

    operator unspecified_bool_type() const {
        return ptr_ == 0? 0: &this_type::ptr_;
    }
    void swap(tx_ptr & other) { // never throws
        std::swap(ptr_, other.ptr_);
    }
};
}}}
//-----------------------------------------------------------------------------
#endif // BOOST_STM_TX_SMART_PTR__HPP


