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

#include <boost/typeof/typeof.hpp>
#include <boost/tuple/tuple.hpp>

#include <boost/assign/v2/detail/checking/check_equal.hpp>
#include <boost/assign/v2/detail/checking/constants.hpp>
#include <boost/assign/v2/ref/fusion/assign_copy.hpp>
#include <boost/assign/v2/put/compose/functor/composer.hpp>
#include <boost/assign/v2/put/compose/functor/forward_seq_args.hpp>

#include <libs/assign/v2/test/put/compose/functor/forward_seq_args.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_compose{
namespace xxx_functor{
namespace xxx_forward_seq_args{

    void test()
    {

        using namespace boost::assign::v2;
        int a1, b1, c1, d1;//, e1, f1, g1, h1;
        using namespace checking::constants;
        {
            a1 = a; b1 = b; c1 = c; d1 = d;
        }
        using namespace adaptor;
        {
        	typedef boost::tuple<int&, int&> tuple_;
            typedef std::deque<tuple_> cont_;
        	cont_ cont;
	        forward_seq_args(
    	        put( cont ),
                _put( a1, b1 )( c1, d1 )
    	    );
        	BOOST_ASSIGN_V2_CHECK_EQUAL( &boost::get<0>( cont[0] ), &a1 );
        	BOOST_ASSIGN_V2_CHECK_EQUAL(  boost::get<0>( cont[0] ), a 	);
        	BOOST_ASSIGN_V2_CHECK_EQUAL( &boost::get<1>( cont[0] ), &b1 );
        	BOOST_ASSIGN_V2_CHECK_EQUAL(  boost::get<1>( cont[0] ), b 	);
        	BOOST_ASSIGN_V2_CHECK_EQUAL( &boost::get<0>( cont[1] ), &c1 );
        	BOOST_ASSIGN_V2_CHECK_EQUAL(  boost::get<0>( cont[1] ), c 	);
        	BOOST_ASSIGN_V2_CHECK_EQUAL( &boost::get<1>( cont[1] ), &d1 );
        	BOOST_ASSIGN_V2_CHECK_EQUAL(  boost::get<1>( cont[1] ), d 	);
        }
    }

}// xxx_forward_seq_args
}// xxx_functor
}// xxx_compose
}// xxx_put
}// xxx_test_assign
