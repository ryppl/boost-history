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

#ifndef BOOST_STM_NON_TX_MIXIN__HPP
#define BOOST_STM_NON_TX_MIXIN__HPP

//-----------------------------------------------------------------------------
#include <boost/stm/transaction.hpp>
#include <boost/stm/non_tx/detail/cache_map.hpp>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
namespace boost { namespace stm { namespace non_tx {

//-----------------------------------------------------------------------------
// class object wraps a non object type providing
// a transactional view on a transactional context
// a non-transactional view on a non-transactional context
// Note: the sizeof(object<T>)==sizeof(T)
//-----------------------------------------------------------------------------

template <typename Final, typename T>
class mixin {
protected:
    T& val_;
public:
    //-----------------------------------------------------------------------------
    mixin() : val_() {}

    // constructors
    template<typename F, typename U>
    mixin(mixin<F,U> const& r) : val_(r.value()) {}

    // contructor from a convertible to T
    template <typename U>
    mixin(U v) : val_(v) {}

    //-----------------------------------------------------------------------------
    // accessors        
    operator T() const { return value(); }
    operator T&() { return ref(); }

    //-----------------------------------------------------------------------------
    T& ref() {
        transaction* tx=current_transaction();
        if (tx!=0) {
            if (tx->forced_to_abort()) {
                tx->lock_and_abort();
                throw aborted_transaction_exception("aborting transaction");
            }

            detail::cache<T>* r(tx->write_ptr(detail::cache_map::get(&val_)));
            return *(r->get());
        }
        return val_;
    }

    //-----------------------------------------------------------------------------
    T value() const {
        transaction* tx=current_transaction();
        if (tx!=0) {
            if (tx->forced_to_abort()) {
                tx->lock_and_abort();
                throw aborted_transaction_exception("aborting transaction");
            }
            detail::cache<T>* r(tx->read_ptr(detail::cache_map::get(&val_)));
            return *(r->get());
        }
        return val_;
    }
};

}}}
#endif


