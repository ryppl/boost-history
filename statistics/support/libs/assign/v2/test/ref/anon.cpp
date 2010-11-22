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
#include <boost/assign/v2/detail/checking/check_equal.hpp>
#include <boost/assign/v2/detail/checking/constants.hpp>
#include <boost/assign/v2/detail/checking/container.hpp>
#include <boost/assign/v2/detail/checking/check_convert.hpp> // TODO
#include <boost/assign/v2/detail/checking/array.hpp>
#include <boost/assign/v2/detail/checking/relational_op.hpp>
#include <boost/assign/v2/ref/anon/functor.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>
#include <libs/assign/v2/test/ref/anon.h>

namespace test_assign_v2{
namespace xxx_ref{
namespace xxx_anon{

    void test()
    {
    	using namespace boost::assign::v2;
        typedef ref::assign_tag::copy copy_;
        {
            // Array
            typedef ref::nth_result_of::anon<
            	8,
                copy_,
                int const
            >::type ar_;
            using namespace checking::constants;
            ar_ ar = ref::anon<copy_>
            	( a )( b )( c )( d )( e )( f )( g )( h );
            {
                using namespace checking;
                typedef container_tag::static_array tag_;
                do_check(tag_(), ar );
            }
		}
		{
        	// Assignement of references
			int a1, b1, c1, d1, e1, f1, g1, h1;
            typedef std::vector<int> vec_;
            vec_ vec;
            {
            	using namespace checking::constants;
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
            	boost::begin( ref::assign_copy::anon
            		( a1 )( b1 )( c1 )( d1 )( e1 )( f1 )( g1 )( h1 )
                )
            );
            {
                using namespace checking::container;
                do_check( ref::assign_copy::anon<int const>
            		( a1 )( b1 )( c1 )( d1 )( e1 )( f1 )( g1 )( h1 )
                );
            }
		}
		{
        	// Rebind
            typedef int const& cref_;
            using namespace checking::constants;
			cref_ 	a1 = a, b1 = b, c1 = c, d1 = d, e1 = e, f1 = f, g1 = g,
            		h1 = h;
            BOOST_AUTO(ar, ref::assign_rebind::anon
            	( a1 )( b1 )( c1 )( d1 )( e1 )( f1 )( g1 )( h1 )
            );
            {
                using namespace checking::container;
                do_check( ar );
            }
            ar.assign( b );
            BOOST_ASSIGN_V2_CHECK_EQUAL( &ar[0] , &b );
            BOOST_ASSIGN_V2_CHECK_EQUAL( &ar[1] , &b );
            BOOST_ASSIGN_V2_CHECK_EQUAL( &ar[2] , &b );
            BOOST_ASSIGN_V2_CHECK_EQUAL( &ar[3] , &b );
            BOOST_ASSIGN_V2_CHECK_EQUAL( &ar[4] , &b );
            BOOST_ASSIGN_V2_CHECK_EQUAL( &ar[5] , &b );
            BOOST_ASSIGN_V2_CHECK_EQUAL( &ar[6] , &b );
            BOOST_ASSIGN_V2_CHECK_EQUAL( &ar[7] , &b );
		}
        {
        	// Conversion
			typedef ref::assign_copy::nth_result_of::anon<
            	8,int const>::type anon_;

            anon_ from;
            {
            	using namespace checking::constants;
            	from = ref::assign_copy::anon
                	( a )( b )( c )( d )( e )( f )( g )( h );
            }
        	using namespace checking::container;
        	{
                typedef boost::array<int,8> to_;
                BOOST_ASSIGN_V2_check_convert
        	}
        	{
                typedef std::deque<int> to_;
                BOOST_ASSIGN_V2_check_convert
        	}
        	{
                typedef std::list<int> to_;
                BOOST_ASSIGN_V2_check_convert
        	}
        	{
                typedef std::queue<int> to_;
                BOOST_ASSIGN_V2_check_convert
        	}
        	{
                typedef std::set<int> to_;
                BOOST_ASSIGN_V2_check_convert
        	}
        	{
        		typedef std::stack<int> to_;
                BOOST_ASSIGN_V2_check_convert
        	}
	        {
    	    	typedef std::vector<int> to_;
                BOOST_ASSIGN_V2_check_convert
        	}

        }
        { 	// Relational
            using namespace checking::constants;
            using namespace checking::relational_op;
            do_check(
            	ref::assign_copy::anon
                	( a )( b )( c )( d )( e )( f )( g )( h )
            );
        }
    }

}// xxx_anon
}// xxx_ref
}// test_assign
