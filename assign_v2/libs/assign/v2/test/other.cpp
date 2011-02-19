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
#include <boost/range/algorithm/copy.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/utility/chain.hpp>
#include <boost/assign/v2/ref/wrapper/adaptor_get.hpp>
#include <boost/assign/v2/ref/array/csv.hpp>
#include <boost/assign/v2/ref/array/functor.hpp>
#include <boost/assign/v2/ref/array/functor/converter.hpp>
#include <boost/assign/v2/put/deque/csv.hpp>
#include <libs/assign/v2/test/other.h>

namespace test_assign_v2{
namespace xxx_other{

    // Tests interaction between different functionalities

    void test(){

        namespace as2 = boost::assign::v2;

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
            std::vector<int> v( 3 ); v[0] = -1; v[1] = 0; v[2] = 1;
            boost::array<int, 2> ar; int z; // lvalues
            boost::copy(
                v,
                boost::begin(
                    ar | as2::_chain(
                        as2::ref::csv_array( z ) // rvalue!
                            | as2::ref::_get
                    )
                )
            );
            BOOST_ASSIGN_V2_CHECK( ar[0] == -1 );
            BOOST_ASSIGN_V2_CHECK( ar[1] == 0 );
            BOOST_ASSIGN_V2_CHECK( z == 1 );
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
    }

}// xxx_other
}// xxx_test_assign
