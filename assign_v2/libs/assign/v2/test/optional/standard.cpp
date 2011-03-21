//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <deque>
#include <list>
#include <queue>
#include <set>
#include <string>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/put.hpp>
#include <boost/assign/v2/deque.hpp>
// Options come next
#include <boost/assign/v2/optional/standard.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/algorithm/lower_bound.hpp>
#include <boost/range/algorithm/upper_bound.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/typeof/typeof.hpp>
#include <libs/assign/v2/test/optional/standard.h>

namespace test_assign_v2{
namespace xxx_optional{
namespace xxx_standard{

    void test()
    {
        using namespace boost;
        namespace as2 = assign::v2;

        // ------------------------------ WARNING ---------------------------- //
        // Don't misconstrue the commands in the tests below as *necessary* to //
        // obtain particular implementation. In most cases, the default is     //
        // already set at that invoked with operator%                          //
        // ------------------------------------------------------------------- //

        // PUT
        {
            //[test_optional_standard_meta
            typedef as2::result_of::put<std::vector<int> >::type put_;
            typedef as2::aux::keyword_standard_modifier keyword_;
            typedef as2::modifier_tag::push_front tag_;
            typedef as2::result_of::optional_modifier<put_> meta1_;
            typedef ::boost::mpl::apply2<meta1_, keyword_, tag_>::type result1_;
            typedef as2::aux::replace_modifier_tag<put_> meta2_;
            typedef ::boost::mpl::apply1<meta2_, tag_>::type result2_;
            BOOST_MPL_ASSERT(( boost::is_same<result1_, result2_> ));
            //]
        }
        {
        	// fully qual boost::begin/end demanded by MSVC - error C2668
            //[test_optional_push_front
            std::deque<double> sqrt2;
            ( as2::put( sqrt2 ) % as2::_push_front )( 1.41421 )( 1.4142 )( 1.414 )( 1.41 );


            BOOST_ASSIGN_V2_CHECK( boost::lower_bound( sqrt2, 1.41 ) == boost::begin( sqrt2 ) );
            BOOST_ASSIGN_V2_CHECK( boost::upper_bound( sqrt2, 1.41421 ) == boost::end( sqrt2 ) );
            //]
        }
        {
            //[test_optional_push
            std::queue<int> fifo; ( as2::put( fifo ) % as2::_push )( 72 )( 31 )( 48 );

            BOOST_ASSIGN_V2_CHECK( fifo.front() == 72 );
            BOOST_ASSIGN_V2_CHECK( fifo.back() == 48 );
            //]
        }
        {
            //[test_optional_insert
            std::set<std::string> letters; ( as2::put( letters ) % as2::_insert )( "d" )( "a" )( "c" )( "b" );

            BOOST_ASSIGN_V2_CHECK( letters.lower_bound( "a" ) == boost::begin( letters ) );
            BOOST_ASSIGN_V2_CHECK( letters.upper_bound( "d" ) == boost::end( letters ) );
            //]
        }
        {
            //[test_optional_push_back
            std::list<int> list;
            ( as2::put( list ) % as2::_push_back )( 72 )( 31 )( 48 );

            BOOST_ASSIGN_V2_CHECK( range::equal( list, as2::csv_deque( 72, 31, 48 ) ) );
            //]
        }
        // DEQUE
        {
            //[test_optional_meta_deque
            typedef as2::result_of::deque<int>::type put_;
            typedef as2::aux::keyword_standard_modifier keyword_;
            typedef as2::modifier_tag::push_front tag_;
            typedef as2::result_of::optional_modifier<put_> meta1_;
            typedef ::boost::mpl::apply2<meta1_, keyword_, tag_>::type result1_;
            typedef as2::aux::replace_modifier_tag<put_> meta2_;
            typedef ::boost::mpl::apply1<meta2_, tag_>::type result2_;

            BOOST_MPL_ASSERT(( boost::is_same<result1_, result2_> ));
            //]
        }
        {
            //[test_optional_push_front_deque
            BOOST_AUTO(
                powers,
                ( as2::deque<int>( as2::_nil ) % as2::_push_front )( 16 )( 8 )( 4 )( 2 )( 1 )
            );

            BOOST_ASSIGN_V2_CHECK( range::equal( powers, as2::csv_deque( 1, 2, 4, 8, 16 ) ) );
            //]
        }
    }

}// xxx_standard
}// xxx_optional
}// test_assign_v2
