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
#include <boost/ptr_container/ptr_array.hpp>
#include <boost/ptr_container/ptr_deque.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/ptr_set.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/container/functor.hpp>

#include <libs/assign/v2/test/put/ptr/functor.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_ptr{
namespace xxx_functor{

    void test(){
        namespace as2 = boost::assign::v2;
        {    
            //[array    
            typedef int T;
            T x = 1, y = 2, z = 3;
            boost::ptr_array<T, 3> ar;
            as2::put( ar )( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( ar[0] == x );
            BOOST_ASSIGN_V2_CHECK( ar[2] == z );
            //]
        }
/*        
        {
            //[map
            boost::ptr_map<std::string, int> assoc;
            as2::put( assoc )( "jan", 31 )( "feb", 28 )( "mar", 31 );
            BOOST_ASSIGN_V2_CHECK( assoc["feb"] == 28 );
            //]
        }
        {
            //[set
            typedef std::string T;
            boost::ptr_set<T> assoc;
            T x = "isomer", y = "ephemeral", z = "prosaic";
            as2::put( assoc )( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( assoc.count( x ) == 1 );
            BOOST_ASSIGN_V2_CHECK( assoc.count( z ) == 1 );
            //]
        }                    
*/
        {
            //[deque
            typedef int T; T x = 1, y = 2, z = 0;
            boost::ptr_deque<T> seq;
            as2::put( seq )( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( seq.front() == x );
            BOOST_ASSIGN_V2_CHECK( seq.back() == z );
            //]
        }
        {
            //[list
            typedef int T; T x = 1, y = 2, z = 0;
            boost::ptr_list<T> seq;
            ( as2::put( seq ) )( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( seq.front() == x );
            BOOST_ASSIGN_V2_CHECK( seq.back() == z );
            //]
        }
        {
            //[vector
            typedef int T; T x = 1, y = 2, z = 0;
            boost::ptr_vector<T> seq;
            as2::put( seq )( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( seq.front() == x );
            BOOST_ASSIGN_V2_CHECK( seq.back() == z );
            //]
        }
    }// test

}// xxx_functor
}// xxx_ptr
}// xxx_put
}// xxx_test_assign
