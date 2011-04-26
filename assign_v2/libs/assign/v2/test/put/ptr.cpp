//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <string>
#include <utility>
#include <boost/assign/v2/support/config/check.hpp>
#include <boost/assign/v2/support/pp/ignore.hpp>
#include <boost/assign/v2/include/csv_put.hpp>
#include <boost/assign/v2/include/put.hpp>
#include <boost/scoped_ptr.hpp>
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
            //[test_put_ptr_array
            typedef int T;
            T x = 1, y = 2, z = 3; boost::ptr_array<T, 3> cont;
            /*<<Calls `assoc[i++] = new T( t )` for `i = 0` and `t`[^ = ] `x`, `y`, and `z`>>*/as2::csv_put( cont, x, y, z );
            
            BOOST_ASSIGN_V2_CHECK( cont.front() == x );
            BOOST_ASSIGN_V2_CHECK( cont.back() == z );
            //]
        }
        // SEQUENCE
        {
            //[test_put_ptr_deque
            typedef int T; T x = 1, y = 2, z = 0; boost::ptr_deque<T> cont;
            /*<<Calls `assoc.push_back( new T( t ) )` for `t` [^=] `x`, `y`, and `z`>>*/as2::csv_put( cont, x, y, z );

            BOOST_ASSIGN_V2_CHECK( cont.front() == x );
            BOOST_ASSIGN_V2_CHECK( cont.back() == z );
            //]
        }
        // MAP
        {
            //[test_put_ptr_map
            typedef std::string key_;
            boost::ptr_map<key_, int> assoc;
            as2::put( assoc )( "jan", 31 )( "feb", 28 )( "mar", 31 );

            
            BOOST_ASSIGN_V2_CHECK( assoc["jan"] == 31 );
            BOOST_ASSIGN_V2_CHECK( assoc["mar"] == 31 );
            //]
        }
        {    
            //[test_put_ptr_unordered_map
            typedef std::string word_; 
            const char x[] = "foo";
            const char y[4] = { 'b', 'a', 'r', '\0' };
            word_ z = "***baz";
            boost::ptr_unordered_map<int, word_> map;
            as2::put( map )( 1, x, 3 )( 2, y )( 3, z, 3, 3 )( 4, "qux");

            assert( map[1] == "foo" ); assert( map[2] == "bar" );
            assert( map[3] == "baz" ); assert( map[4] == "qux" );
            //]
        }
        {
            //[test_csv_put_ptr_map
            typedef std::string key_; typedef int days_;
            typedef boost::ptr_map<key_, days_> C; 
            typedef as2::value_container_value<C>::type /*<<Same as `std::map<key_, days_>::value_type`>>*/T;
            C map; as2::csv_put( map, T("jan", 31 ), T( "feb", 28 ), T( "mar", 31 ) );

            
            BOOST_ASSIGN_V2_CHECK( map["jan"] == 31 );
            BOOST_ASSIGN_V2_CHECK( map["mar"] == 31 );
            //]
        }
        // SET
        {
            // Shows that x, y, z can be variadic
            //[test_csv_put_ptr_set
            typedef std::string T; boost::ptr_set<T> assoc;
            T x = "isomer", y = "ephemeral", z = "prosaic";
            as2::csv_put( assoc, x, z, y );

            BOOST_ASSIGN_V2_CHECK( assoc.count( x ) == 1 );
            BOOST_ASSIGN_V2_CHECK( assoc.count( z ) == 1 );
            //]
        }
        {    
            //[test_csv_put_ptr_unordered_set
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
