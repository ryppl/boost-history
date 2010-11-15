//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/mpl/int.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/vector/vector0.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/array.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/assign/v2/detail/checking/check_equal.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>
#include <boost/assign/v2/ref/fusion/fusion.hpp>
#include <libs/assign/v2/test/ref/fusion.h>

namespace test_assign_v2{
namespace xxx_ref{
namespace xxx_fusion{

    void test()
    { 
    	using namespace boost::assign::v2;
    	using namespace ref;
        typedef assign_tag::copy tag1_;
        typedef boost::use_default tag2_;
        typedef boost::mpl::int_<0> i0_;
        typedef boost::mpl::int_<1> i1_;
        typedef boost::mpl::int_<2> i2_;
        {
        	typedef int t0_;
        	typedef int const t1_;
        	typedef const char* t2_;
			t0_ a = 0;
        	t1_ b = 1;
            t2_ c = "c";

        	typedef nth_result_of::fusion<tag1_, tag2_> result_;
        	typedef boost::mpl::apply1<result_,
            	boost::mpl::vector<t0_,t1_,t2_>
            >::type ar_;
        	ar_ ar = fusion<tag1_, tag2_>( _nil )( a )( b )( c );
            ar.static_lookup( i0_() );
        	BOOST_ASSIGN_V2_CHECK_EQUAL( &ar.static_lookup( i0_() ), &a );
        	BOOST_ASSIGN_V2_CHECK_EQUAL( &ar.static_lookup( i1_() ), &b );
        	BOOST_ASSIGN_V2_CHECK_EQUAL( &ar.static_lookup( i2_() ), &c );
			{
        		typedef boost::mpl::apply1<result_,
                	boost::mpl::vector<t0_,t1_>
                >::type ar2_;
            	ar2_ ar2 = ar.pop();
                BOOST_ASSIGN_V2_CHECK_EQUAL( &ar2.back(), &b );
        		typedef boost::mpl::apply1<result_,
                	boost::mpl::vector<t0_>
                >::type ar1_;
            	ar1_ ar1 = ar2.pop();
                BOOST_ASSIGN_V2_CHECK_EQUAL( &ar1.back(), &a );
        		typedef boost::mpl::apply1<
                	result_,
                    boost::mpl::vector0<>
                >::type ar0_;
            	ar0_ ar0 = ar1.pop();
            }
        }
        {
        	typedef int t_;
			t_ a = 0;
        	t_ b = 1;
            t_ c = 2;
			typedef wrapper<tag1_,t_> w_;
            typedef boost::array<w_,3> arw_;    
        	typedef nth_result_of::fusion<tag1_, tag2_> result_;
        	typedef boost::mpl::apply1<result_,
            	boost::mpl::vector<t_,t_,t_>
            >::type ar_;
        	ar_ ar = fusion<tag1_, tag2_>( _nil )( a )( b )( c );
            arw_ arw;
            assign_array( arw, ar );
        	BOOST_ASSIGN_V2_CHECK_EQUAL( &arw[ 0 ].unwrap() , &a );
        	BOOST_ASSIGN_V2_CHECK_EQUAL( &arw[ 1 ].unwrap() , &b );
        	BOOST_ASSIGN_V2_CHECK_EQUAL( &arw[ 2 ].unwrap() , &c );
        }
        {
        	typedef int t_;
			t_ a = 0;
        	t_ b = 1;
            t_ c = 2;
            typedef boost::tuple<t_ const&> tuple_;
        	typedef nth_result_of::fusion<tag1_,tag2_> result_;
        	typedef boost::mpl::apply1<result_,
            	boost::mpl::vector<t_,t_,t_>
            >::type ar_;
        	fusion<tag1_, tag2_>( _nil )
            	( tuple_( a ) )( tuple_( b ) )( tuple_( c ) );
        }
    }

}// xxx_fusion
}// xxx_ref
}// test_assign

