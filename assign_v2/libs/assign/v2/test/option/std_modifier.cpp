//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <deque>
#include <list>
#include <queue>
#include <set>
#include <string>
#include <boost/assign/v2/support/config/check.hpp>
#include <boost/assign/v2/include/csv_put_basic.hpp>
#include <boost/assign/v2/include/put.hpp>
#include <boost/assign/v2/deque.hpp>
#include <boost/assign/v2/option/modifier/std.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/typeof/typeof.hpp>
#include <libs/assign/v2/test/option/std_modifier.h>

namespace test_assign_v2{
namespace xxx_option{
namespace xxx_std_modifier{

    void test()
    {
        namespace as2 = boost::assign::v2;

        // PUT
        {
            typedef as2::result_of::put<std::vector<int> >::type put_;
            typedef as2::modifier_tag::push_front tag_;
            typedef as2::result_of::option_std_modifier<
                put_, tag_
            >::type result1_;
            typedef as2:: interpreter_aux::replace_modifier_tag<put_> meta2_;
            typedef ::boost::mpl::apply1<meta2_, tag_>::type result2_;
            BOOST_MPL_ASSERT(( boost::is_same<result1_, result2_> ));
            //]
        }
        {
            //[test_push_front1
            boost::circular_buffer<int> cb( 3 );

            as2::csv_put<as2::push_front_>( cb, 3, 2, 1 );

            BOOST_ASSIGN_V2_CHECK(
                boost::range::equal( cb, as2::csv_deque( 1, 2, 3 ) )
            );

            as2::csv_put( cb, 4, 5 );

            BOOST_ASSIGN_V2_CHECK(
                boost::range::equal( cb, as2::csv_deque( 3, 4, 5 ) )
            );

            //]
        }
        {
            //[test_push1
            std::queue<int> fifo;

            // Same as default
            as2::csv_put<as2::push_>( fifo, 1, 10, 100 );

            BOOST_ASSIGN_V2_CHECK( fifo.front() == 1 );
            BOOST_ASSIGN_V2_CHECK( fifo.back() == 100 );
            //]
        }
        {
            //[test_insert1
            // Same as default
            std::set<std::string> letters;
            as2::csv_put<as2::insert_>( letters, "d", "a", "c", "b" );

            BOOST_ASSIGN_V2_CHECK( letters.lower_bound( "a" ) == boost::begin( letters ) );
            BOOST_ASSIGN_V2_CHECK( letters.upper_bound( "d" ) == boost::end( letters ) );
            //]
        }
        {
            //[test_push_back1
            // Same as default
            std::list<int> list;
            as2::csv_put<as2::push_back_>( list, 1, 10, 100 );

            BOOST_ASSIGN_V2_CHECK(
                boost::range::equal( list, as2::csv_deque( 1, 10, 100 ) )
            );
            //]
        }
        // DEQUE
        {
            typedef as2::result_of::deque<int>::type put_;
            typedef as2::modifier_tag::push_front tag_;
            typedef as2::result_of::option_push_front<put_>::type result1_;
            typedef as2:: interpreter_aux::replace_modifier_tag<put_> meta2_;
            typedef ::boost::mpl::apply1<meta2_, tag_>::type result2_;

            BOOST_MPL_ASSERT(( boost::is_same<result1_, result2_> ));
        }
        {
            //[test_push_front2
            BOOST_ASSIGN_V2_CHECK(
                boost::range::equal(
                    as2::csv_deque<int, as2::push_front_>( 16, 8, 4, 2, 1 ),
                    as2::csv_deque( 1, 2, 4, 8, 16 )
                )
            );
            //]
        }
    }

}// xxx_std_modifier
}// xxx_option
}// test_assign_v2
