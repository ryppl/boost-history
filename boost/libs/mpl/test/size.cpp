//-----------------------------------------------------------------------------
// boost mpl/test/size.cpp source file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-01
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#include "boost/mpl/size.hpp"
#include "boost/mpl/type_list.hpp"
#include "boost/static_assert.hpp"

namespace mpl = boost::mpl;

int main()
{    
    typedef mpl::type_list<> empty_list;
    typedef mpl::type_list<char,short,int,long> list;
    
    BOOST_STATIC_ASSERT((mpl::size<empty_list>::value == 0));
    BOOST_STATIC_ASSERT((mpl::size<list>::value == 4));

    return 0;
}
