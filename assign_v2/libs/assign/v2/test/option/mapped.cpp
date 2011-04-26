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
#include <list>
#include <boost/assign/v2/support/config/check.hpp>
#include <boost/assign/v2/include/put.hpp>
#include <boost/assign/v2/deque.hpp>
#include <boost/assign/v2/option/data.hpp>
#include <boost/assign/v2/option/modifier/mapped.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/range/algorithm_ext/iota.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/tuple/tuple.hpp>
#include <libs/assign/v2/test/option/mapped.h>

namespace test_assign_v2{
namespace xxx_option{
namespace xxx_mapped{

    void test()
    {
        namespace as2 = boost::assign::v2;
        {
            //[test_option_mapped_map
            typedef std::string month_; typedef int days_;
            typedef std::map<month_, days_> C; C year;
            (
                as2::put( year )
                    ( "feb", 28 )( "apr", 30 )( "jun", 30 )( "sep", 30 )( "nov", 30 )
                    % ( as2::_data = as2::_key ) % ( as2::_mapped = ( boost::lambda::constant( 31 ) ) )
            )/*<<Calls `year[ month_( "jan" ) ] = 31`>>*/( "jan" )( "mar" )( "may" )( "jul" )( "aug" )( "oct" )( "dec" );
            
            BOOST_ASSIGN_V2_CHECK( year["jan"] == 31 );
            BOOST_ASSIGN_V2_CHECK( year["dec"] == 31 );
            //]
        }
        {
            //[test_option_mapped_meta_deque
            typedef BOOST_TYPEOF(boost::lambda::_1) arg_;
            typedef as2:: interpreter_aux::keyword_mapped keyword_;
            typedef as2::result_of::deque<int>::type put_;
            typedef as2::result_of::option_mapped<put_, arg_>::type result1_;
            typedef as2::modifier_tag::mapped<arg_> tag1_;
            typedef as2:: interpreter_aux::replace_modifier_tag<put_> meta2_;
            typedef ::boost::mpl::apply1<meta2_, tag1_>::type result2_;
            BOOST_MPL_ASSERT((boost::is_same<result1_, result2_>));
            //]
        }
        {
            //[test_option_mapped_deque
            std::list<int> source(10, 1); boost::iota( source, 1 );
            BOOST_AUTO( option, ( as2::_mapped = ( boost::lambda::_1 *= -1 ) ) );
            
            BOOST_ASSIGN_V2_CHECK( 
                boost::range::equal(
                    (
                        as2::deque<int>( as2::as_arg_list( source ) ) 
                        % option
                    )( 1 )( 3 )( 5 )( 7 )( 9 ),
                    as2::csv_deque( +1, -2, +3, -4, +5, -6, +7, -8, +9, -10 )
                )
            );
            //]
        }
    }

}// xxx_mapped
}// xxx_option
}// test_assign_v2
