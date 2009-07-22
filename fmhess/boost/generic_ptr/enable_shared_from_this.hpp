#ifndef BOOST_SMART_PTR_ENABLE_GENERIC_SHARED_FROM_THIS_HPP_INCLUDED
#define BOOST_SMART_PTR_ENABLE_GENERIC_SHARED_FROM_THIS_HPP_INCLUDED

//
//  enable_generic_shared_from_this.hpp
//
//  Copyright 2002, 2009 Peter Dimov
//  Copyright 2009 Frank Mori Hess
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//  http://www.boost.org/libs/smart_ptr/enable_shared_from_this.html
//

#include <boost/smart_ptr/generic_weak.hpp>
#include <boost/smart_ptr/generic_shared.hpp>
#include <boost/assert.hpp>
#include <boost/config.hpp>

namespace boost
{

template<class T> class enable_generic_shared_from_this
{
protected:

    enable_generic_shared_from_this()
    {
    }

    enable_generic_shared_from_this(enable_generic_shared_from_this const &)
    {
    }

    enable_generic_shared_from_this & operator=(enable_generic_shared_from_this const &)
    {
        return *this;
    }

    ~enable_generic_shared_from_this()
    {
    }

public:

    generic_shared<T> shared_from_this()
    {
        generic_shared<T> p( weak_this_ );
        BOOST_ASSERT( get_plain_old_pointer(p) == this );
        return p;
    }
//FIXME: yikes! we need to make it easier to add/remove const from a generic pointer's value_type
    typename generic_shared<T>::template rebind<typename generic_shared<T>::value_type const>::other shared_from_this() const
    {
        typename generic_shared<T>::template rebind<typename generic_shared<T>::value_type const>::other p( weak_this_ );
        BOOST_ASSERT( get_plain_old_pointer(p) == this );
        return p;
    }

public: // actually private, but avoids compiler template friendship issues

    // Note: invoked automatically by generic_shared; do not call
    template<class X, class Y> void _internal_accept_owner( generic_shared<X> const * ppx, Y py ) const
    {
        if( weak_this_.expired() )
        {
            weak_this_ = generic_shared<T>( *ppx, py );
        }
    }

private:

    mutable generic_weak<T> weak_this_;
};

} // namespace boost

#endif  // #ifndef BOOST_SMART_PTR_ENABLE_GENERIC_SHARED_FROM_THIS_HPP_INCLUDED
