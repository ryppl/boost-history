//-----------------------------------------------------------------------------
// boost move_fwd.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MOVE_FWD_HPP
#define BOOST_MOVE_FWD_HPP

#include "boost/mpl/if.hpp"
#include "boost/type_traits/is_base_and_derived.hpp"

namespace boost {

//////////////////////////////////////////////////////////////////////////
// forward declares
//
template <typename Deriving> struct moveable;
template <typename T>        struct move_source;

//////////////////////////////////////////////////////////////////////////
// function template move
//
// Takes a T& and returns, if T derives moveable<T>, a move_source<T> for
// the object; else, returns the T&.
//

namespace detail {

template <typename T>
struct move_type
{
    typedef typename mpl::if_<
          is_base_and_derived<moveable<T>, T>
        , move_source<T>
        , T&
        >::type type;
};

} // namespace detail

template <typename T>
inline
    typename detail::move_type<T>::type
move(T& source)
{
    typedef typename detail::move_type<T>::type
        move_t;

    return move_t(source);
}

} // namespace boost

#endif // BOOST_MOVE_FWD_HPP
