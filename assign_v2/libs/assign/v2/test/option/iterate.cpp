//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <list>
#include <boost/array.hpp>
#include <boost/assign/v2/support/config/check.hpp>
#include <boost/assign/v2/deque.hpp>
#include <boost/assign/v2/option/modifier/iterate.hpp>
#include <boost/assign/v2/include/csv_put.hpp>
#include <boost/lambda/lambda.hpp>
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
        namespace as2 = boost::assign::v2;
        {
            //[test_option_iterate_meta
            typedef as2::functor_aux::iterate arg_;
            typedef boost::array<int, 4> cont_;
            typedef as2::result_of::put<cont_>::type put_;
            typedef as2::result_of::option_iterate<put_, arg_>::type result1_;
            typedef as2::modifier_tag::iterate<arg_> tag1_;
            typedef as2::interpreter_aux::replace_modifier_tag<put_> meta2_;
            typedef ::boost::mpl::apply1<meta2_, tag1_>::type result2_;
            BOOST_MPL_ASSERT(( boost::is_same<result1_, result2_> ));
            //]
        }
        {
            //[test_option_iterate_shifted
            typedef int T; boost::array<T, 10> seq;
            seq[0] = 1; seq[1] = 2; seq[2] = 3;

            int index = 3;
            as2::csv_put(
                seq
                , as2::_option % ( as2::_iterate = boost::lambda::var( index )++ )
                , 4, 5, 6, 7, 8, 9, 10
            );

            BOOST_ASSIGN_V2_CHECK(
                boost::range::equal(
                    seq,
                    as2::csv_deque( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 )
                )
            );
            //]
        }
        {
            //[test_option_iterate_meta_deque
            typedef as2:: interpreter_aux::keyword_iterate keyword_;
            typedef as2::functor_aux::iterate arg_;
            typedef as2::result_of::deque<int>::type put_;
            typedef as2::result_of::option_iterate<put_, arg_>::type result1_;
            typedef as2::modifier_tag::iterate<arg_> tag1_;
            typedef as2:: interpreter_aux::replace_modifier_tag<put_> meta2_;
            typedef ::boost::mpl::apply1<meta2_, tag1_>::type result2_;
            BOOST_MPL_ASSERT(( boost::is_same<result1_, result2_> ));
            //]
        }
        {
            //[test_option_iterate_shifted_deque
            as2::result_of::csv_deque<int>::type tmp = as2::csv_deque<int>
                ( 1, 2, 3, as2::as_arg_list( std::list<int>( 7 ) ) );

            int index = 3;
            BOOST_AUTO(
                seq,
                (
                    tmp  % ( as2::_iterate = boost::lambda::var( index )++ )
                )( 4 )( 5 )( 6 )( 7 )( 8 )( 9 )( 10 )
            );

            BOOST_ASSIGN_V2_CHECK(
                boost::range::equal(
                    alternating,
                    as2::csv_deque( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 )
                )
            );
            //]
        }
    }

}// xxx_iterate
}// xxx_option
}// test_assign_v2
