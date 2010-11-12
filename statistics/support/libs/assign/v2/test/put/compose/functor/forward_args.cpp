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
#include <vector>

#include <boost/mpl/int.hpp>

#include <boost/typeof/typeof.hpp>
#include <boost/tuple/tuple.hpp>

#include <boost/assign/v2/detail/checking/check_equal.hpp>
#include <boost/assign/v2/detail/checking/constants.hpp>
#include <boost/assign/v2/ref/fusion/assign_copy.hpp>
#include <boost/assign/v2/ref/fusion_matrix/container.hpp>
#include <boost/assign/v2/ref/fusion_matrix/nth_result_of.hpp>
#include <boost/assign/v2/put/compose/functor/forward_args.hpp>

#include <libs/assign/v2/test/put/compose/functor/forward_args.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_compose{
namespace xxx_functor{
namespace xxx_forward_args{

    void test()
    {

        using namespace boost::assign::v2;
        int a1, b1, c1, d1;//, e1, f1, g1, h1;
        {
            using namespace checking::constants;
            a1 = a; b1 = b; c1 = c; d1 = d;
        }
        typedef boost::mpl::int_<0> zero_;
        typedef boost::mpl::int_<1> one_;
        typedef boost::mpl::int_<2> two_;
        typedef boost::mpl::int_<3> three_;
        typedef boost::mpl::int_<4> four_;
        typedef ref::fusion_matrix_aux::empty<>::type empty_;
        {
        	typedef std::vector<int> cont_;
        	cont_ cont;
	        BOOST_AUTO( tmp, ( put( cont ) ) );
            empty_ e;
            BOOST_AUTO( mat, e( a1 )( b1 )( c1 )( d1 )() );
	        forward_args(
    	        tmp,
    	        put_compose_aux::make_args( mat.static_row( zero_() ) )
    	    );
	        forward_args(
    	        tmp,
    	        put_compose_aux::make_args( mat.static_row( one_() ) )
    	    );
	        forward_args(
    	        tmp,
    	        put_compose_aux::make_args( mat.static_row( two_() ) )
    	    );
	        forward_args(
    	        tmp,
    	        put_compose_aux::make_args( mat.static_row( three_() ) )
    	    );
	        forward_args(
    	        tmp,
    	        put_compose_aux::make_args( mat.static_row( four_() ) )
    	    );
        	BOOST_ASSIGN_V2_CHECK_EQUAL( cont[0], a1 );
        	BOOST_ASSIGN_V2_CHECK_EQUAL( cont[1], b1 );
        	BOOST_ASSIGN_V2_CHECK_EQUAL( cont[2], c1 );
        	BOOST_ASSIGN_V2_CHECK_EQUAL( cont[3], d1 );
        	BOOST_ASSIGN_V2_CHECK_EQUAL( cont.size(), 5 );
			
		}
    }
    
}// xxx_forward_args
}// xxx_functor
}// xxx_compose
}// xxx_put
}// xxx_test_assign
