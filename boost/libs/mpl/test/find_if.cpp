//-----------------------------------------------------------------------------
// boost mpl/test/find_if.cpp source file
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

#include "boost/mpl/find_if.hpp"
#include "boost/mpl/type_list.hpp"
#include "boost/mpl/value_list.hpp"
#include "boost/mpl/make_fun.hpp"
#include "boost/mpl/comparison.hpp"
#include "boost/mpl/int_t.hpp"
#include "boost/mpl/same_as.hpp"
#include "boost/mpl/distance.hpp"
#include "boost/mpl/assert_is_same.hpp"
#include "boost/mpl/mpl_config.hpp"
#include "boost/type_traits/arithmetic_traits.hpp"
#include "boost/static_assert.hpp"

namespace mpl = boost::mpl;

#if !defined(BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS)
    typedef mpl::make_f_x<boost::is_float> is_float_type;
#else
    BOOST_MPL_MAKE_F_X(is_float_type, boost::is_float);
#endif

int main()
{
    typedef mpl::type_list<int,char,long,short,char,long,double,long> types;
    typedef mpl::value_list<1,0,5,1,7,5,0,5> values;

    typedef mpl::find_if< types, is_float_type > types_find1;
    typedef mpl::find_if< types, mpl::same_as<long> > types_find2;
    typedef mpl::find_if< types, mpl::same_as<void> > types_find3;

    typedef mpl::find_if< values, mpl::gt<1> > values_find1;
    typedef mpl::find_if< values, mpl::eq<7> > values_find2;
    typedef mpl::find_if< values, mpl::eq<-1> > values_find3;

    BOOST_MPL_ASSERT_IS_SAME(types_find1::iterator::type, double);
    BOOST_MPL_ASSERT_IS_SAME(types_find2::iterator::type, long);
    BOOST_MPL_ASSERT_IS_SAME(types_find3::iterator, mpl::end<types>::iterator);

    BOOST_STATIC_ASSERT(values_find1::iterator::value == 5);
    BOOST_STATIC_ASSERT(values_find2::iterator::value == 7);
    BOOST_MPL_ASSERT_IS_SAME(values_find3::iterator, mpl::end<values>::iterator);

    typedef mpl::begin<types>::iterator types_first;
    BOOST_STATIC_ASSERT((mpl::distance<types_first, types_find1::iterator>::value == 6));
    BOOST_STATIC_ASSERT((mpl::distance<types_first, types_find2::iterator>::value == 2));

    typedef mpl::begin<values>::iterator values_first;
    BOOST_STATIC_ASSERT((mpl::distance<values_first, values_find1::iterator>::value == 2));
    BOOST_STATIC_ASSERT((mpl::distance<values_first, values_find2::iterator>::value == 4));

    return 0;
}
