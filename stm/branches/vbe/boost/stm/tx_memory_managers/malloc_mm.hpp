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

#ifndef BOOST_STM_TX_ALLOC_MM__HPP
#define BOOST_STM_TX_ALLOC_MM__HPP

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
namespace boost { namespace stm {
class transaction;

struct malloc_mm {
    template <class T> 
    static T* allocate(transaction& t) {
        T* res= reinterpret_cast<T*>(malloc(sizeof(T)));
        if (res==0) throw std::bad_alloc();
        return res;
    }

    template <class T>
    static void deallocate(T* ptr) {
        if (ptr) {
            free(ptr);
        }
    }

};
} // stm  namespace
} // boost namespace

///////////////////////////////////////////////////////////////////////////////
#endif // BOOST_STM_TX_ALLOC_MM__HPP


