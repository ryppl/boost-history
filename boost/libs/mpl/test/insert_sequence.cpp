//-----------------------------------------------------------------------------
// boost mpl/test/insert.cpp source file
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

#include "boost/mpl/insert_sequence.hpp"
#include "boost/mpl/type_list.hpp"
#include "boost/mpl/value_list.hpp"
#include "boost/mpl/find.hpp"
#include "boost/mpl/size.hpp"
#include "boost/mpl/int_t.hpp"
#include "boost/mpl/assert_is_same.hpp"
#include "boost/static_assert.hpp"

namespace mpl = boost::mpl;

int main()
{
    typedef mpl::type_list<int,char,long,char,long,double,long> types;
    typedef mpl::value_list<1,0,5,1,5,0,5> values;

    typedef mpl::find<types, long> types_find;
    typedef mpl::find<values, mpl::int_t<5> > values_find;

    typedef mpl::type_list<short,void> other_types;
    typedef mpl::value_list<7, 9> other_values;
    typedef mpl::insert_sequence<types, types_find::iterator, other_types>::sequence result_types;
    typedef mpl::insert_sequence<values, values_find::iterator, other_values>::sequence result_values;
    
    BOOST_STATIC_ASSERT(mpl::size<result_types>::value  == 9);
    BOOST_STATIC_ASSERT(mpl::size<result_values>::value == 9);

    typedef mpl::find<result_types, void>::iterator types_iterator;
    typedef mpl::find<result_values, mpl::int_t<9> >::iterator values_iterator;

    BOOST_STATIC_ASSERT((mpl::distance< mpl::begin<result_types>::iterator, types_iterator >::value == 3));
    BOOST_STATIC_ASSERT((mpl::distance< mpl::begin<result_values>::iterator, values_iterator >::value == 3));
    
    return 0;
}
