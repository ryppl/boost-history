//-----------------------------------------------------------------------------
// boost move/algorithm.hpp header file
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

#ifndef BOOST_MOVE_ALGORITHM_HPP
#define BOOST_MOVE_ALGORITHM_HPP

#include "boost/move_fwd.hpp"
#include <iterator> // for iterator_traits

namespace boost {

//////////////////////////////////////////////////////////////////////////
// function template move_swap
//
// Swaps using Koenig lookup but falls back to move-swap for primitive
// types and on non-conforming compilers.
//

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

#endif // BOOST_MOVE_ALGORITHM_HPP
