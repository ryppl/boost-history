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

#include <boost/assign/v2/value/fun.hpp>
#include <boost/assign/v2/value/modifier/lookup.hpp>
#include <boost/assign/v2/value/put.hpp>
#include <boost/assign/v2/value/deque.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/tuple/tuple.hpp>
#include <libs/assign/v2/test/value/modifier/lookup.h>

namespace test_assign_v2{
namespace xxx_value{
namespace xxx_modifier{
namespace xxx_lookup{

    void test()
    {
        using namespace boost;
        using namespace lambda;
        namespace as2 = assign::v2;
        {
            //[test_put_modifier_lookup_meta
            typedef BOOST_TYPEOF(_1) arg_;
            typedef as2::value_aux::keyword_lookup keyword_;
            typedef as2::value_aux::modulo_modifier<keyword_, arg_> modulo_;
            typedef std::map<std::string, int> cont_;
            typedef as2::result_of::put<cont_>::type put_;
            typedef as2::result_of::modulo_modifier<put_> meta1_;
            typedef ::boost::mpl::apply2<meta1_, keyword_, arg_>::type result1_;
            typedef as2::modifier_tag::lookup<arg_> tag1_;
            typedef as2::value_aux::replace_modifier_tag<put_> meta2_;
            typedef ::boost::mpl::apply1<meta2_, tag1_>::type result2_;
            BOOST_MPL_ASSERT((boost::is_same<result1_, result2_>));
            //]
        }
        {
            //[test_put_modifier_lookup_map
            typedef std::map<std::string, int> C; C year; ( as2::put( year )( "feb", 28 )( "apr", 30 )( "jun", 30 )( "sep", 30 )( "nov", 30 ) 
                % ( as2::_fun = _1 ) %  ( as2::_lookup = ( _1 = 31 ) ) )/*<<Calls `year["jan"] = 31` etc.>>*/( "jan" )( "mar" )( "may" )( "jul" )( "aug" )( "oct" )( "dec" );
            
            BOOST_ASSIGN_V2_CHECK( year["jan"] == 31 );
            BOOST_ASSIGN_V2_CHECK( year["dec"] == 31 );
            //]        
        }
        {
            //[test_put_modifier_lookup_meta_deque
            typedef BOOST_TYPEOF(_1) arg_;
            typedef as2::value_aux::keyword_lookup keyword_;
            typedef as2::value_aux::modulo_modifier<keyword_, arg_> modulo_;
            typedef as2::result_of::deque<int>::type put_;
            typedef as2::result_of::modulo_modifier<put_> meta1_;
            typedef ::boost::mpl::apply2<meta1_, keyword_, arg_>::type result1_;
            typedef as2::modifier_tag::lookup<arg_> tag1_;
            typedef as2::value_aux::replace_modifier_tag<put_> meta2_;
            typedef ::boost::mpl::apply1<meta2_, tag1_>::type result2_;
            BOOST_MPL_ASSERT((boost::is_same<result1_, result2_>));
            //]
        }
        {
            //[test_put_modifier_lookup_map_deque
            BOOST_AUTO(
                days_in_first_quater,
                ( 
                    as2::deque<int>( 31 )( 31 )( 31 ) % ( as2::_lookup = ( _1 -= 3 ) ) 
                )( 1 ) 
            );
            BOOST_ASSIGN_V2_CHECK( days_in_first_quater[0] == 31 );
            BOOST_ASSIGN_V2_CHECK( days_in_first_quater[1] == 28 );
            BOOST_ASSIGN_V2_CHECK( days_in_first_quater[2] == 31 );
            //]
        }
    }

}// xxx_lookup
}// xxx_modifier
}// xxx_value
}// test_assign_v2
