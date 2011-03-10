//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/array.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/construct.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/typeof/typeof.hpp>

#include <boost/assign/v2/detail/functor/post_increment.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/modifier/iterate.hpp>
#include <boost/assign/v2/put/container/put.hpp>
#include <boost/assign/v2/put/deque.hpp>
#include <libs/assign/v2/test/put/modifier/iterate.h>

#include <iostream>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_modifier{
namespace xxx_iterate{

    void test()
    {
        namespace as2 = boost::assign::v2;
        namespace lambda = boost::lambda;
        {
        	//[iterate_meta
            typedef as2::put_aux::keyword_iterate keyword_;
			typedef as2::modifier_tag::iterate_arg arg_;
            typedef as2::put_aux::modulo_modifier<keyword_, arg_> modulo_;
            typedef boost::array<int, 4> cont_;
            typedef as2::result_of::put<cont_>::type put_;
            typedef as2::result_of::modulo_modifier<put_> meta1_;
            typedef ::boost::mpl::apply2<meta1_, keyword_, arg_>::type result1_;
            typedef as2::modifier_tag::iterate<arg_> tag1_;
            typedef as2::put_aux::replace_modifier_tag<put_> meta2_;
            typedef ::boost::mpl::apply1<meta2_, tag1_>::type result2_;
            BOOST_MPL_ASSERT((boost::is_same<result1_, result2_>));
            //]
        }
        {
        
            //[iterate
            typedef int T; boost::array<T, 4> consecutive_powers; consecutive_powers[0] = 1; consecutive_powers[1] = 10;
            int i = 2; ( as2::put( consecutive_powers ) % ( as2::_iterate = lambda::var( i )++ ) )( 100 )( 1000 );

            BOOST_ASSIGN_V2_CHECK( consecutive_powers[0] == 1 );
            BOOST_ASSIGN_V2_CHECK( consecutive_powers[1] == 10 );
            BOOST_ASSIGN_V2_CHECK( consecutive_powers[2] == 100 );
            BOOST_ASSIGN_V2_CHECK( consecutive_powers[3] == 1000 );
            //]
        }
        {
        	//[iterate_meta_deque
            typedef as2::put_aux::keyword_iterate keyword_;
			typedef as2::modifier_tag::iterate_arg arg_;
            typedef as2::put_aux::modulo_modifier<keyword_, arg_> modulo_;
            typedef as2::result_of::deque<int>::type put_;
            typedef as2::result_of::modulo_modifier<put_> meta1_;
            typedef ::boost::mpl::apply2<meta1_, keyword_, arg_>::type result1_;
            typedef as2::modifier_tag::iterate<arg_> tag1_;
            typedef as2::put_aux::replace_modifier_tag<put_> meta2_;
            typedef ::boost::mpl::apply1<meta2_, tag1_>::type result2_;
            BOOST_MPL_ASSERT((boost::is_same<result1_, result2_>));
            //]
        }
        {
            //[iterate_deque
			as2::result_of::deque<int>::type missing_tail = as2::deque<int>( 1 )( 10 )( -1 )( -1 );
            int i = 2; 
            
            BOOST_AUTO(
            	consecutive_powers,
            	( missing_tail % ( as2::_iterate = lambda::var( i )++ ) )( 100 )( 1000 )
            );

            BOOST_ASSIGN_V2_CHECK( consecutive_powers[0] == 1 );
            BOOST_ASSIGN_V2_CHECK( consecutive_powers[1] == 10 );
            BOOST_ASSIGN_V2_CHECK( consecutive_powers[2] == 100 );
            BOOST_ASSIGN_V2_CHECK( consecutive_powers[3] == 1000 );
            //]
        }
    }

}// xxx_iterate
}// xxx_modifier
}// xxx_put
}// test_assign_v2
