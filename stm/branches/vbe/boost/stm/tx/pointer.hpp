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
#include <boost/stm/tx/proxy_cache.hpp>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
namespace boost { namespace stm { namespace tx {

//-----------------------------------------------------------------------------
// smart pointer providing typycal smart pointer operators +
// a transparent transactional view on a transactional context.
// a non-transactional view on a non-transactional context
// Note: the sizeof(pointer<T>)>>>>=sizeof(T*)
//-----------------------------------------------------------------------------
template <typename T>
class pointer : public proxy_cache< pointer<T>, T* >
{
    typedef proxy_cache< pointer<T>, T* > base_type;

public:
    //-----------------------------------------------------------------------------
    pointer() : base_type(static_cast<T*>(0)) {}
    pointer(pointer const& r) : base_type(r) {}
    template<class U>
    pointer(pointer<U> const& r) : base_type(r) {}

    template<class U, class V, class B>
    pointer(proxy_cache<U, V*, B> const& rhs) : base_type(rhs.value()) {}

    pointer(T* v) : base_type(v) {}
    template <typename U>
    pointer(U* v) : base_type(v) {}

    template<class U, class V, class B>
    pointer& operator=(proxy_cache<U, V*, B> const& rhs) {
        this->val_=rhs.value();
    };

    T* operator->() const {
        return this->value();
    }
    T& operator*() const {
        return *this->value();
    }

    #if 0
    T* operator->() {
        return this->ref();
    }
    T& operator*() {
        return *this->ref();
    }
    #endif

    // shallow copy
    pointer(pointer const& rhs, stm::shallow_t)
    : base_type(rhs, stm::shallow)
    {}
    // shallow assignment
    pointer& shallow_assign(pointer const& rhs)
    {
        this->base_type::shallow_assign(rhs);
        return *this;
    }

};
}
// shallow trait
template <typename T>
struct has_shallow_copy_semantics<tx::pointer<T> > : boost::mpl::true_
{};

namespace tx {

template <typename T>
pointer<T> address_of(object<T>& obj) {
    return pointer<T>(obj.address_of());
}

template <typename C, typename R>
class pointer_to_member : public proxy_cache< pointer_to_member<C,R>, R C::*>
{
    typedef proxy_cache< pointer_to_member<C,R>, R C::*> base_type;
public:
    //-----------------------------------------------------------------------------
    pointer_to_member() : base_type(static_cast<R C::*>(0)) {}

    template <typename D, typename S>
    pointer_to_member(pointer_to_member<D,S> const& r) : base_type(r) {}
    template <typename D, typename S>
    pointer_to_member(S D::* v) : base_type(v) {}
    //pointer_to_member(R C::* v) : base_type(v) {}
};

}}}
#endif


