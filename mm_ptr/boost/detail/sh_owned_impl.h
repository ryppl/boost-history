/**
	@file
	Boost sh_owned_impl.h header file.

	@note
	Copyright (c) 2008 Phil Bouchard <phil@fornux.com>.

	Distributed under the Boost Software License, Version 1.0.

	See accompanying file LICENSE_1_0.txt or copy at
	http://www.boost.org/LICENSE_1_0.txt

	See http://www.boost.org/libs/smart_ptr/doc/index.html for documentation.
*/


#ifndef BOOST_DETAIL_SH_OWNED_IMPL_HPP_INCLUDED
#define BOOST_DETAIL_SH_OWNED_IMPL_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  detail/sh_owned_impl.hpp
//
//  Copyright (c) 2001, 2002, 2003 Peter Dimov and Multi Media Ltd.
//  Copyright 2008 Philippe Bouchard
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/config.hpp>

#if defined(BOOST_SP_USE_STD_ALLOCATOR) && defined(BOOST_SP_USE_QUICK_ALLOCATOR)
# error BOOST_SP_USE_STD_ALLOCATOR and BOOST_SP_USE_QUICK_ALLOCATOR are incompatible.
#endif

#include <boost/checked_delete.hpp>
#include <boost/detail/sh_owned_base.hpp>

#if defined(BOOST_SP_USE_QUICK_ALLOCATOR)
#include <boost/detail/quick_allocator.hpp>
#endif

#if defined(BOOST_SP_USE_STD_ALLOCATOR)
#include <memory>           // std::allocator
#endif

#include <typeinfo>         // std::type_info in get_deleter
#include <cstddef>          // std::size_t

namespace boost
{

#if defined(BOOST_SP_ENABLE_DEBUG_HOOKS)

void sh_scalar_constructor_hook( void * px, std::size_t size, void * pn );
void sh_scalar_destructor_hook( void * px, std::size_t size, void * pn );

#endif

namespace detail
{

namespace sh
{

template<class X> class owned_impl_p: public owned<X>
{
private:

    owned_impl_p( owned_impl_p const & );
    owned_impl_p & operator= ( owned_impl_p const & );

    typedef owned_impl_p<X> this_type;

public:

    virtual void dispose() // nothrow
    {
#if defined(BOOST_SP_ENABLE_DEBUG_HOOKS)
        boost::scalar_destructor_hook( p_, sizeof(X), this );
#endif
    }

    virtual void * get_deleter( std::type_info const & )
    {
        return 0;
    }

#if defined(BOOST_SP_USE_STD_ALLOCATOR)

    void * operator new( std::size_t )
    {
        return std::allocator<this_type>().allocate( 1, static_cast<this_type *>(0) );
    }

    void operator delete( void * p )
    {
        std::allocator<this_type>().deallocate( static_cast<this_type *>(p), 1 );
    }

#endif

#if defined(BOOST_SP_USE_QUICK_ALLOCATOR)

    void * operator new( std::size_t )
    {
        return quick_allocator<this_type>::alloc();
    }

    void operator delete( void * p )
    {
        quick_allocator<this_type>::dealloc( p );
    }

#endif
};

//
// Borland's Codeguard trips up over the -Vx- option here:
//
#ifdef __CODEGUARD__
# pragma option push -Vx-
#endif

template<class P, class D> class owned_impl_pd: public owned<X>
{
private:

    D d_; // copy constructor must not throw

    owned_impl_pd( owned_impl_pd const & );
    owned_impl_pd & operator= ( owned_impl_pd const & );

    typedef owned_impl_pd<P, D> this_type;

public:

    // pre: d(p) must not throw

    owned_impl_pd( D d ): d_(d)
    {
    }

    virtual void dispose() // nothrow
    {
        d_( p_ );
    }

    virtual void * get_deleter( std::type_info const & ti )
    {
        return ti == typeid(D)? &d_: 0;
    }

#if defined(BOOST_SP_USE_STD_ALLOCATOR)

    void * operator new( std::size_t )
    {
        return std::allocator<this_type>().allocate( 1, static_cast<this_type *>(0) );
    }

    void operator delete( void * p )
    {
        std::allocator<this_type>().deallocate( static_cast<this_type *>(p), 1 );
    }

#endif

#if defined(BOOST_SP_USE_QUICK_ALLOCATOR)

    void * operator new( std::size_t )
    {
        return quick_allocator<this_type>::alloc();
    }

    void operator delete( void * p )
    {
        quick_allocator<this_type>::dealloc( p );
    }

#endif
};

template<class P, class D, class A> class owned_impl_pda: public owned<X>
{
private:

    D d_; // copy constructor must not throw
    A a_; // copy constructor must not throw

    owned_impl_pda( owned_impl_pda const & );
    owned_impl_pda & operator= ( owned_impl_pda const & );

    typedef owned_impl_pda<P, D, A> this_type;

public:

    // pre: d( p ) must not throw

    owned_impl_pda( D d, A a ): d_( d ), a_( a )
    {
    }

    virtual void dispose() // nothrow
    {
        d_( p_ );
    }

    virtual void * get_deleter( std::type_info const & ti )
    {
        return ti == typeid( D )? &d_: 0;
    }
};

#ifdef __CODEGUARD__
# pragma option pop
#endif

} // namespace sh

} // namespace detail

} // namespace boost

#endif  // #ifndef BOOST_DETAIL_SP_OWNED_IMPL_HPP_INCLUDED
