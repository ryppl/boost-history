//-----------------------------------------------------------------------------
// boost mpl/test/replace_if.cpp source file
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

#include "boost/mpl/replace_if.hpp"
#include "boost/mpl/type_list.hpp"
#include "boost/mpl/value_list.hpp"
#include "boost/mpl/equal.hpp"
#include "boost/mpl/comparison/greater.hpp"
#include "boost/static_assert.hpp"

namespace mpl = boost::mpl;

int main()
{    
    typedef mpl::value_list<1,4,5,2,7,5,3,5> values;    
    typedef mpl::replace_if< 
          values
        , mpl::gt<4>
        , mpl::int_t<777>
        >::sequence result;

    typedef mpl::value_list<1,4,777,2,777,777,3,777> values_answer;    
    BOOST_STATIC_ASSERT((mpl::equal<result, values_answer>::value));
    
    return 0;
}
