//-----------------------------------------------------------------------------
// boost mpl/test/insert.cpp source file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2001-02
// Aleksey Gurtovoy
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "boost/mpl/insert.hpp"
#include "boost/mpl/find.hpp"
#include "boost/mpl/list_c.hpp"
#include "boost/mpl/size.hpp"
#include "boost/mpl/range_c.hpp"
#include "boost/mpl/equal.hpp"
#include "boost/static_assert.hpp"

namespace mpl = boost::mpl;

int main()
{
    typedef mpl::list_c<int,0,1,3,4,5,6,7,8,9> numbers;
    typedef mpl::find< numbers,mpl::integral_c<int,3> >::type pos;
    typedef mpl::insert< numbers,pos,mpl::integral_c<int,2> >::type range;
    
    BOOST_STATIC_ASSERT(mpl::size<range>::type::value == 10);
    BOOST_STATIC_ASSERT((mpl::equal< range,mpl::range_c<int,0,10> >::type::value));
    return 0;
}
