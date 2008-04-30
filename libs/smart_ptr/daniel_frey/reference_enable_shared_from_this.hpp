#ifndef BOOST_ENABLE_SHARED_FROM_THIS_HPP_INCLUDED
#define BOOST_ENABLE_SHARED_FROM_THIS_HPP_INCLUDED

//
//  enable_shared_from_this.hpp
//
//  Copyright (c) 2002 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  http://www.boost.org/libs/smart_ptr/enable_shared_from_this.html
//

#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/assert.hpp>
#include <boost/config.hpp>

namespace boost
{

template< class T > class enable_shared_from_this;
template< class T, class Y > void sp_accept_owner( shared_ptr<Y> const * ptr, enable_shared_from_this<T> const * pe );
template< class T, class Y > void sp_accept_owner( shared_ptr<Y> const * ptr, enable_shared_from_this<T> const * pe, void * /*pd*/ );

template< class T > class enable_shared_from_this
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

// virtual destructor because we need a vtable for dynamic_cast from base to derived to work
    virtual ~enable_shared_from_this()
    {
    }

public:

    shared_ptr<T> shared_from_this()
    {
        shared_ptr<T> p( _weak_ptr );
        BOOST_ASSERT( p.get() == this );
        return p;
    }

    shared_ptr<T const> shared_from_this() const
    {
        shared_ptr<T const> p( _weak_ptr );
        BOOST_ASSERT( p.get() == this );
        return p;
    }

private:

    mutable weak_ptr<T> _weak_ptr;

    template< class U, class Y > friend void sp_accept_owner( shared_ptr<Y> const * ptr, enable_shared_from_this<U> const * pe );
    template< class U, class Y > friend void sp_accept_owner( shared_ptr<Y> const * ptr, enable_shared_from_this<U> const * pe, void * /*pd*/ );
};

template< class T, class Y > inline void sp_accept_owner( shared_ptr<Y> const * ptr, enable_shared_from_this<T> const * pe )
{
    if( pe != 0 && pe->_weak_ptr.use_count() == 0 )
    {
        T * p = dynamic_cast<T *>( const_cast<enable_shared_from_this<T>*>( pe ) );
        pe->_weak_ptr = shared_ptr<T>( *ptr, p );
    }
}

template< class T, class Y > inline void sp_accept_owner( shared_ptr<Y> const * ptr, enable_shared_from_this<T> const * pe, void * /*pd*/ )
{
    if( pe != 0 && pe->_weak_ptr.use_count() == 0 )
    {
        T * p = dynamic_cast<T *>( const_cast<enable_shared_from_this<T>*>( pe ) );
        pe->_weak_ptr = shared_ptr<T>( *ptr, p );
    }
}

} // namespace boost

#endif  // #ifndef BOOST_ENABLE_SHARED_FROM_THIS_HPP_INCLUDED
