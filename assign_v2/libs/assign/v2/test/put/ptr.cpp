//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
/*
#include <string>
#include <boost/ptr_container/ptr_array.hpp>
#include <boost/ptr_container/ptr_deque.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/ptr_set.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/put.hpp>
*/
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <libs/assign/v2/test/put/ptr.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_ptr{

    void test(){
// TODO         
/*
        namespace as2 = boost::assign::v2;
        {
            //[put_ptr_array
            typedef int T;
            T x = 1, y = 2, z = 3; boost::ptr_array<T, 3> cont;
            as2::put( cont )( x )( y )( z );
            /*<-*/BOOST_ASSIGN_V2_CHECK( BOOST_ASSIGN_V2_IGNORE(/*->*/assert(/*<-*/))/*->*/cont[0] == x );
            /*<-*/BOOST_ASSIGN_V2_CHECK( BOOST_ASSIGN_V2_IGNORE(/*->*/assert(/*<-*/))/*->*/cont[2] == z );
            //]
        }
        {
            //[put_ptr_deque
            typedef int T; T x = 1, y = 2, z = 0; boost::ptr_deque<T> cont;
            as2::put( cont )( x )( y )( z );
            /*<-*/BOOST_ASSIGN_V2_CHECK( BOOST_ASSIGN_V2_IGNORE(/*->*/assert(/*<-*/))/*->*/cont.front() == x );
            /*<-*/BOOST_ASSIGN_V2_CHECK( BOOST_ASSIGN_V2_IGNORE(/*->*/assert(/*<-*/))/*->*/cont.back() == z );
            //]
        }
        {
            //[put_ptr_list
            typedef int T; T x = 1, y = 2, z = 0; boost::ptr_list<T> cont;
            as2::put( cont )( x )( y )( z );
            /*<-*/BOOST_ASSIGN_V2_CHECK( BOOST_ASSIGN_V2_IGNORE(/*->*/assert(/*<-*/))/*->*/cont.front() == x );
            /*<-*/BOOST_ASSIGN_V2_CHECK( BOOST_ASSIGN_V2_IGNORE(/*->*/assert(/*<-*/))/*->*/cont.back() == z );
            //]
        }
        {
            //[put_ptr_vector
            typedef int T; T x = 1, y = 2, z = 0; boost::ptr_vector<T> cont;
            as2::put( cont )( x )( y )( z );
            /*<-*/BOOST_ASSIGN_V2_CHECK( BOOST_ASSIGN_V2_IGNORE(/*->*/assert(/*<-*/))/*->*/cont.front() == x );
            /*<-*/BOOST_ASSIGN_V2_CHECK( BOOST_ASSIGN_V2_IGNORE(/*->*/assert(/*<-*/))/*->*/cont.back() == z );
            //]
        }
*/            


/*
        // These are not yet available (TODO)
        {
            //[put_ptr_map
            boost::ptr_map<std::string, int> assoc;
            as2::put( assoc )( "jan", 31 )( "feb", 28 )( "mar", 31 );
            /*<-*/BOOST_ASSIGN_V2_CHECK( BOOST_ASSIGN_V2_IGNORE(/*->*/assert(/*<-*/))/*->*/assoc["feb"] == 28 );
            //]
        }
        {
            //[put_ptr_set
            typedef std::string T;
            boost::ptr_set<T> assoc;
            T x = "isomer", y = "ephemeral", z = "prosaic";
            as2::put( assoc )( x )( y )( z );
            /*<-*/BOOST_ASSIGN_V2_CHECK( BOOST_ASSIGN_V2_IGNORE(/*->*/assert(/*<-*/))/*->*/assoc.count( x ) == 1 );
            /*<-*/BOOST_ASSIGN_V2_CHECK( BOOST_ASSIGN_V2_IGNORE(/*->*/assert(/*<-*/))/*->*/assoc.count( z ) == 1 );
            //]
        }
*/
    }// test

}// xxx_ptr
}// xxx_put
}// xxx_test_assign
