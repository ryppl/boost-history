// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2007-8 Anthony Williams

#ifndef BOOST_FIBERS_DETAIL_MOVE_HPP
#define BOOST_FIBERS_DETAIL_MOVE_HPP

#include <boost/config.hpp>

#ifndef BOOST_NO_SFINAE
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_convertible.hpp>
#endif

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace detail {

template< typename F >
struct fiber_move_t
{
    F	&	f;

    explicit fiber_move_t( F & f_) :
        f( f_)
    {}

    F & operator*() const
    { return f; }

    F * operator->() const
    { return & f; }

private:
    void operator=( fiber_move_t &);
};

}

#ifndef BOOST_NO_SFINAE
template< typename F >
typename enable_if< is_convertible< F &, detail::fiber_move_t< F > >, F >::type move( F & f)
{ return F( detail::fiber_move_t< F >( f) ); }
#endif

template< typename F >
detail::fiber_move_t< F > move( detail::fiber_move_t< F > f)
{ return f; }

}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_FIBERS_DETAIL_MOVE_H
