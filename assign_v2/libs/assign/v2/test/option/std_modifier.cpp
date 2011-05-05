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
#include <boost/assign/v2/include/csv_put.hpp>
#include <boost/assign/v2/deque.hpp>
#include <boost/assign/v2/option/modifier/std.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/typeof/typeof.hpp>
#include <libs/assign/v2/test/option/std_modifier.h>

namespace test_assign_v2{
namespace xxx_option{
namespace xxx_standard{

    void test()
    {
        namespace as2 = boost::assign::v2;

        // PUT
        {
            //[test_option_standard_meta
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
            // fully qual boost::begin/end demanded by MSVC - error C2668
            //[test_option_push_front
            boost::circular_buffer<int> cb( 3 ); 
            
            BOOST_ASSIGN_V2_CHECK(
                boost::range::equal(
                    cb | as2::delay_csv_put( as2::_push_front, as2::csv_deque( 3, 2, 1 ) ), 
                    as2::csv_deque( 1, 2, 3 ) 
                )
            );

            BOOST_ASSIGN_V2_CHECK(
                boost::range::equal(
                    cb | as2::delay_csv_put( as2::csv_deque( 4, 5 ) ), 
                    as2::csv_deque( 3, 4, 5 ) 
                )
            );    

            //]
        }
        {
            //[test_option_push
            // NB: This option is dedundant with the default
            std::queue<int> fifo; as2::csv_put( fifo, as2::_option % as2::_push, 1, 10, 100 );

            BOOST_ASSIGN_V2_CHECK( fifo.front() == 1 );
            BOOST_ASSIGN_V2_CHECK( fifo.back() == 100 );
            //]
        }
        {
            //[test_option_insert
            // NB: This option is dedundant with the default
            std::set<std::string> letters; 
            as2::csv_put( letters, as2::_option % as2::_insert, "d", "a", "c", "b" );

            BOOST_ASSIGN_V2_CHECK( letters.lower_bound( "a" ) == boost::begin( letters ) );
            BOOST_ASSIGN_V2_CHECK( letters.upper_bound( "d" ) == boost::end( letters ) );
            //]
        }
        {
            //[test_option_push_back
            // NB: This option is dedundant with the default
            std::list<int> list;
            as2::csv_put( list, as2::_option % as2::_push_back, 1, 10, 100 );

            BOOST_ASSIGN_V2_CHECK( 
                boost::range::equal( list, as2::csv_deque( 1, 10, 100 ) ) 
            );
            //]
        }
        // DEQUE
        {
            //[test_option_meta_deque
            typedef as2::result_of::deque<int>::type put_;
            typedef as2::modifier_tag::push_front tag_;
            typedef as2::result_of::option_push_front<put_>::type result1_;
            typedef as2:: interpreter_aux::replace_modifier_tag<put_> meta2_;
            typedef ::boost::mpl::apply1<meta2_, tag_>::type result2_;

            BOOST_MPL_ASSERT(( boost::is_same<result1_, result2_> ));
            //]
        }
        {
            //[test_option_push_front_deque
            BOOST_AUTO(
                two_power_n,
                as2::csv_deque( as2::_option % as2::_push_front, 16, 8, 4, 2, 1 )
            );

            BOOST_ASSIGN_V2_CHECK( 
                boost::range::equal( two_power_n, as2::csv_deque( 1, 2, 4, 8, 16 ) ) 
            );
            //]
        }
    }

}// xxx_standard
}// xxx_option
}// test_assign_v2
