//-----------------------------------------------------------------------------
// boost mpl/test/value_list.cpp source file
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

#include "boost/mpl/value_list.hpp"

namespace mpl = boost::mpl;

int main()
{
    typedef mpl::value_list<> list0;
    typedef mpl::value_list<1> list1;
    typedef mpl::value_list<1,2> list2;
    typedef mpl::value_list<1,2,3,4,5,6,7,8,9,10,11,12,13,14,15> list_15;

    return 0;
}
