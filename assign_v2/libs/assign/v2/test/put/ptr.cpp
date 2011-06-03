//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <string>
#include <utility>
#include <boost/assign/v2/support/config/check.hpp>
#include <boost/assign/v2/include/csv_put.hpp>
#include <boost/assign/v2/include/put.hpp>
#include <boost/assign/v2/support/pp/ignore.hpp>
#include <boost/ptr_container/ptr_array.hpp>
#include <boost/ptr_container/ptr_deque.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/ptr_set.hpp>
#include <boost/ptr_container/ptr_unordered_set.hpp>
#include <boost/ptr_container/ptr_unordered_map.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <libs/assign/v2/test/put/ptr.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_ptr{

    void test(){

        namespace as2 = boost::assign::v2;

        // ARRAY
        {
            //[test_csv_put_ptr1
            typedef int T;
            T x = 1, y = 2, z = 3; boost::ptr_array<T, 3> cont;
            /*<<Calls `assoc[i++] = new T( t )` for `i = 0` and `t`[^ = ] `x`, `y`, and `z`>>*/as2::csv_put( cont, x, y, z );

            BOOST_ASSIGN_V2_CHECK( cont.front() == x );
            BOOST_ASSIGN_V2_CHECK( cont.back() == z );
            //]
        }
        // SEQUENCE
        {
            //[test_csv_put_ptr2
            typedef int T; T x = 1, y = 2, z = 0; boost::ptr_deque<T> cont;
            /*<<Calls `assoc.push_back( new T( t ) )` for `t` [^=] `x`, `y`, and `z`>>*/as2::csv_put( cont, x, y, z );

            BOOST_ASSIGN_V2_CHECK( cont.front() == x );
            BOOST_ASSIGN_V2_CHECK( cont.back() == z );
            //]
        }
        // MAP
        {
            //[test_put_ptr1
            typedef std::string word_;
            const char x[] = "foo";
            const char y[4] = { 'b', 'a', 'r', '\0' };
            word_ z = "***baz";
            boost::ptr_unordered_map<int, word_> map;

            as2::put( map )( 1, x, 3 )( 2, y )( 3, z, 3, 3 )( 4, "qux" );

            assert( map[1] == "foo" ); assert( map[2] == "bar" );
            assert( map[3] == "baz" ); assert( map[4] == "qux" );
            //]
        }
        {
            //[test_csv_put_ptr3
            typedef std::string month_; typedef int days_;
            boost::ptr_map<month_, days_> q1;

            as2::csv_put<2>( q1, "jan", 31, "feb", 28, "mar", 31 );

            BOOST_ASSIGN_V2_CHECK( q1["jan"] == 31 );
            BOOST_ASSIGN_V2_CHECK( q1["feb"] == 28 );
            BOOST_ASSIGN_V2_CHECK( q1["mar"] == 31 );
            //]
        }
        // SET
        {
            //[test_csv_put_ptr4
            typedef std::string T; boost::ptr_set<T> assoc;
            T x = "isomer", y = "ephemeral", z = "prosaic";
            as2::csv_put( assoc, x, z, y );

            BOOST_ASSIGN_V2_CHECK( assoc.count( x ) == 1 );
            BOOST_ASSIGN_V2_CHECK( assoc.count( z ) == 1 );
            //]
        }
        {
            //[test_csv_put_ptr5
            boost::ptr_unordered_set<std::string> set;
            as2::csv_put( set, "foo", "bar", "baz" );

            BOOST_ASSIGN_V2_CHECK( set.count( "foo" ) == 1 );
            BOOST_ASSIGN_V2_CHECK( set.count( "baz" ) == 1 );
            //]
        }
    }// test

}// xxx_ptr
}// xxx_put
}// xxx_test_assign
