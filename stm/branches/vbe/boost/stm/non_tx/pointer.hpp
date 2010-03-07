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
#include <boost/stm/non_tx/reference.hpp>
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
class pointer : public reference< pointer<T> ,T* >
{
    typedef reference< pointer<T> , T* > base_type;
    
public:
    //-----------------------------------------------------------------------------
    pointer() : base_type(static_cast<T*>(0)) {}
    template<class U>
    pointer(pointer<U> const& r) : base_type(r) {}
    template <typename U>
    pointer(U* v) : base_type(v) {}
    //pointer(T* v) : base_type(v) {}

    T* operator->() const {
        return this->value();
    }
    T& operator*() const {
        return *this->value();
    }

    T* operator->() {
        return this->ref();
    }
    T& operator*() {
        return *this->ref();
    }

};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template <typename C, typename R>
class pointer_to_member : public reference< pointer_to_member<C,R>, R C::* >
{
    typedef reference< pointer_to_member<C,R>, R C::*> base_type;
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


