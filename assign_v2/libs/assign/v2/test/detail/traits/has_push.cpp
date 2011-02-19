//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <stack>
#include <queue>
#include <boost/mpl/assert.hpp>
#include <boost/assign/v2/detail/traits/container/has_push.hpp>
#include <libs/assign/v2/test/detail/traits/has_push.h>

namespace test_assign_v2{
namespace xxx_detail{
namespace xxx_traits{
namespace xxx_has_push{

    void test()
    {
         namespace as2 = boost::assign::v2;
         namespace ns = as2::container_traits;
         {
            typedef std::queue<int> v_;
            BOOST_MPL_ASSERT(( ns::has_push<v_> ));
         }
         {
            typedef std::queue<int> v_;
            BOOST_MPL_ASSERT(( ns::has_push<v_> ));
         }
         {
            typedef std::vector<int> v_;
            BOOST_MPL_ASSERT_NOT(( ns::has_push<v_> ));
         }
    }

}// xxx_has_push
}// xxx_detail
}// xxx_type_traits
}// test_assign_v2

