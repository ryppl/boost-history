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

#ifndef BOOST_STM_NON_TX_POINTER__HPP
#define BOOST_STM_NON_TX_POINTER__HPP

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <boost/stm/transaction.hpp>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
namespace boost { namespace stm { namespace non_tx {

//-----------------------------------------------------------------------------
// smart pointer class wraps a pointer providing 
// typycal smart pointer operators +
// a transactional view on a transactional context
// a non-transactional view on a non-transactional context
// Note: the sizeof(pointer<T>)==sizeof(T*)
//-----------------------------------------------------------------------------

template <typename T>
class pointer
{
protected:
    T* val_;
public:
    //-----------------------------------------------------------------------------
    pointer() : val_(0) {}

    //
    template<class U>
    pointer(pointer<U> const& r) : val_(r.value()) {}

    // contructor from a implicitly convertible to T
    template <typename U>
    pointer(U* v) : val_(v) {}
    pointer(T* v) : val_(v) {}
    //
    ~pointer() {}

    operator T*() const { return get(); }
    operator T*&() { return get(); }

    T*& get() {
        transaction* tx=current_transaction();
        if (tx!=0) {
            if (tx->forced_to_abort()) {
                tx->lock_and_abort();
                throw aborted_transaction_exception("aborting transaction");
            }
            detail::cache<T*>* r(tx->write_ptr(detail::cache_map::get(&val_)));
            return *(r->get());

        }
        return val_;
    }

    T* get() const {
        transaction* tx=current_transaction();
        if (tx!=0) {
            if (tx->forced_to_abort()) {
                tx->lock_and_abort();
                throw aborted_transaction_exception("aborting transaction");
            }
            return tx->read(*this).val_;
            detail::cache<T*>* r(tx->read_ptr(detail::cache_map::get(&val_)));
            return *(r->get());
        }
        return val_;
    }

    T* operator->() const {
        return this->get();
    }
    T & operator*() const {
        return *this->get();
    }

    T * operator->() {
        return this->get();
    }
    T & operator*() {
        return *this->get();
    }

};

template <typename C, typename R>
class pointer_to_member
{
protected:
    R C::* val_;
public:
    //-----------------------------------------------------------------------------
    pointer_to_member() : val_(0) {}

    //
    pointer_to_member(pointer_to_member const& r) : val_(r.value()) {}

    // contructor from a implicitly convertible to T
    pointer_to_member(R C::* v) : val_(v) {}
    //

    operator R C::*() const { return get(); }
    operator R C::*&() { return get(); }

    R C::*& get() {
        transaction* tx=current_transaction();
        if (tx!=0) {
            if (tx->forced_to_abort()) {
                tx->lock_and_abort();
                throw aborted_transaction_exception("aborting transaction");
            }

            detail::cache<R C::*>* r(tx->write_ptr(detail::cache_map::get(&val_)));
            return *(r->get());
        }
        return val_;
    }

    R C::* const * get() const {
        transaction* tx=current_transaction();
        if (tx!=0) {
            if (tx->forced_to_abort()) {
                tx->lock_and_abort();
                throw aborted_transaction_exception("aborting transaction");
            }
            detail::cache<R C::*>* r(tx->read_ptr(detail::cache_map::get(&val_)));
            return *(r->get());
        }
        return val_;
    }

};

#if 0

// two transactional pointers are equal if they point to the same cache on the current transaction.
template <typename T, typename U>
inline bool operator==(const pointer<T>& lhs, const pointer<U>& rhs) {
    return lhs.ref()==rhs.ref();
}

template <typename T, typename U>
inline bool operator==(const T& lhs, const pointer<U>& rhs) {
    return lhs==rhs.ref();
}

template <typename T, typename U>
inline bool operator==(const pointer<T>& lhs, const U& rhs) {
    return lhs.ref()==rhs;
}

template <typename T, typename U>
inline bool operator!=(const pointer<T>& lhs, const pointer<U>& rhs) {
    return lhs.ref()!=rhs.ref();
}

template<class T> inline void swap(pointer<T> & a, pointer<T> & b) {
    a.swap(b);
}
#endif

}}}
#endif


