////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                       //
//                                                                        //
//  Copyright (C) 2003-2004 Thorsten Ottosen                              //
//  Copyright (C) 2010 Erwann Rogard                                      //
//  Use, modification and distribution are subject to the                 //
//  Boost Software License, Version 1.0. (See accompanying file           //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)      //
////////////////////////////////////////////////////////////////////////////
#include <string>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/assign/v2/put/deque/csv_deque.hpp>
#include <boost/assign/v2/put/deque/deque.hpp>

#include <libs/assign/v2/test/put/deque.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_deque{

    void test()
    {

        using namespace boost;
        namespace as2 = assign::v2;    
        {
            //[test_put_deque_ints
            typedef as2::result_of::deque<int>::type C0;
            C0 empty_cont = as2::deque<int>( as2::_nil );

            /*<-*/BOOST_ASSIGN_V2_CHECK( BOOST_ASSIGN_V2_IGNORE(/*->*/assert( /*<-*/))/*->*/ empty_cont( 72 )( 31 )( 48 ).front() == 72 );
            /*<-*/BOOST_ASSIGN_V2_CHECK( BOOST_ASSIGN_V2_IGNORE(/*->*/assert( /*<-*/))/*->*/ as2::deque<int>( 72 )( 31 )( 48 ).back() == 48 );

            typedef as2::result_of::csv_deque<int>::type C1;
            C1 cont = as2::csv_deque( 72, 31, 48 );

            /*<-*/BOOST_ASSIGN_V2_CHECK( BOOST_ASSIGN_V2_IGNORE(/*->*/assert( /*<-*/))/*->*/ cont.front() == 72 );
            /*<-*/BOOST_ASSIGN_V2_CHECK( BOOST_ASSIGN_V2_IGNORE(/*->*/assert( /*<-*/))/*->*/ as2::csv_deque( 72 )( 31 )( 48 ).back() == 48 );

			BOOST_MPL_ASSERT(( is_same<C0, C1> ));

            //]
        }
        {
            //[test_put_deque_str_literal
            typedef std::string str_;
            /*<-*/BOOST_ASSIGN_V2_CHECK( BOOST_ASSIGN_V2_IGNORE(/*->*/assert( /*<-*/))/*->*/
                str_( as2::deque<const char*>( "x" )( "y" )( "z" )[1] ) == "y"
            );
            /*<-*/BOOST_ASSIGN_V2_CHECK( BOOST_ASSIGN_V2_IGNORE(/*->*/assert( /*<-*/))/*->*/
                str_( as2::csv_deque( "x", "y", "z" )[1] ) == "y"
            );
            //]
        }
    }

}// xxx_deque
}// xxx_put
}// test_assign_v2
