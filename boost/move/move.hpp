//-----------------------------------------------------------------------------
// boost move/move.hpp header file
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

#ifndef BOOST_MOVE_MOVE_HPP
#define BOOST_MOVE_MOVE_HPP

#include <algorithm> // for std::swap
#include <iterator> // for iterator_traits

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

//////////////////////////////////////////////////////////////////////////
// function template move_swap
//
// Swaps using Koenig lookup but falls back to move-swap for primitive
// types and on non-conforming compilers.
//

#if   defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)   \
 || ( defined(__GNUC__) && __GNUC__ < 3 )

// [Indicate that move_swap by overload is disabled...]
#define BOOST_NO_MOVE_SWAP_BY_OVERLOAD

// [...and provide straight swap-by-move implementation:]
template <typename T>
inline void move_swap(T& lhs, T& rhs)
{
    T tmp( boost::move(lhs) );
    lhs = boost::move(rhs);
    rhs = boost::move(tmp);
}

#else// !defined(__GNUC__) || __GNUC__ >= 3

namespace detail { namespace move_swap {

template <typename T>
inline void swap(T& lhs, T& rhs)
{
    T tmp( boost::move(lhs) );
    lhs = boost::move(rhs);
    rhs = boost::move(tmp);
}

}} // namespace detail::move_swap

template <typename T>
inline void move_swap(T& lhs, T& rhs)
{
    using detail::move_swap::swap;

    swap(lhs, rhs);
}

#endif // __GNUC__ workaround (for 2.x and below)

//////////////////////////////////////////////////////////////////////////
// function template move
//
// Moves-assigns each element in the range [first, last) to the range
// that begins with iterator result.
//
template <typename InputIterator, typename OutputIterator>
    OutputIterator
move(InputIterator first, InputIterator last, OutputIterator result)
{
    for (; first != last; ++first, ++result)
        *result = move(*first);

    return result;
}

//////////////////////////////////////////////////////////////////////////
// function template move_backward
//
// Move-assigns each element in the reverse range (last, first] to the
// range that begins with iterator result.
//
template <typename BidirectionalIterator1, typename BidirectionalIterator2>
    BidirectionalIterator2
move_backward(
      BidirectionalIterator1 first, BidirectionalIterator1 last
    , BidirectionalIterator2 result
    )
{
    while (last != first)
        *--result = move(*--last);

    return result;
}

//////////////////////////////////////////////////////////////////////////
// function template uninitialized_move
//
// Move-constructs each element in the reverse range (last, first] to the
// range that begins with iterator result.
//
template <typename InputIterator, typename OutputIterator>
    OutputIterator
uninitialized_move(
      InputIterator first, InputIterator last
    , OutputIterator result
    )
{
    typedef typename std::iterator_traits<
          OutputIterator
        >::value_type T;

    for (; first != last; ++first, ++result)
        new(*result) T(move(*first));

    return result;
}

} // namespace boost

#endif // BOOST_MOVE_MOVE_HPP
