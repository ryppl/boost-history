//-----------------------------------------------------------------------------
// boost visitor/detail/dynamic_return_error.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002-2003
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_VISITOR_DETAIL_DYNAMIC_RETURN_ERRROR_HPP
#define BOOST_VISITOR_DETAIL_DYNAMIC_RETURN_ERRROR_HPP

#include <exception>

namespace boost {

namespace detail {

//////////////////////////////////////////////////////////////////////////
// class dynamic_return_error
//
// Exception thrown when a value-returning dynamic visitor fails to set
// its result_value field (see dynamic_visitor_return_base).
//
struct dynamic_return_error
    : std::exception
{
public:
    virtual const char * what() const throw()
    {
        return "logic error: "
               "dynamic_visitor_return_base<...> did not set its result value";
    }
};

} // namespace detail

} // namespace boost

#endif // BOOST_VISITOR_DETAIL_DYNAMIC_RETURN_ERRROR_HPP
