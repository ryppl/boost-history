//-----------------------------------------------------------------------------
// boost mpl/test/replace.cpp source file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-01
// Aleksey Gurtovoy and John R. Bandela
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#include "boost/mpl/replace.hpp"
#include "boost/mpl/type_list.hpp"
#include "boost/mpl/value_list.hpp"
#include "boost/mpl/equal.hpp"
#include "boost/static_assert.hpp"

namespace mpl = boost::mpl;

int main()
{
    typedef mpl::type_list<int,char,long,short,char,long,double,long> types;
    typedef mpl::value_list<1,0,5,1,7,5,0,5> values;

    typedef mpl::replace<types,char,short>::sequence types_replace;
    typedef mpl::replace<values,mpl::int_t<0>,mpl::int_t<9> >::sequence values_replace;
	
    typedef mpl::type_list<int,short,long,short,short,long,double,long> types_answer;
    typedef mpl::value_list<1,9,5,1,7,5,9,5> values_answer;	
    
    BOOST_STATIC_ASSERT((mpl::equal<types_replace,types_answer>::value));
    BOOST_STATIC_ASSERT((mpl::equal<values_replace,values_answer>::value));
    
    return 0;
}
