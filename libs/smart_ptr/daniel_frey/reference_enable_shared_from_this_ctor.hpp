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
template< class T, class Y > void sp_accept_owner( shared_ptr<Y> * ptr, enable_shared_from_this<T> const * pe );
template< class T, class Y > void sp_accept_owner( shared_ptr<Y> * ptr, enable_shared_from_this<T> const * pe, void * /*pd*/ );

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
        BOOST_ASSERT( _shared_ptr.use_count() <= 1 ); // make sure no dangling shared_ptr objects exist
    }

public:

    shared_ptr<T> shared_from_this()
    {
        init_weak_once();
        shared_ptr<T> p( _weak_ptr );
        BOOST_ASSERT( p.get() == this );
        return p;
    }

    shared_ptr<T const> shared_from_this() const
    {
        init_weak_once();
        shared_ptr<T const> p( _weak_ptr );
        BOOST_ASSERT( p.get() == this );
        return p;
    }

private:

    mutable weak_ptr<T> _weak_ptr;
    mutable shared_ptr<T> _shared_ptr;

    void init_weak_once() const
    {
        weak_ptr<T> e;
        if( !( _weak_ptr < e ) && !( e < _weak_ptr ) )
        {
            T * p = dynamic_cast<T *>( const_cast<enable_shared_from_this*>( this ) );
            _shared_ptr.reset( p, detail::sp_deleter_wrapper() );
            BOOST_ASSERT( _shared_ptr.get() == this );
            _weak_ptr = _shared_ptr;
        }
    }

    template<typename U>
    void sp_accept_owner( shared_ptr<U> & owner ) const
    {
        if( _weak_ptr.use_count() == 0 )
        {
            T * p = dynamic_cast<T *>( const_cast<enable_shared_from_this*>( this ) );
            _weak_ptr = shared_ptr<T>( owner, p );
        }else if( _shared_ptr )
        {
            BOOST_ASSERT( owner.unique() ); // no weak_ptrs to owner should exist either, but there's no way to check that
            detail::sp_deleter_wrapper * pd = get_deleter<detail::sp_deleter_wrapper>( _shared_ptr );
            BOOST_ASSERT( pd != 0 );
            pd->set_deleter( owner );

            owner.reset( _shared_ptr, owner.get() );
            _shared_ptr.reset();
        }
    }

    template< class U, class Y > friend void sp_accept_owner( shared_ptr<Y> * ptr, enable_shared_from_this<U> const * pe );
    template< class U, class Y > friend void sp_accept_owner( shared_ptr<Y> * ptr, enable_shared_from_this<U> const * pe, void * /*pd*/ );
};

template< class T, class Y > inline void sp_accept_owner( shared_ptr<Y> * ptr, enable_shared_from_this<T> const * pe )
{
    if( pe != 0 )
    {
        pe->sp_accept_owner( *ptr );
    }
}

template< class T, class Y > inline void sp_accept_owner( shared_ptr<Y> * ptr, enable_shared_from_this<T> const * pe, void * /*pd*/ )
{
    if( pe != 0 )
    {
        pe->sp_accept_owner( *ptr );
    }
}

} // namespace boost

#endif  // #ifndef BOOST_ENABLE_SHARED_FROM_THIS_HPP_INCLUDED
