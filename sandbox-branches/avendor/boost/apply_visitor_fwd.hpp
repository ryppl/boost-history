//-----------------------------------------------------------------------------
// boost apply_visitor_fwd.hpp header file
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

#ifndef BOOST_APPLY_VISITOR_FWD_HPP
#define BOOST_APPLY_VISITOR_FWD_HPP

namespace boost {

// class template apply_visitor_traits
//
// Class template meant to be specialized for types wishing to offer
//   apply_visitor(visitor, visitable) functionality.
// NOTE: apply_visitor_traits::execute(visitor, visitable) must be defined
//   for any specialization of apply_visitor_traits.
//
template <typename Visitable> struct apply_visitor_traits;

} // namespace boost

#endif // BOOST_APPLY_VISITOR_FWD_HPP
