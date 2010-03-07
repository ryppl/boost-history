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
public:
    T val_;
public:
    typedef mixin<Final, T, Base> this_type;
    typedef Final final_type;
    typedef T value_type;
    //-----------------------------------------------------------------------------
    mixin() : val_() {}

    mixin(mixin const& r) : val_(r.value()) {}
    template<typename F, typename U>
    mixin(mixin<F,U> const& r) : val_(r.value()) {}
    mixin(T v) : val_(v) {}
    mixin& operator=(mixin const& rhs) {
        if (this!=&rhs) {
            ref()=rhs.value();
        }
        return *this;
    }

    // contructor from a convertible to T
    template <typename U>
    mixin(U v) : val_(v) {}

    operator T() const { return value(); }
    operator T&() { return ref(); }

    //-----------------------------------------------------------------------------
    // accessors
    T& ref() {
        transaction* tx=current_transaction();
        if (tx!=0 && tx->in_flight()) {
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
        if (tx!=0 && tx->in_flight()) {
            if (tx->forced_to_abort()) {
                tx->lock_and_abort();
                throw aborted_transaction_exception("aborting transaction");
            }
            return tx->read(*static_cast<Final const*>(this)).val_;
        }
        return val_;
    }
    // shallow copy
    mixin(mixin const& rhs, stm::shallow_t)
    : val_(rhs.val_)
    {}
    // shallow assignment
    mixin& shallow_assign(mixin const& rhs)
    {
        val_=rhs.val_;
        return *this;
    }
};



}
// shallow trait
template <typename F, typename T, typename B>
struct has_shallow_copy_semantics<tx::mixin<F,T,B> > : boost::mpl::true_
{};

}}
#endif //BOOST_STM_TX_MIXIN__HPP


