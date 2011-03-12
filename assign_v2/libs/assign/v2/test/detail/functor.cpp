//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#include <boost/assign/v2/detail/functor/constructor.hpp>
#include <boost/tuple/tuple.hpp>
#include <libs/assign/v2/test/detail/functor.h>

namespace test_assign_v2{
namespace xxx_detail{
namespace xxx_functor{


    void test(){
        using namespace boost;
        namespace as2 = assign::v2;
#if !BOOST_ASSIGN_V2_ENABLE_CPP0X
// Requirement specific to this test file
#if BOOST_ASSIGN_V2_LIMIT_LVALUE_CONST_ARITY != 2
#error
#endif
#endif

        // Primarily tests whether a functor is overloaded on any combination of 
        // non-const/cont within BOOST_ASSIGN_V2_LIMIT_LVALUE_CONST_ARITY
        // (relevant only for CPP03) and on all-non-const and all-const above
        // this limit.

        {
            typedef int e_; e_ x = -1, y = 1;
            typedef boost::tuple<e_&, e_&> tuple_;

            BOOST_ASSIGN_V2_CHECK( &get<0>( as2::constructor<tuple_>()( x, y ) ) == &x );
            BOOST_ASSIGN_V2_CHECK( &get<1>( as2::constructor<tuple_>()( x, y ) ) == &y );

        }
        {
            typedef int e_; e_ x = -1;
            {
                typedef boost::tuple<e_&, e_ const&> tuple_;

                BOOST_ASSIGN_V2_CHECK( &get<0>( as2::constructor<tuple_>()( x, 1 ) ) == &x );
                BOOST_ASSIGN_V2_CHECK(  get<1>( as2::constructor<tuple_>()( x, 1 ) ) == 1  );
            }
            {
                typedef boost::tuple< e_ const&, e_&> tuple_;

                BOOST_ASSIGN_V2_CHECK(  get<0>( as2::constructor<tuple_>()( 1, x ) ) == 1 );
                BOOST_ASSIGN_V2_CHECK( &get<1>( as2::constructor<tuple_>()( 1, x ) ) == &x  );
            }
        }
        {
            typedef int e_; e_ x = -1, y = 0, z = 1;
            typedef boost::tuple<e_&, e_&, e_&> tuple_;

            BOOST_ASSIGN_V2_CHECK( &get<0>( as2::constructor<tuple_>()( x, y, z ) ) == &x );
            BOOST_ASSIGN_V2_CHECK( &get<1>( as2::constructor<tuple_>()( x, y, z ) ) == &y );
        }
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
        {
            typedef int e_; int y = 0;
            typedef boost::tuple<e_ const&, e_ &, e_ const&> tuple_;

            BOOST_ASSIGN_V2_CHECK(  get<0>( as2::constructor<tuple_>()( -1, y, 1 ) ) == -1 );
            BOOST_ASSIGN_V2_CHECK( &get<1>( as2::constructor<tuple_>()( -1, y, 1 ) ) == &y );
            BOOST_ASSIGN_V2_CHECK(  get<2>( as2::constructor<tuple_>()( -1, y, 1 ) ) == 1 );
        }
#endif
        {
            typedef int e_; 
            typedef boost::tuple<e_ const&, e_ const&, e_ const&> tuple_;

            BOOST_ASSIGN_V2_CHECK( get<0>( as2::constructor<tuple_>()( -1, 0, 1 ) ) == -1 );
            BOOST_ASSIGN_V2_CHECK( get<1>( as2::constructor<tuple_>()( -1, 0, 1 ) ) == 0 );
            BOOST_ASSIGN_V2_CHECK( get<2>( as2::constructor<tuple_>()( -1, 0, 1 ) ) == 1 );
        }
    }

}// xxx_functor
}// xxx_detail
}// test_assign_v2
