//-----------------------------------------------------------------------------
// boost mpl/test/type_list.cpp source file
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

#include "boost/mpl/type_list.hpp"

namespace mpl = boost::mpl;

int main()
{
    typedef mpl::type_list<> list0;
    typedef mpl::type_list<int> list1;
    typedef mpl::type_list<int,long> list2;
    typedef mpl::type_list<
          void
        , char
        , signed char
        , unsigned char
        , signed short
        , unsigned short
        , signed int
        , unsigned int
        , signed long
        , unsigned long
        , float
        , double
        , long double
        , void
        > list3;

    return 0;
}
