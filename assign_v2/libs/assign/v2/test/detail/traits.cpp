//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <cstddef>
#include <queue>
#include <stack>
#include <vector>
#include <boost/assign/v2/detail/traits.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/static_assert.hpp>
#include <libs/assign/v2/test/detail/traits.h>

namespace test_assign_v2{
namespace xxx_detail{
namespace xxx_traits{

    void test()
    {
        namespace as2 = boost::assign::v2;
        namespace ns = as2::container_aux;
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
        {
            typedef std::vector<int> inp_;
            typedef ns::has_value_type<inp_>::type pred_;
            BOOST_STATIC_ASSERT(pred_::value);
        }
        {
            typedef int inp_;
            typedef ns::has_value_type<inp_>::type pred_;
            BOOST_STATIC_ASSERT(!pred_::value);
        }
    }

}// xxx_traits
}// xxx_detail
}// xxx_test_assign

