//-----------------------------------------------------------------------------
// boost mpl/test/copy_backward.cpp source file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-02
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#include "boost/mpl/copy_backward.hpp"
#include "boost/mpl/list.hpp"
#include "boost/mpl/list_c.hpp"
#include "boost/mpl/range_c.hpp"
#include "boost/mpl/size.hpp"
#include "boost/mpl/equal.hpp"
#include "boost/static_assert.hpp"

namespace mpl = boost::mpl;

int main()
{
    using namespace mpl::placeholder;

    typedef mpl::list_c<int,5,6,7,8,9>::type numbers;
    typedef mpl::copy_backward<
          mpl::range_c<int,0,5>
        , mpl::push_front<_,_>
        , numbers
        >::type result;

    BOOST_STATIC_ASSERT(mpl::size<result>::value == 10);
    BOOST_STATIC_ASSERT((mpl::equal< result,mpl::range_c<int,0,10> >::value));

    return 0;
}
