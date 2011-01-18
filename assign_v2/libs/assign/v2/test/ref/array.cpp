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
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>
#include <boost/mpl/int.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/assign/v2/detail/checking/check.hpp>
#include <boost/assign/v2/detail/checking/constants.hpp>
#include <boost/assign/v2/detail/checking/container.hpp>
#include <boost/assign/v2/ref/array/functor.hpp>
#include <boost/assign/v2/ref/wrapper.hpp>
#include <libs/assign/v2/test/ref/array.h>

namespace test_assign_v2{
namespace xxx_ref{
namespace xxx_array{

    void test()
    {
    	namespace as2 = boost::assign::v2;
        {
            // Array
            typedef as2::ref::nth_result_of::array<8, int const>::type ar_;
            using namespace as2::checking::constants;
            ar_ ar = as2::ref::array( a )( b )( c )( d )( e )( f )( g )( h );
            {

                typedef as2::container_tag::array tag_;
                namespace ns = as2::checking::container;
                as2::checking::do_check(tag_(), ar );
            }
		}
		{
        	// Assignement of references
			int a1, b1, c1, d1, e1, f1, g1, h1;
            typedef std::vector<int> vec_;
            vec_ vec;
            {
            	using namespace as2::checking::constants;
            	vec.push_back( a );
            	vec.push_back( b );
            	vec.push_back( c );
            	vec.push_back( d );
            	vec.push_back( e );
            	vec.push_back( f );
            	vec.push_back( g );
            	vec.push_back( h );
            }
            boost::copy(
            	vec,
            	boost::begin(  
                    as2::ref::array( a1 )( b1 )( c1 )( d1 )
                    	( e1 )( f1 )( g1 )( h1 ) | as2::ref::_get
                )
            );
            {
                namespace ns = as2::checking::container;
                ns::do_check( as2::ref::array<int const>
            		( a1 )( b1 )( c1 )( d1 )( e1 )( f1 )( g1 )( h1 )
                );
            }
		}
    }

}// xxx_array
}// xxx_ref
}// test_assign
