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
#include <boost/stm/tx/mixin.hpp>
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
class pointer : public mixin< pointer<T> ,T* >
{
    typedef mixin< pointer<T> , T* > base_type;
    
public:
    //-----------------------------------------------------------------------------
    pointer() : base_type(static_cast<T*>(0)) {
            //std::cerr << __LINE__ << " pointer" << std::endl;        
    }
    //pointer(pointer const& r) : base_type(*((base_type const*)(&r))) {
    pointer(pointer const& r) : base_type(r) {
            //std::cerr << __LINE__ << " pointer" << std::endl;        
    }
    template<class U>
    pointer(pointer<U> const& r) : base_type(r) {
            //std::cerr << __LINE__ << " pointer" << std::endl;        
    }
    
    pointer(T* v) : base_type(v) {
            //std::cerr << __LINE__ << " pointer" << std::endl;        
    }
    template <typename U>
    pointer(U* v) : base_type(v) {
            //std::cerr << __LINE__ << " pointer" << std::endl;        
    }

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

template <typename C, typename R>
class pointer_to_member : public mixin< pointer_to_member<C,R>, R C::*>
{
    typedef mixin< pointer_to_member<C,R>, R C::*> base_type;
public:
    //-----------------------------------------------------------------------------
    pointer_to_member() : base_type(static_cast<R C::*>(0)) {}

    template <typename D, typename S>
    pointer_to_member(pointer_to_member<D,S> const& r) : base_type(r) {}
    template <typename D, typename S>
    pointer_to_member(S D::* v) : base_type(v) {}
    //pointer_to_member(R C::* v) : base_type(v) {}
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


