//-----------------------------------------------------------------------------
// libs mpl/test/max_element.cpp source file
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

#include "boost/mpl/max_element.hpp"

#include "boost/static_assert.hpp"
#include "boost/mpl/list_c.hpp"

namespace mpl = boost::mpl;

int main()
{
    typedef mpl::list_c<int, 3, 4, 2, 0, -5, 8, -1, 7>::type numbers;

    typedef mpl::max_element< numbers >::type max_it;
    typedef max_it::type max_value;
    BOOST_STATIC_ASSERT((max_value::value == 8));
}
