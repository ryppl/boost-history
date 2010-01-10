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

#ifndef BOOST_STM_TX_MIXIN__HPP
#define BOOST_STM_TX_MIXIN__HPP

//-----------------------------------------------------------------------------
#include <boost/stm/transaction.hpp>
#include <boost/stm/transaction_object.hpp>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
namespace boost { namespace stm { namespace tx {

//-----------------------------------------------------------------------------
// mixin transactional object class that wraps a object type providing
// a transparent transactional view on a transactional context
// a non-transactional view on a non-transactional context
// Note: the sizeof(object<T>)>>>>=sizeof(T)
//-----------------------------------------------------------------------------
template <typename Final, typename T, typename Base=base_transaction_object>
class mixin : public transaction_object< Final, Base >
{
protected:
    T val_;
public:
    typedef mixin<Final, T, Base> this_type;
    typedef Final final_type;
    typedef T value_type;
    //-----------------------------------------------------------------------------
    mixin() : val_() {
            //std::cerr << __LINE__ << " mixin val_=" << val_ << std::endl;
    }

    mixin(mixin const& r) : val_(r.value()) {
            //std::cerr << __LINE__ << " mixin val_=" << val_ << std::endl;
    }
    template<typename F, typename U>
    mixin(mixin<F,U> const& r) : val_(r.value()) {
            //std::cerr << __LINE__ << " mixin val_=" << val_ << std::endl;
    }

    mixin(T v) : val_(v) {
            //std::cerr << __LINE__ << " mixin val_=" << v << std::endl;
    }
    // contructor from a convertible to T
    template <typename U>
    mixin(U v) : val_(v) {
            //std::cerr << __LINE__ << " mixin val_=" << v << std::endl;
    }

    operator T() const { return value(); }
    operator T&() { return ref(); }

    //-----------------------------------------------------------------------------
    // accessors
    T& ref() {
        transaction* tx=current_transaction();
        if (tx!=0) {
            if (tx->forced_to_abort()) {
                tx->lock_and_abort();
                throw aborted_transaction_exception("aborting transaction");
            }

            return tx->write(*static_cast<Final*>(this)).val_;
        }
        return val_;
    }

    T* address_of() {
        return &val_;
    }
    //-----------------------------------------------------------------------------
    T value() const {
        transaction* tx=current_transaction();
        if (tx!=0) {
            if (tx->forced_to_abort()) {
                tx->lock_and_abort();
                throw aborted_transaction_exception("aborting transaction");
            }
            return tx->read(*static_cast<Final const*>(this)).val_;
        }
        return val_;
    }
};


}}}
#endif //BOOST_STM_TX_MIXIN__HPP


