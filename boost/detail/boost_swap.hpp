//-----------------------------------------------------------------------------
// boost detail/boost_swap.hpp header file
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

#ifndef BOOST_DETAIL_BOOST_SWAP_HPP
#define BOOST_DETAIL_BOOST_SWAP_HPP

#include <utility> // for std::swap

namespace boost {
namespace detail {

//////////////////////////////////////////////////////////////////////////
// function template boost_swap
//
// Swaps using Koenig lookup but falls back to std::swap for primitive
// types and on non-conforming compilers.
//
template <typename T>
void boost_swap(T& lhs, T& rhs)
{
#	if !defined(__GNUC__)
		using std::swap;
#	else// defined(__GNUC__)
		using namespace std;
#	endif // __GNUC__ workaround

	swap(lhs, rhs);
}

} // namespace detail
} // namespace boost

#endif // BOOST_DETAIL_BOOST_SWAP_HPP
