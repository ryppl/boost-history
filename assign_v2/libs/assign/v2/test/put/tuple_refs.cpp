//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <deque>
#include <boost/tuple/tuple.hpp>
#include <boost/assign/v2/detail/checking/check.hpp>
#include <boost/assign/v2/detail/checking/constants.hpp>
#include <boost/assign/v2/put/sub.hpp>
#include <libs/assign/v2/test/put/tuple_refs.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_tuple_refs{

    void test()
    {
        namespace as2 = boost::assign::v2;
        using namespace as2::checking::constants;
        {
        	int a1, b1, c1, d1;//, e1, f1, g1, h1;
            {
                a1 = a; b1 = b; c1 = c;
                d1 = d;
            }
            {	// Deque + tuple
            	typedef boost::tuple<int&, const int&> tuple_;
                typedef std::deque<tuple_> cont_;
                cont_ cont;
                as2::put( cont )( a1 , b1 )( c1 , d1 );
                BOOST_ASSIGN_V2_CHECK( &a1 == &boost::get<0>( cont[0] ) );
                BOOST_ASSIGN_V2_CHECK( &b1 == &boost::get<1>( cont[0] ) );
                BOOST_ASSIGN_V2_CHECK( &c1 == &boost::get<0>( cont[1] ) );
                BOOST_ASSIGN_V2_CHECK( &d1 == &boost::get<1>( cont[1] ) );
            }
        }
    }

}// xxx_tuple_refs
}// xxx_put
}// test_assign_v2

