//-----------------------------------------------------------------------------
// boost mpl/test/remove.cpp source file
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

#include "boost/mpl/remove.hpp"
#include "boost/mpl/type_list.hpp"
#include "boost/mpl/size.hpp"
#include "boost/mpl/count.hpp"
#include "boost/static_assert.hpp"

namespace mpl = boost::mpl;

int main()
{    
    typedef mpl::type_list<int,char,long,short,char,long,double,long> types;

    typedef mpl::remove<types, long>::sequence removed_longs;
    typedef mpl::remove<types, char>::sequence removed_chars;
    typedef mpl::remove<types, void>::sequence removed_voids;

    BOOST_STATIC_ASSERT((mpl::size<removed_longs>::value == mpl::size<types>::value - 3));
    BOOST_STATIC_ASSERT((mpl::size<removed_chars>::value == mpl::size<types>::value - 2));
    BOOST_STATIC_ASSERT((mpl::size<removed_voids>::value == mpl::size<types>::value));
    
    BOOST_STATIC_ASSERT((mpl::count<removed_longs, long>::value == 0));
    BOOST_STATIC_ASSERT((mpl::count<removed_chars, char>::value == 0));
    BOOST_STATIC_ASSERT((mpl::count<removed_voids, void>::value == 0));

    return 0;
}
