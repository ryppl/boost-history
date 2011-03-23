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
#include <boost/assign/v2/detail/config/check.hpp>

#include <boost/assign/v2/put.hpp>
// Options come next
#include <boost/assign/v2/option/iterate.hpp> 
#include <boost/assign/v2/deque.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/construct.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/typeof/typeof.hpp>
#include <libs/assign/v2/test/option/iterate.h>

#include <iostream>

namespace test_assign_v2{
namespace xxx_option{
namespace xxx_iterate{

    void test()
    {
        using namespace boost;
        namespace as2 = assign::v2;
        {
            //[test_option_iterate_meta
            typedef as2::aux::keyword_iterate keyword_;
            typedef as2::modifier_tag::iterate_arg arg_;
            typedef as2::aux::option_modifier<keyword_, arg_> modulo_;
            typedef array<int, 4> cont_;
            typedef as2::result_of::put<cont_>::type put_;
            typedef as2::result_of::option_modifier<put_> meta1_;
            typedef ::boost::mpl::apply2<meta1_, keyword_, arg_>::type result1_;
            typedef as2::modifier_tag::iterate<arg_> tag1_;
            typedef as2::aux::replace_modifier_tag<put_> meta2_;
            typedef ::boost::mpl::apply1<meta2_, tag1_>::type result2_;
            BOOST_MPL_ASSERT(( is_same<result1_, result2_> ));
            //]
        }
        {
            //[test_option_iterate_shifted
            using namespace lambda;
            typedef int T; array<T, 4> powers; powers[0] = 1; powers[1] = 10;
            int index = 2; ( as2::put( powers ) % ( as2::_iterate = var( index )++ ) )( 100 )( 1000 );

            BOOST_ASSIGN_V2_CHECK( range::equal( powers, as2::csv_deque( 1, 10, 100, 1000 ) ) );
            //]
        }
        {
            //[test_option_iterate_meta_deque
            typedef as2::aux::keyword_iterate keyword_;
            typedef as2::modifier_tag::iterate_arg arg_;
            typedef as2::aux::option_modifier<keyword_, arg_> modulo_;
            typedef as2::result_of::deque<int>::type put_;
            typedef as2::result_of::option_modifier<put_> meta1_;
            typedef ::boost::mpl::apply2<meta1_, keyword_, arg_>::type result1_;
            typedef as2::modifier_tag::iterate<arg_> tag1_;
            typedef as2::aux::replace_modifier_tag<put_> meta2_;
            typedef ::boost::mpl::apply1<meta2_, tag1_>::type result2_;
            BOOST_MPL_ASSERT(( is_same<result1_, result2_> ));
            //]
        }
        {
            //[test_option_iterate_shifted_deque
            using namespace lambda;
            as2::result_of::deque<int>::type missing_tail = as2::deque<int>( 1 )( 10 )( -1 )( -1 );
            int index = 2; 
            
            BOOST_AUTO(
                powers,
                ( missing_tail % ( as2::_iterate = var( index )++ ) )( 100 )( 1000 )
            );

            BOOST_ASSIGN_V2_CHECK( range::equal( powers, as2::csv_deque( 1, 10, 100, 1000 ) ) );
            //]
        }
    }

}// xxx_iterate
}// xxx_option
}// test_assign_v2
