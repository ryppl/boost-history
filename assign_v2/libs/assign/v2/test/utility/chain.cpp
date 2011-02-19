//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <iterator>
#include <vector>
#include <list>
#include <boost/next_prior.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/assign/v2/utility/chain/check.hpp>
#include <boost/assign/v2/utility/chain.hpp>
#include <libs/assign/v2/test/utility/chain.h>

namespace test_assign_v2{
namespace xxx_utility{
namespace xxx_chain{


    void test()
    {
        namespace as2 = boost::assign::v2;
        {
            namespace ns = as2::check_chain_aux;
            {    typedef int T; ns::static_<T>(); ns::static_<T>(); }
            {    typedef double T; ns::static_<T>(); ns::static_<T>(); }
        }
        {
            //[chain_r
            typedef int T;
            boost::array<T, 2> ar;  ar[0] = 0; ar[1] = 1;
            std::list<T> list; list.push_back( 2 ); list.push_back( 3 );
            std::vector<T> v( boost::begin( ar ), boost::end( ar ) );
            boost::copy( list, std::back_inserter( v ) );
            BOOST_ASSIGN_V2_CHECK(
                boost::range::equal( v, ar | as2::_chain( list ) )
            );
            //]
        }
        {
            //[chain_w
            typedef int T; std::vector<T> v( 4 );
            v[0] = 1; v[1] = 2; v[2] = 0; v[3] = 5;
            boost::array<T, 2> ar; std::list<T> list( v.size() - ar.size() );
            boost::copy( v, boost::begin( ar | as2::_chain( list ) ) );
            BOOST_ASSIGN_V2_CHECK( ar[0] == v[0] );
            BOOST_ASSIGN_V2_CHECK( ar[1] == v[1] );
            BOOST_ASSIGN_V2_CHECK( list.front() == v[2] );
            BOOST_ASSIGN_V2_CHECK( list.back() == v[3] );
            //]
        }

    }// test

}// xxx_chain
}// xxx_utility
}// test_assign_v2
