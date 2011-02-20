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
#include <vector>
#include <boost/lambda/lambda.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/stable_partition.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/utility/chain.hpp>
#include <boost/assign/v2/ref/wrapper/adaptor_get.hpp>
#include <boost/assign/v2/ref/array/csv.hpp>
#include <boost/assign/v2/ref/array/functor.hpp>
#include <boost/assign/v2/ref/array/functor/converter.hpp>
#include <boost/assign/v2/put/deque/csv.hpp>
#include <boost/assign/v2/put/pipe/csv.hpp>
#include <libs/assign/v2/test/other.h>

namespace test_assign_v2{
namespace xxx_other{

    // Tests interaction between different functionalities

    void test(){

        namespace as2 = boost::assign::v2;
		namespace bl = boost::lambda;

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
        {
        	// suggested by JB:
        	typedef int T; std::vector<T> cont;
   			boost::range::stable_partition(
       			cont | as2::_csv_put(0, 1, 2, 3, 4, 5), 
                bl::_1 % 2
            );
   			BOOST_ASSIGN_V2_CHECK(boost::range::equal(
       			cont, 
                as2::ref::csv_array(1, 3, 5, 0, 2, 4)
            ));
        
        }
    }

}// xxx_other
}// xxx_test_assign
