//-----------------------------------------------------------------------------
// boost generic_visitor.hpp header file
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

#ifndef BOOST_GENERIC_VISITOR_HPP
#define BOOST_GENERIC_VISITOR_HPP

namespace boost {

//////////////////////////////////////////////////////////////////////////
// class template generic_visitor
//
// An empty base class that typedefs the return type of a deriving generic
// visitor. The class is analogous to std::unary_function in this role.
//

template <typename R = void>
struct generic_visitor
{
    typedef R result_type;

protected:
    ~generic_visitor() { }
};

} // namespace boost

#endif // BOOST_GENERIC_VISITOR_HPP
