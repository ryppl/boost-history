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
//-----------------------------------------------------------------------------
#include <assert.h>
#include <boost/stm/transaction.hpp>
#include <boost/stm/non_tx/detail/cache_map.hpp>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
namespace boost { namespace stm {
namespace non_tx {

template <typename T>
class numeric;
class chat_t;
class uchat_t;
class short_t;
class ushort_t;
class int_t;
class uint_t;
class long_t;
class ulong_t;

//-----------------------------------------------------------------------------
// class numeric wraps a transactional_object providing builting operators
//-----------------------------------------------------------------------------

template <typename T>
class numeric {
public:
    T val_;
    //-----------------------------------------------------------------------------
    numeric() : val_(0) {}

    //
    template<class U>
    numeric(numeric<U> const& r) : val(r.value()) {}

    // contructor from a implicitly convertible to T
    template <typename U>
    numeric(U v) : val_(v) {}
    //
    ~numeric() {}

    operator T() const { return value(); }
    operator T&() { return ref(); }

    T& ref() {
        transaction* tx=transaction::current_transaction();
        if (tx!=0) {
            if (tx->forced_to_abort()) {
                tx->lock_and_abort();
                throw aborted_transaction_exception("aborting transaction");
            }

            detail::cache<T>* r(tx.write_ptr(detail::cache_map::get(&val_)));
            return *(r->get());
        }
        return val_;
    }

    T value() const {
        transaction* tx=transaction::current_transaction();
        if (tx!=0) {
            if (tx->forced_to_abort()) {
                tx->lock_and_abort();
                throw aborted_transaction_exception("aborting transaction");
            }
            detail::cache<T>* r(tx.read_ptr(detail::cache_map::get(&val_)));
            return *(r->get());
        }
        return val_;
    }

#if 0
    numeric& operator--() { --ref(); return *this; }
    T operator--(int) { T n = val_; --ref(); return n; }

    numeric& operator++() { ++ref(); return *this; }
    T operator++(int) { T n = val_; ++ref(); return n; }

    template <typename U>
    numeric& operator+=(U const &rhs) {
        ref() += rhs;
        return *this;
    }

    template <typename U>
    numeric& operator-=(U const &rhs) {
        ref() -= rhs;
        return *this;
    }

    template <typename U>
    T operator+(U const &rhs) const {
        return value()+u;
    }

    template <typename U>
    T operator-(U const &rhs) const {
        return value()+u;
    }
    // as lvalue
    template <typename U>
    numeric& operator=(U& u) {
        ref()=u;
        return *this;
    }
    void swap(numeric & other) { // never throws
        std::swap(obj_, other.obj_);
    }
#endif

};
#if 0

// two transactional pointers are equal if they point to the same cache on the current transaction.
template <typename T, typename U>
inline bool operator==(const numeric<T>& lhs, const numeric<U>& rhs) {
    return lhs.ref()==rhs.ref();
}

template <typename T, typename U>
inline bool operator==(const T& lhs, const numeric<U>& rhs) {
    return lhs==rhs.ref();
}

template <typename T, typename U>
inline bool operator==(const numeric<T>& lhs, const U& rhs) {
    return lhs.ref()==rhs;
}

template <typename T, typename U>
inline bool operator!=(const numeric<T>& lhs, const numeric<U>& rhs) {
    return lhs.ref()!=rhs.ref();
}

template<class T> inline void swap(numeric<T> & a, numeric<T> & b) {
    a.swap(b);
}
#endif

}}}
#endif


