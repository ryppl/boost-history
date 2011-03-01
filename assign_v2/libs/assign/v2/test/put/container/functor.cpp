//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/array.hpp>
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>
#include <string>
#include <boost/tuple/tuple.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/container/functor.hpp>

#include <libs/assign/v2/test/put/container/functor.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_container{
namespace xxx_functor{

    void test(){
        namespace as2 = boost::assign::v2;
        {
            //[put_cont_functor_array
            typedef int T; T x = 1, y = 2, z = 3;
            boost::array<T, 3> cont; as2::put( cont )( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( cont[0] == x );
            BOOST_ASSIGN_V2_CHECK( cont[2] == z );
            //]
        }
        {
            // std::string may be replaced by const char* using a custom
            // comparator
            //[put_cont_functor_map
            typedef std::map<std::string, int> C; C cont;
            as2::put( cont )( "jan", 31 )( "feb", 28 )( "mar", 31 );
            BOOST_ASSIGN_V2_CHECK( cont["feb"] == 28 );
            //]
            BOOST_ASSIGN_V2_CHECK( cont["jan"] == 31 );
            BOOST_ASSIGN_V2_CHECK( cont["mar"] == 31 );
        }
        {
        	// Used in intro (same as above but different naming)
            //[put_cont_functor_cal
            typedef std::map<std::string, int> C; C cal;
            /*<< Calls `cal.insert( C::value_type(x, y) )` for [^(x,y)=( "jan", 31 ),( "feb", 28 ),( "mar", 31 )] >>*/
            as2::put( cal )( "jan", 31 )( "feb", 28 )( "mar", 31 );
            BOOST_ASSIGN_V2_CHECK( cal["feb"] == 28 );
            //]
        }
        {
            //[put_cont_functor_set
            typedef std::string T;
            std::set<T> cont;
            T x = "isomer", y = "ephemeral", z = "prosaic";
            as2::put( cont )( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( cont.count( x ) == 1 );
            BOOST_ASSIGN_V2_CHECK( cont.count( z ) == 1 );
            //]
        }
        {
            //[put_cont_functor_deque
            typedef int T; T x = 1, y = 2, z = 0;
            std::deque<T> cont; as2::put( cont )( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( cont.front() == x );
            BOOST_ASSIGN_V2_CHECK( cont.back() == z );
            //]
        }
        {
            //[put_cont_functor_list
            typedef int T; T x = 1, y = 2, z = 0;
            std::list<T> cont; ( as2::put( cont ) )( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( cont.front() == x );
            BOOST_ASSIGN_V2_CHECK( cont.back() == z );
            //]
        }
        {
            //[put_cont_functor_vector
            typedef int T; T x = 1, y = 2, z = 0;
            std::vector<T> cont; as2::put( cont )( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( cont.front() == x );
            BOOST_ASSIGN_V2_CHECK( cont.back() == z );
            //]
        }
        {
            //[put_cont_functor_queue
            typedef int T; T x = 8, y = 7, z = 4;
            std::queue<T> fifo; as2::put( fifo )( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( fifo.front() == x );
            BOOST_ASSIGN_V2_CHECK( fifo.back() == z );
            //]
        }
        {
            //[put_cont_functor_stack
            typedef int T; T x = 8, y = 7, z = 4;
            std::stack<T> lifo; as2::put( lifo )( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( lifo.top() == z ); lifo.pop();
            BOOST_ASSIGN_V2_CHECK( lifo.top() == y ); lifo.pop();
            BOOST_ASSIGN_V2_CHECK( lifo.top() == x ); lifo.pop();
            //]
        }
        {
            using namespace boost;
            //[put_cont_functor_tuple
            typedef int i_; typedef double d_;
            typedef boost::tuple</*<< Notice this is a reference >>*/ d_&, i_> T;
            d_ w = 2.7,  x = 2.71, y = 2.718, z = 0.0; std::vector<T> cont;
            as2::put( cont )/*<< Notice the binary calls >>*/( w, 1 )( x, 2 )( y, 3 )/*<< Notice the unary call >>*/( z );
            BOOST_ASSIGN_V2_CHECK( &get<0>( cont[1] ) == &x );
            BOOST_ASSIGN_V2_CHECK( get<1>( cont[1] ) == 2 );
            T t3( z );
            BOOST_ASSIGN_V2_CHECK( &get<0>( cont[3] ) == &z );
            BOOST_ASSIGN_V2_CHECK( get<1>( cont[3] ) == get<1>( t3 ) );
            //]
        }
    }// test()

}// xxx_functor
}// xxx_container
}// xxx_put
}// xxx_test_assign
