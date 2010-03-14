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

#ifndef BOOST_STM_TX_PROXY_CACHE__HPP
#define BOOST_STM_TX_PROXY_CACHE__HPP

//-----------------------------------------------------------------------------
#include <boost/stm/transaction.hpp>
#include <boost/stm/transaction_object.hpp>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
namespace boost { namespace stm { namespace tx {

//-----------------------------------------------------------------------------
template <typename Final, typename T, typename Base=base_transaction_object>
class proxy_cache : public transaction_object< Final, Base >
{
protected:
public:
    T val_;
public:
    typedef proxy_cache<Final, T, Base> this_type;
    typedef Final final_type;
    typedef T value_type;
    //-----------------------------------------------------------------------------
    proxy_cache() : val_() {}

    proxy_cache(proxy_cache const& r) : val_(r.value()) {}
    template<typename F, typename U>
    proxy_cache(proxy_cache<F,U> const& r) : val_(r.value()) {}
    proxy_cache(T v) : val_(v) {}
    // contructor from a convertible to T
    template <typename U>
    proxy_cache(U v) : val_(v) {}

    proxy_cache& operator=(proxy_cache const& rhs) {
        if (this!=&rhs) {
            ref()=rhs.value();
        }
        return *this;
    }
    template<typename F, typename U>
    proxy_cache& operator=(proxy_cache<F,U> const& rhs) {
        ref()=rhs.value();
        return *this;
    }
    template<typename F, typename U>
    proxy_cache& operator+=(proxy_cache<F,U> const& rhs) {
        ref()+=rhs.value();
        return *this;
    }
    template<typename U>
    proxy_cache& operator+=(U const& rhs) {
        ref()+=rhs;
        return *this;
    }

    template<typename F, typename U>
    proxy_cache& operator-=(proxy_cache<F,U> const& rhs) {
        ref()-=rhs.value();
        return *this;
    }
    template<typename U>
    proxy_cache& operator-=(U const& rhs) {
        ref()-=rhs;
        return *this;
    }

    template<typename F, typename U>
    proxy_cache& operator*=(proxy_cache<F,U> const& rhs) {
        ref()*=rhs.value();
        return *this;
    }
    template<typename U>
    proxy_cache& operator*=(U const& rhs) {
        ref()*=rhs;
        return *this;
    }

    template<typename F, typename U>
    proxy_cache& operator/=(proxy_cache<F,U> const& rhs) {
        ref()/=rhs.value();
        return *this;
    }
    template<typename U>
    proxy_cache& operator/=(U const& rhs) {
        ref()/=rhs;
        return *this;
    }

    template<typename F, typename U>
    proxy_cache& operator%=(proxy_cache<F,U> const& rhs) {
        ref()%=rhs.value();
        return *this;
    }
    template<typename U>
    proxy_cache& operator%=(U const& rhs) {
        ref()%=rhs;
        return *this;
    }

    template<typename F, typename U>
    proxy_cache& operator^=(proxy_cache<F,U> const& rhs) {
        ref()^=rhs.value();
        return *this;
    }
    template<typename U>
    proxy_cache& operator^=(U const& rhs) {
        ref()^=rhs;
        return *this;
    }

    template<typename F, typename U>
    proxy_cache& operator&=(proxy_cache<F,U> const& rhs) {
        ref()&=rhs.value();
        return *this;
    }
    template<typename U>
    proxy_cache& operator&=(U const& rhs) {
        ref()&=rhs;
        return *this;
    }

    template<typename F, typename U>
    proxy_cache& operator|=(proxy_cache<F,U> const& rhs) {
        ref()|=rhs.value();
        return *this;
    }
    template<typename U>
    proxy_cache& operator|=(U const& rhs) {
        ref()|=rhs;
        return *this;
    }

    template<typename F, typename U>
    proxy_cache& operator>>=(proxy_cache<F,U> const& rhs) {
        ref()>>=rhs.value();
        return *this;
    }
    template<typename U>
    proxy_cache& operator>>=(U const& rhs) {
        ref()>>=rhs;
        return *this;
    }

    template<typename F, typename U>
    proxy_cache& operator<<=(proxy_cache<F,U> const& rhs) {
        ref()<<=rhs.value();
        return *this;
    }
    template<typename U>
    proxy_cache& operator<<=(U const& rhs) {
        ref()<<=rhs;
        return *this;
    }


    proxy_cache& operator++() {
        ++ref();
        return *this;
    }

    proxy_cache operator++(int) {
        ref()++;
        return *this;
    }

    proxy_cache& operator--() {
        --ref();
        return *this;
    }
    proxy_cache  operator--(int) {
        ref()--;
        return *this;
    }

    operator T() const { return value(); }
    //operator T&() { return ref(); }

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
    proxy_cache(proxy_cache const& rhs, stm::shallow_t)
    : val_(rhs.val_)
    {}
    // shallow assignment
    proxy_cache& shallow_assign(proxy_cache const& rhs)
    {
        val_=rhs.val_;
        return *this;
    }
    // TODO add case has_shallow_copy_semansics<T> is true
};

template <typename Final, typename T, typename Base>
typename proxy_cache<Final,T,Base>::value_type&
write(proxy_cache<Final,T,Base>& r) {
    return r.ref();
}

template <typename Final, typename T, typename Base>
typename proxy_cache<Final,T,Base>::value_type*
write(proxy_cache<Final,T,Base>* r) {
    return &r->ref();
}

template <typename Final, typename T, typename Base>
typename proxy_cache<Final,T,Base>::value_type
read(proxy_cache<Final,T,Base> const& r) {
    return r.value();
}

template <typename Final, typename T, typename Base>
typename proxy_cache<Final,T,Base>::value_type*
read(proxy_cache<Final,T,Base> const* r) {
    return r->value();
}

template <typename OSTREAM, typename F, typename T, typename B>
OSTREAM& operator<<(OSTREAM& os, proxy_cache<F, T, B> const& r) {
    os << r.value();
    return os;
}
template <typename ISTREAM, typename F, typename T, typename B>
ISTREAM& operator>>(ISTREAM& is, proxy_cache<F, T, B> & r) {
    T v;
    is >> v;
    r=v;
    return is;
}


}
// shallow trait
template <typename F, typename T, typename B>
struct has_shallow_copy_semantics<tx::proxy_cache<F,T,B> > : boost::mpl::true_
{};


}}
#endif //BOOST_STM_TX_PROXY_CACHE__HPP


