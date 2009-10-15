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

#ifndef BOOST_STM_TX_POINTER__HPP
#define BOOST_STM_TX_POINTER__HPP

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <boost/stm/transaction.hpp>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
namespace boost { namespace stm { namespace tx {

//-----------------------------------------------------------------------------
// class pointer wraps a transactional_object providing builting operators
//-----------------------------------------------------------------------------

template <typename T>
class pointer : public transaction_object< pointer<T> >
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

            return tx->write(*this).val_;
        }
        return val_;
    }

    T const * get() const {
        transaction* tx=current_transaction();
        if (tx!=0) {
            if (tx->forced_to_abort()) {
                tx->lock_and_abort();
                throw aborted_transaction_exception("aborting transaction");
            }
            return tx->read(*this).val_;
        }
        return val_;
    }

    T const * operator->() const {
        return this->get();
    }
    T const & operator*() const {
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
class pointer_to_member : public transaction_object< pointer_to_member<C,R> >
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

            return tx->write(*this).val_;
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
            return tx->read(*this).val_;
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


