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
#include <boost/assign/v2/deque.hpp>
#include <boost/assign/v2/include/csv_basic.hpp>
#include <boost/assign/v2/include/put.hpp>
#include <boost/assign/v2/support/config/check.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/typeof/typeof.hpp>
#include <libs/assign/v2/test/option/iterate.h>

namespace test_assign_v2{
namespace xxx_option{
namespace xxx_iterate{

    void test()
    {
        namespace as2 = boost::assign::v2;
        {
            typedef as2::functor_aux::iterate arg_;
            typedef boost::array<int, 4> cont_;
            typedef as2::result_of::put<cont_>::type put_;
            typedef as2::result_of::option_iterate<put_, arg_>::type result1_;
            typedef as2::modifier_tag::iterate<arg_> tag1_;
            typedef as2::interpreter_aux::replace_modifier_tag<put_> meta2_;
            typedef ::boost::mpl::apply1<meta2_, tag1_>::type result2_;
            BOOST_MPL_ASSERT(( boost::is_same<result1_, result2_> ));
        }
        {
            //[test_iterate1
            typedef int T; boost::array<T, 10> cont;
            cont[0] = 1; cont[1] = 2; cont[2] = 3;

            int index = 3;
            BOOST_AUTO(
                _modifier,
                ( as2::_iterate = boost::lambda::var( index )++ )
            );

            BOOST_ASSIGN_V2_CHECK(
                boost::range::equal(
                    csv(
                        as2::put( cont ) % _modifier
                        , 4, 5, 6, 7, 8, 9, 10
                    ).container(),
                    as2::csv_deque( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 )
                )
            );
            //]
        }
        {
            typedef as2:: interpreter_aux::keyword_iterate keyword_;
            typedef as2::functor_aux::iterate arg_;
            typedef as2::result_of::deque<int>::type put_;
            typedef as2::result_of::option_iterate<put_, arg_>::type result1_;
            typedef as2::modifier_tag::iterate<arg_> tag1_;
            typedef as2:: interpreter_aux::replace_modifier_tag<put_> meta2_;
            typedef ::boost::mpl::apply1<meta2_, tag1_>::type result2_;
            BOOST_MPL_ASSERT(( boost::is_same<result1_, result2_> ));
        }
        {
            //[test_iterate2
            typedef as2::result_of::csv_deque<int>::type interpreter_;
            interpreter_ interpreter = as2::csv_deque<int>
                ( 1, 2, 3 ).for_each( std::list<int>( 7 ) );

            int index = 3;
            BOOST_AUTO(
                _modifier,
                ( as2::_iterate = boost::lambda::var( index )++ )
            );
            BOOST_ASSIGN_V2_CHECK(
                boost::range::equal(
                    csv(
                        interpreter % _modifier,
                        4, 5, 6, 7, 8, 9, 10
                    ).container(),
                    as2::csv_deque( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 )
                )
            );
            //]
        }
    }

}// xxx_iterate
}// xxx_option
}// test_assign_v2
