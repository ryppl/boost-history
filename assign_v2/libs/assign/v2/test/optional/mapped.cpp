//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <map>
#include <string>
#include <cmath>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/put.hpp>
#include <boost/assign/v2/deque.hpp>
// Options come next
#include <boost/assign/v2/optional/key.hpp>
#include <boost/assign/v2/optional/mapped.hpp> 
#include <boost/lambda/lambda.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/tuple/tuple.hpp>
#include <libs/assign/v2/test/optional/mapped.h>

namespace test_assign_v2{
namespace xxx_optional{
namespace xxx_mapped{

    void test()
    {

        using namespace boost;
        namespace as2 = assign::v2;
        {
            //[test_optional_mapped_meta
            using namespace lambda;
            typedef BOOST_TYPEOF(_1) arg_;
            typedef as2::aux::keyword_mapped keyword_;
            typedef as2::aux::optional_modifier<keyword_, arg_> modulo_;
            typedef std::map<std::string, int> cont_;
            typedef as2::result_of::put<cont_>::type put_;
            typedef as2::result_of::optional_modifier<put_> meta1_;
            typedef ::boost::mpl::apply2<meta1_, keyword_, arg_>::type result1_;
            typedef as2::modifier_tag::mapped<arg_> tag1_;
            typedef as2::aux::replace_modifier_tag<put_> meta2_;
            typedef ::boost::mpl::apply1<meta2_, tag1_>::type result2_;
            BOOST_MPL_ASSERT((boost::is_same<result1_, result2_>));
            //]
        }
        {
            //[test_optional_mapped_map
            using namespace lambda;
            typedef std::string str_; typedef std::map<str_, int> C; C year; 
            ( 
                as2::put( year )( "feb", 28 )( "apr", 30 )( "jun", 30 )( "sep", 30 )( "nov", 30 ) 
                    % as2::_key %  ( as2::_mapped = ( _1 = 31 ) ) 
            )/*<<Calls `year["jan"] = 31` etc.>>*/( "jan" )( "mar" )( "may" )( "jul" )( "aug" )( "oct" )( "dec" );
            
            BOOST_ASSIGN_V2_CHECK( year["jan"] == 31 );
            BOOST_ASSIGN_V2_CHECK( year["dec"] == 31 );
            //]        
        }
        {
            //[test_optional_mapped_meta_deque
            using namespace lambda;
            typedef BOOST_TYPEOF(_1) arg_;
            typedef as2::aux::keyword_mapped keyword_;
            typedef as2::aux::optional_modifier<keyword_, arg_> modulo_;
            typedef as2::result_of::deque<int>::type put_;
            typedef as2::result_of::optional_modifier<put_> meta1_;
            typedef ::boost::mpl::apply2<meta1_, keyword_, arg_>::type result1_;
            typedef as2::modifier_tag::mapped<arg_> tag1_;
            typedef as2::aux::replace_modifier_tag<put_> meta2_;
            typedef ::boost::mpl::apply1<meta2_, tag1_>::type result2_;
            BOOST_MPL_ASSERT((boost::is_same<result1_, result2_>));
            //]
        }
        {
            //[test_optional_mapped_map_deque
            using namespace lambda;
            BOOST_AUTO(
                days_in_first_quater,
                ( 
                    as2::deque<int>( 31 )( 31 )( 31 ) % ( as2::_mapped = ( _1 -= 3 ) ) 
                )( 1 ) 
            );
            BOOST_ASSIGN_V2_CHECK( days_in_first_quater[0] == 31 );
            BOOST_ASSIGN_V2_CHECK( days_in_first_quater[1] == 28 );
            BOOST_ASSIGN_V2_CHECK( days_in_first_quater[2] == 31 );
            //]
        }
    }

}// xxx_mapped
}// xxx_optional
}// test_assign_v2
