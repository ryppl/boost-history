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

#ifndef BOOST_STM_TX_OBJECT__HPP
#define BOOST_STM_TX_OBJECT__HPP

//-----------------------------------------------------------------------------
#include <boost/stm/tx/mixin.hpp>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
namespace boost { namespace stm { namespace tx {

//-----------------------------------------------------------------------------
// transactional object class that wraps a object type providing
// a transparent transactional view on a transactional context
// a non-transactional view on a non-transactional context
// Note: the sizeof(object<T>)>>>>=sizeof(T)
//-----------------------------------------------------------------------------
template <typename T>
class object : public mixin< object<T>, T >
{
public:
    typedef mixin< object<T>, T > base_type;
    //-----------------------------------------------------------------------------
    object() : base_type() {}
    template<typename U>
    object(object<U> const& r) : base_type(r) {}
    object(object const& r) : base_type(r) {}
    // contructor from a convertible to T
    template <typename U>
    object(U v) : base_type(v) {}
    object(T v) : base_type(v) {}
    // shallow copy
    object(object const& rhs, stm::shallow_t)
    : base_type(rhs, stm::shallow)
    {}
    // shallow assignment
    object& shallow_assign(object const& rhs)
    {
        this->base_type::shallow_assign(rhs);
        return *this;
    }

};


}
// shallow trait
template <typename T>
struct has_shallow_copy_semantics<tx::object<T> > : boost::mpl::true_
{};

}}
#endif //BOOST_STM_TX_OBJECT__HPP


