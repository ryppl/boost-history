//-----------------------------------------------------------------------------
// boost mpl/test/merge.cpp source file
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

#include "boost/mpl/merge.hpp"
#include "boost/mpl/type_list.hpp"
#include "boost/mpl/equal.hpp"
#include "boost/static_assert.hpp"

namespace mpl = boost::mpl;

int main()
{
    typedef mpl::type_list<char,signed char,unsigned char,signed short,unsigned short> list1;
    typedef mpl::type_list<signed int,unsigned int,signed long,unsigned long> list2;
    typedef mpl::type_list<
          char
        , signed   char
        , unsigned char
        , signed   short
        , unsigned short 
        , signed   int
        , unsigned int
        , signed   long
        , unsigned long
        > list3;

    
    typedef mpl::merge< list1, list2, mpl::type_list<> >::sequence result;
    BOOST_STATIC_ASSERT((mpl::equal<result,list3>::value));
    
    return 0;
}
