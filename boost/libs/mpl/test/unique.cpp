//-----------------------------------------------------------------------------
// boost mpl/test/unique.cpp source file
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

#include "boost/mpl/unique.hpp"
#include "boost/mpl/type_list.hpp"
#include "boost/mpl/size.hpp"
#include "boost/mpl/count.hpp"
#include "boost/static_assert.hpp"

namespace mpl = boost::mpl;

int main()
{    
    typedef mpl::type_list<int,char,long,short,char,long,double,long> types;
    typedef mpl::type_list<int,char,long,short,double> result;

    typedef mpl::unique<types>::sequence unique_types;

    BOOST_STATIC_ASSERT((mpl::size<unique_types>::value == 5));    
    BOOST_STATIC_ASSERT((mpl::count<unique_types,int>::value == 1));
    BOOST_STATIC_ASSERT((mpl::count<unique_types,char>::value == 1));
    BOOST_STATIC_ASSERT((mpl::count<unique_types,long>::value == 1));
    BOOST_STATIC_ASSERT((mpl::count<unique_types,short>::value == 1));
    BOOST_STATIC_ASSERT((mpl::count<unique_types,double>::value == 1));

    return 0;
}
