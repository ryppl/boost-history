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
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/assign/v2/put/put.hpp>
#include <boost/ptr_container/ptr_array.hpp>
#include <boost/ptr_container/ptr_deque.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/ptr_set.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <libs/assign/v2/test/put/ptr.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_ptr{

    void test(){

        namespace as2 = boost::assign::v2;

        {
            //[test_put_ptr_array
            typedef int T;
            T x = 1, y = 2, z = 3; boost::ptr_array<T, 3> cont;
            as2::put( cont )/*<<Calls `assoc[i++] = new T( t )` for `i = 0` and `t`[^ = ] `x`, `y`, and `z`>>*/( x )( y )( z );
            
            BOOST_ASSIGN_V2_CHECK( cont.front() == x );
            BOOST_ASSIGN_V2_CHECK( cont.back() == z );
            //]
        }
        {
            //[test_put_ptr_deque
            typedef int T; T x = 1, y = 2, z = 0; boost::ptr_deque<T> cont;
            as2::put( cont )/*<<Calls `assoc.push_back( new T( t ) )` for `t` [^=] `x`, `y`, and `z`>>*/( x )( y )( z );

            BOOST_ASSIGN_V2_CHECK( cont.front() == x );
            BOOST_ASSIGN_V2_CHECK( cont.back() == z );
            //]
        }
        {
            //[test_put_ptr_list
            typedef int T; T x = 1, y = 2, z = 0; boost::ptr_list<T> cont;
            as2::put( cont )/*<<Calls `assoc.push_back( new T( t ) )` for `t` [^=] `x`, `y`, and `z`>>*/( x )( y )( z );

            BOOST_ASSIGN_V2_CHECK( cont.front() == x );
            BOOST_ASSIGN_V2_CHECK( cont.back() == z );
            //]
        }
        {
            //[test_put_ptr_vector
            typedef int T; T x = 1, y = 2, z = 0; boost::ptr_vector<T> cont;
            as2::put( cont )/*<<Calls `assoc.push_back( new T( t ) )` for `t` [^=] `x`, `y`, and `z`>>*/( x )( y )( z );

            BOOST_ASSIGN_V2_CHECK( cont.front() == x );
            BOOST_ASSIGN_V2_CHECK( cont.back() == z );
            //]
        }
        // TODO
/*
        {
            //[put_ptr_map
            boost::ptr_map<std::string, int> assoc;
            as2::put( assoc )( "jan", 31 )( "feb", 28 )( "mar", 31 );
            //]
        }
*/
        {
            //[put_ptr_set
            typedef std::string T; boost::ptr_set<T> assoc;
            T x = "isomer", y = "ephemeral", z = "prosaic";
            as2::put( assoc )/*<<Calls `assoc.insert( new T( t ) )` for `t` [^=] `x`, `y`, and `z`>>*/( x )( z )( y );
            BOOST_ASSIGN_V2_CHECK( assoc.count( x ) == 1 );
            BOOST_ASSIGN_V2_CHECK( assoc.count( z ) == 1 );
            //]
        }
    }// test

}// xxx_ptr
}// xxx_put
}// xxx_test_assign
