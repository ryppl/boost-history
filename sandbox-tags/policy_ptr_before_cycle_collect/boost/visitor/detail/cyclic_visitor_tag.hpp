//-----------------------------------------------------------------------------
// boost visitor/detail/cyclic_visitor_tag.hpp header file
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

#ifndef BOOST_VISITOR_DETAIL_CYCLIC_VISITOR_TAG_HPP
#define BOOST_VISITOR_DETAIL_CYCLIC_VISITOR_TAG_HPP

namespace boost {

namespace detail { namespace visitor {

//////////////////////////////////////////////////////////////////////////
// class template cyclic_visitor_tag
//

struct cyclic_visitor_tag
{
protected:
    ~cyclic_visitor_tag() { }
};

}} // namespace detail::visitor

} // namespace boost

#endif // BOOST_VISITOR_DETAIL_CYCLIC_VISITOR_TAG_HPP
