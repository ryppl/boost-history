//-----------------------------------------------------------------------------
// boost extract_fwd.hpp header file
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

#ifndef BOOST_EXTRACT_FWD_HPP
#define BOOST_EXTRACT_FWD_HPP

namespace boost {

// class template extract_traits
//
// Class template meant to be specialized for types wishing to offer
//   extract<T>(...) functionality.
// NOTE: T* extract_traits::execute( [const ]Holder& ) must be defined
//   in any specialization of extract_traits.
template <typename Holder> struct extract_traits;

} // namespace boost

#endif // BOOST_EXTRACT_FWD_HPP
