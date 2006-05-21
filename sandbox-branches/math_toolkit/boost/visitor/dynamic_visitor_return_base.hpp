//-----------------------------------------------------------------------------
// boost visitor/dynamic_visitor_return_base.hpp header file
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

#ifndef BOOST_VISITOR_DYNAMIC_VISITOR_RETURN_BASE_HPP
#define BOOST_VISITOR_DYNAMIC_VISITOR_RETURN_BASE_HPP

#include "boost/optional.hpp"
#include "boost/visitor/dynamic_visitor_base.hpp"

namespace boost {

//////////////////////////////////////////////////////////////////////////
// class template dynamic_visitor_return_base<R>
//
// Serves as an abstract base to all dynamic visitors returning objects
// of type R.
//

template <typename R>
struct dynamic_visitor_return_base
    : dynamic_visitor_base
{
public: // typedefs
    typedef R result_type;

public: // fields
    optional<result_type> result_value;

protected:
    ~dynamic_visitor_return_base()
    {
    }
};

} // namespace boost

#endif // BOOST_VISITOR_DYNAMIC_VISITOR_RETURN_BASE_HPP
