//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/mpl/vector/vector0.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/mpl/vector.hpp>

#include <boost/typeof/typeof.hpp>
#include <boost/assign/v2/detail/checking/check_equal.hpp>
#include <boost/assign/v2/ref/fusion_matrix/container.hpp>
#include <boost/assign/v2/ref/fusion_matrix/nth_result_of.hpp>

#include <libs/assign/v2/test/ref/fusion_matrix.h>

namespace test_assign_v2{
namespace xxx_ref{
namespace xxx_fusion_matrix{

    void test()
    {
    	using namespace boost::assign::v2;
        typedef ref::fusion_matrix_aux::empty<>::type empty_;
        typedef boost::mpl::vector0<> v0_;
        typedef boost::mpl::vector1<int const> v1_;
        typedef boost::mpl::vector2<int const, int const> v2_;
        typedef ref::nth_result_of::fusion_matrix<> meta_result_;
		typedef boost::mpl::apply1<
        	meta_result_,
            boost::mpl::vector<
            	v2_,
                v1_,
                v0_
            >
        >::type result_;

        empty_ e;
        result_ result = e( 1, 2 )( 1 )();
        
        typedef boost::mpl::int_<0> int0_;
        typedef boost::mpl::int_<1> int1_;
        typedef boost::mpl::int_<2> int2_;
        {
        	BOOST_AUTO(tmp, result.static_row( int0_() ) );
            BOOST_ASSIGN_V2_CHECK_EQUAL( tmp.static_column( int0_() ), 1);
            BOOST_ASSIGN_V2_CHECK_EQUAL( tmp.static_column( int1_() ), 2);
        }
        {
        	BOOST_AUTO(tmp, result.static_row( int1_() ) );
            BOOST_ASSIGN_V2_CHECK_EQUAL( tmp.static_column( int0_() ), 1);
        }
        {
        	BOOST_AUTO(tmp, result.static_row( int2_() ) );
        }
        
    }

}// xxx_fusion_matrix
}// xxx_ref
}// test_assign
