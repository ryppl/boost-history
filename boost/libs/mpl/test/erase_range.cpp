//-----------------------------------------------------------------------------
// boost mpl/test/pop_front.cpp source file
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

#include "boost/mpl/erase_range.hpp"
#include "boost/mpl/type_list.hpp"
#include "boost/mpl/value_list.hpp"
#include "boost/mpl/find.hpp"
#include "boost/mpl/size.hpp"
#include "boost/mpl/int_t.hpp"
#include "boost/mpl/begin_end.hpp"
#include "boost/mpl/assert_is_same.hpp"
#include "boost/static_assert.hpp"
#include "boost/config.hpp"

namespace mpl = boost::mpl;

int main()
{
    typedef mpl::type_list<int,char,long,short,unsigned,long,double,long> types;
    typedef mpl::value_list<1,0,5,2,7,5,0,5> values;

    typedef mpl::find<types,short>::iterator types_itor1;
    typedef mpl::find<types,double>::iterator types_itor2;

    typedef mpl::find<values, mpl::int_t<0> >::iterator values_itor1;
    typedef mpl::find<values, mpl::int_t<7> >::iterator values_itor2;

    typedef mpl::erase_range<types,types_itor1,types_itor2>::sequence result_types;
    typedef mpl::erase_range<values,values_itor1,values_itor2>::sequence result_values;
    
    BOOST_STATIC_ASSERT(mpl::size<result_types>::value == 5);
    BOOST_STATIC_ASSERT(mpl::size<result_values>::value == 5);

    typedef mpl::find<result_types,unsigned> result_types_find;
    typedef mpl::find<result_values,mpl::int_t<2> > result_values_find;

    BOOST_MPL_ASSERT_IS_SAME(result_types_find::iterator, mpl::end<result_types>::iterator);
    BOOST_MPL_ASSERT_IS_SAME(result_values_find::iterator, mpl::end<result_values>::iterator);

    return 0;
}
