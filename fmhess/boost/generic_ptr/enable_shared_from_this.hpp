#ifndef BOOST_GENERIC_PTR_ENABLE_SHARED_FROM_THIS_HPP_INCLUDED
#define BOOST_GENERIC_PTR_ENABLE_SHARED_FROM_THIS_HPP_INCLUDED

//
//  generic_ptr::enable_shared_from_this.hpp
//
//  Copyright 2002, 2009 Peter Dimov
//  Copyright 2009 Frank Mori Hess
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//  http://www.boost.org/libs/generic_ptr
//

#include <boost/generic_ptr/weak.hpp>
#include <boost/generic_ptr/shared.hpp>
#include <boost/assert.hpp>
#include <boost/config.hpp>

namespace boost
{
namespace generic_ptr
{

template<class T> class enable_shared_from_this
{
protected:

    enable_shared_from_this()
    {
    }

    enable_shared_from_this(enable_shared_from_this const &)
    {
    }

    enable_shared_from_this & operator=(enable_shared_from_this const &)
    {
        return *this;
    }

    ~enable_shared_from_this()
    {
    }

public:

    shared<T> shared_from_this()
    {
        shared<T> p( weak_this_ );
        BOOST_ASSERT( get_plain_old_pointer(p) == this );
        return p;
    }

    typename shared<T>::template rebind<typename shared<T>::value_type const>::other shared_from_this() const
    {
        typename shared<T>::template rebind<typename shared<T>::value_type const>::other p( weak_this_ );
        BOOST_ASSERT( get_plain_old_pointer(p) == this );
        return p;
    }

public: // actually private, but avoids compiler template friendship issues

    // Note: invoked automatically by generic_ptr::shared; do not call
    template<class X, class Y> void _internal_accept_owner( shared<X> const * ppx, Y py ) const
    {
        if( weak_this_.expired() )
        {
            weak_this_ = shared<T>( *ppx, py );
        }
    }

private:

    mutable weak<T> weak_this_;
};

} // namespace generic_ptr
} // namespace boost

#endif  // #ifndef BOOST_GENERIC_PTR_ENABLE_SHARED_FROM_THIS_HPP_INCLUDED
