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
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/assign/v2/put.hpp>
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
        {
            //[put_ptr_map
            typedef std::string key_;
            boost::ptr_map<key_, int> assoc;
            //( as2::put( assoc ) % _key % _mapped ) ( "jan", 31 )( "feb", 28 )( "mar", 31 );
			//key_ key = "jan";
            //std::pair<key_, int*> p( key, new int(31) );
			//assoc.insert( p );
            //]
        }
        {
            //[put_ptr_set
            typedef std::string T; boost::ptr_set<T> assoc;
            T x = "isomer", y = "ephemeral", z = "prosaic";
            as2::put( assoc )/*<<Calls `assoc.insert( new T( t ) )` for `t` [^=] `x`, `y`, and `z`>>*/( x )( z )( y );
            BOOST_ASSIGN_V2_CHECK( assoc.count( x ) == 1 );
            BOOST_ASSIGN_V2_CHECK( assoc.count( z ) == 1 );
            //]
        }
        {/*	
        	//[test_put_put_unordered_map
			boost::ptr_unordered_map<std::string, int> map; 
            as2::put( map )("foo", 1)("bar", 2)("baz", 3);

            BOOST_ASSIGN_V2_CHECK( map["foo"] == 1 );
            BOOST_ASSIGN_V2_CHECK( map["baz"] == 3 );
            //]
        */}
        {	
        	//[test_put_put_unordered_set
			boost::ptr_unordered_set<std::string> set; 
            as2::put( set )("foo")("bar")("baz");

            BOOST_ASSIGN_V2_CHECK( set.count("foo") == 1 );
            BOOST_ASSIGN_V2_CHECK( set.count("baz") == 1 );
            //]
        }
    }// test

}// xxx_ptr
}// xxx_put
}// xxx_test_assign
