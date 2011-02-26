//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <utility>
#include <deque>
#include <list>
#include <map>
#include <vector>
#include <boost/array.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/next_prior.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/stable_partition.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/utility/chain.hpp>
#include <boost/assign/v2/ref/wrapper/adaptor_get.hpp>
#include <boost/assign/v2/ref/array/csv.hpp>
#include <boost/assign/v2/ref/array/functor.hpp>
#include <boost/assign/v2/ref/array/functor/converter.hpp>
#include <boost/assign/v2/put/container.hpp>
#include <boost/assign/v2/put/deque/csv.hpp>
#include <boost/assign/v2/put/pipe/functor.hpp>
#include <boost/assign/v2/put/container/functor.hpp>
#include <boost/assign/v2/put/std.hpp>
#include <boost/assign/v2/put/ext.hpp>
#include <boost/assign/v2/utility/csv.hpp>
#include <libs/assign/v2/test/other.h>

namespace test_assign_v2{
namespace xxx_other{

    // Tests interaction between different functionalities

    void test(){

        namespace as2 = boost::assign::v2;
		namespace lambda = boost::lambda;

		// INTRODUCTION
        {
        	// suggested by JB:
            //[csv_put_pipe
            std::deque<int> cont;
            boost::range::stable_partition(
                /*<< Calls `cont.push_back( t )` for [^t=0,...,5], and returns `cont` >>*/
                cont | as2::csv( as2::_put, 0, 1, 2, 3, 4, 5), 
                lambda::_1 % 2
            );
            //]
            //[csv_array
            BOOST_ASSIGN_V2_CHECK(boost::range::equal(
                cont, 
                /*<< The input [^1, 3, ..., 4] is held by reference (not copies) >>*/
                as2::ref::csv_array(1, 3, 5, 0, 2, 4)
            ));
            //]
        }
		// REF + CONVERTER
        {
            //[array_converter
            typedef std::pair<std::string, int> T;
            typedef std::vector<T> cont_;
            cont_ cont = converter( // name lookup
                as2::ref::array( T("jan", 31) )( T("feb", 28) )( T("mar", 31) )
            );
            BOOST_ASSIGN_V2_CHECK( cont[1].first == "feb" );
            BOOST_ASSIGN_V2_CHECK( cont[1].second == 28 );
            //]
            BOOST_ASSIGN_V2_CHECK( cont[0].first == "jan" );
            BOOST_ASSIGN_V2_CHECK( cont[0].second == 31 );
            BOOST_ASSIGN_V2_CHECK( cont[2].first == "mar" );
            BOOST_ASSIGN_V2_CHECK( cont[2].second == 31 );
        }
		// CHAIN + REF
        {
            //[chain_w
            std::vector<int> r( 3 ); r[0] = 1; r[1] = 2; r[2] = 0;
            boost::array<int, 2> cont; int z; // lvalues
            boost::copy(
                r,
                boost::begin(
                    cont | as2::_chain(
                        as2::ref::csv_array( z ) // rvalue!
                            | as2::ref::_get
                    )
                )
            );
            BOOST_ASSIGN_V2_CHECK( cont[0] == r[0] );
            BOOST_ASSIGN_V2_CHECK( cont[1] == r[1] );
            BOOST_ASSIGN_V2_CHECK( z == r[2] );
            //]
        }
		// PUT + CSV
        // container.hpp
        {
            //[csv_put_default
            typedef int T; T x = 1, y = 2, z = 0;
            std::list<T> cont; 
            as2::csv( as2::put( cont ) , x, y, z );
            BOOST_ASSIGN_V2_CHECK( cont.front() == x );
            BOOST_ASSIGN_V2_CHECK( cont.back() == z );
            //]
            BOOST_ASSIGN_V2_CHECK( *boost::next( boost::begin( cont ) ) == y );
        }
        // modulo.hpp
        {
            //[csv_put_fun
            typedef int T; T x = 1, y = 2, z = 0;
            std::vector<int> cont;
            cont | as2::csv(
                as2::_put % ( as2::_fun = ( lambda::_1 + 1 ) ),
                x, y, z 
            );
            BOOST_ASSIGN_V2_CHECK( cont.front() == ( x + 1 ) );
            BOOST_ASSIGN_V2_CHECK( cont.back() == ( z + 1 ) );
            //]
        }
        {
            //[csv_put_id
            typedef int T; T x = 1, y = 2, z = 0;
            std::list<T> cont; 
            as2::csv( 
            	as2::put( cont ) % ( as2::_fun = as2::_identity ), 
                x, y, z 
            );
            BOOST_ASSIGN_V2_CHECK( cont.front() == x );
            BOOST_ASSIGN_V2_CHECK( cont.back() == z );
            //]
            BOOST_ASSIGN_V2_CHECK( *boost::next( boost::begin( cont ) ) == y );
        }
		// std.hpp
        {
            //[csv_put_push_front
            typedef int T; T x = 1, y = 2, z = 0;
            std::deque<int> cont;
            cont | as2::csv( as2::_put %  as2::_push_front, x, y ,z );
            BOOST_ASSIGN_V2_CHECK( cont.front() == z );
            BOOST_ASSIGN_V2_CHECK( cont.back() == x );
            //]
        }
		// ext.hpp
        {
            //[csv_put_lookup
            std::map<std::string, int> cont;
            cont["jan"] = 29; cont["feb"] = 26; cont["mar"] = 29;
            cont |
                as2::csv( 
                	as2::_put % ( as2::_lookup = ( lambda::_1 +=2 ) )
            		, "jan", "feb", "mar" 
                );
            BOOST_ASSIGN_V2_CHECK( cont["feb"] == 28 );
            //]
            BOOST_ASSIGN_V2_CHECK( cont["jan"] == 31 );
            BOOST_ASSIGN_V2_CHECK( cont["mar"] == 31 );
        }
        {
            //[csv_put_repeat
            typedef int T; std::deque<T> cont; T x = 1, y = 2, z = 0;
            cont | as2::csv( as2::_put % ( as2::_repeat = 2  ), x, y, z );
            BOOST_ASSIGN_V2_CHECK( cont.size() == 6 );
            //]
            BOOST_ASSIGN_V2_CHECK( cont[0] == x );
            BOOST_ASSIGN_V2_CHECK( cont[1] == x );
            BOOST_ASSIGN_V2_CHECK( cont[2] == y );
            BOOST_ASSIGN_V2_CHECK( cont[3] == y );
            BOOST_ASSIGN_V2_CHECK( cont[4] == z );
            BOOST_ASSIGN_V2_CHECK( cont[5] == z );
        }
        {
            //[csv_put_iterate
            typedef int T; T x = 1, y = 2, z = 0;
            boost::array<T, 3> cont; cont[0] = x;
            cont | as2::csv( as2::_put % ( as2::_iterate = 1  ) , y, z );
            BOOST_ASSIGN_V2_CHECK( cont[1] == y );
            //]
            BOOST_ASSIGN_V2_CHECK( cont[0] == x );
            BOOST_ASSIGN_V2_CHECK( cont[2] == z );
        }
        
    }

}// xxx_other
}// xxx_test_assign
