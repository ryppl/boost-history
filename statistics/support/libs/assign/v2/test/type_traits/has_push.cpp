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
#include <boost/static_assert.hpp>
#include <boost/assign/v2/detail/type_traits/container/has_push.hpp>
#include <libs/assign/v2/test/type_traits/has_push.h>

namespace test_assign_v2{
namespace xxx_type_traits{
namespace xxx_has_push{

    void test()
    {
         namespace ns = boost::assign::v2::container_type_traits;
         {
             typedef std::queue<int> v_;
             typedef ns::has_push<v_> has_push_;
             
             //typedef void (v_::*fp_)(int const&) ;
             //fp_ fp = &v_::push; v_ v; ((v).*fp)( 1 );
             //BOOST_ASSERT(v.front() == 1);
             BOOST_STATIC_ASSERT( has_push_::value );
         }
         {
             typedef std::stack<int> v_;
             typedef ns::has_push<v_> has_push_;
             BOOST_STATIC_ASSERT( has_push_::value );
         }
         {
             typedef std::vector<int> v_;
             typedef ns::has_push<v_> has_push_;
             BOOST_STATIC_ASSERT( !has_push_::value );
         }
    }
    
}// xxx_has_push
}// xxx_type_traits
}// test_assign_v2

