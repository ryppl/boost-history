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

#ifndef BOOST_STM_NON_TX_OBJECT__HPP
#define BOOST_STM_NON_TX_OBJECT__HPP

//-----------------------------------------------------------------------------
#include <boost/stm/non_tx/reference.hpp>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
namespace boost { namespace stm { namespace non_tx {

//-----------------------------------------------------------------------------
// class object wraps a non object type providing
// a transactional view on a transactional context
// a non-transactional view on a non-transactional context
// Note: the sizeof(object<T>)==sizeof(T)
//-----------------------------------------------------------------------------

template <typename T>
struct object : reference< object<T>, T> {
public:
    typedef reference< object<T>, T > base_type;
    //-----------------------------------------------------------------------------
    object() : base_type() {}

    // constructors
    template<typename F, typename U>
    object(object<U> const& r) : base_type(r) {}

    // contructor from a convertible to T
    template <typename U>
    object(U v) :base_type(v) {}
};

}}}
#endif


