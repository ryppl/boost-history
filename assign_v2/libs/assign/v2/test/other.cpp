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
#include <boost/mpl/assert.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/array.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/next_prior.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/stable_partition.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/detail/check/equal_ref.hpp>
#include <boost/assign/v2/detail/check/equal_val.hpp>
#include <boost/assign/v2/utility/chain.hpp>
#include <boost/assign/v2/utility/chain/alias.hpp>
#include <boost/assign/v2/ref/wrapper/range_get.hpp>
#include <boost/assign/v2/ref/array/csv.hpp>
#include <boost/assign/v2/ref/array/functor.hpp>
#include <boost/assign/v2/ref/array/converter.hpp>
#include <boost/assign/v2/put/container.hpp>
#include <boost/assign/v2/put/deque.hpp>
#include <boost/assign/v2/put/container/functor.hpp>
#include <boost/assign/v2/put/pipe/functor.hpp>
#include <boost/assign/v2/put/pipe/csv.hpp>
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

            //[other_put_pipe_csv
            std::deque<int> cont;
            boost::range::stable_partition(
                /*<< Calls `cont.push_back( t )` for [^t=0,...,5], and returns `cont` >>*/
                cont | as2::_csv_put( 0, 1, 2, 3, 4, 5 ),
                lambda::_1 % 2
            );
            //]
            //[other_ref_csv_array
            BOOST_ASSIGN_V2_CHECK(boost::range::equal(
                cont,
                /*<< The input [^1, 3, ..., 4] is held by reference (not copies) >>*/
                as2::ref::csv_array(1, 3, 5, 0, 2, 4)
            ));
            //]
        }
        // REF + CONVERTER
        {
            //[other_convert_ref_array
            typedef std::pair<std::string, double> T;
            typedef std::vector<T> C;
            C cont = /*<< Name lookup >>*/ converter(
                as2::ref::array( T("pi", 3.14) )( T("e", 2.71) )( T("log2", 0.69) )
            );

            BOOST_ASSIGN_V2_CHECK( cont[1].first == "e" );
            BOOST_ASSIGN_V2_CHECK( cont[1].second == 2.71 );
            //]
            BOOST_ASSIGN_V2_CHECK( cont[0].first == "pi" );
            BOOST_ASSIGN_V2_CHECK( cont[0].second == 3.14 );
            BOOST_ASSIGN_V2_CHECK( cont[2].first == "log2" );
            BOOST_ASSIGN_V2_CHECK( cont[2].second == 0.69 );
        }
        // CHAIN + REF
        {
            //[other_chain_write
            /*<< Needed to bring && into scope >>*/ using namespace boost::assign::v2;
            std::vector<int> r( 3 ); r[0] = 1; r[1] = 2; r[2] = 0;
            /*<< lvalue >>*/boost::array<int, 2> cont; /*<< lvalue >>*/ int z;
            boost::copy(
                r,
                boost::begin(
                    cont && (/*<< rvalue >>*/ as2::ref::csv_array( z ) | as2::ref::_get )
                )
            );

            BOOST_ASSIGN_V2_CHECK( cont[0] == r[0] );
            BOOST_ASSIGN_V2_CHECK( cont[1] == r[1] );
            BOOST_ASSIGN_V2_CHECK( z == r[2] );
            //]
        }
    }

}// xxx_other
}// xxx_test_assign
