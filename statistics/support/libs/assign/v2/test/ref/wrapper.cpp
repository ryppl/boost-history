//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/assign/v2/ref/wrapper/wrapper.hpp>
#include <boost/assign/v2/detail/checking/check_equal.hpp>
#include <libs/assign/v2/test/ref/wrapper.h>

namespace test_assign_v2{
namespace xxx_ref{
namespace xxx_wrapper{

    void test()
    { 
    	using namespace boost::assign::v2;
        typedef int val_;
		{
			typedef ref::type_traits::copy_wrapper<val_>::type w_;
            w_ w;
            val_ a = 1, b = 2, c = 3;
            w.rebind( a ); BOOST_ASSIGN_V2_CHECK_EQUAL( &w.unwrap(), &a );
            w.assign( b ); BOOST_ASSIGN_V2_CHECK_EQUAL( a, b );
            w = c; BOOST_ASSIGN_V2_CHECK_EQUAL( a, c );
        }
		{
			typedef ref::type_traits::copy_wrapper<val_ const>::type w_;
            w_ w;
            val_ a = 1;
            w.rebind( a ); BOOST_ASSIGN_V2_CHECK_EQUAL( &w.unwrap(), &a );
        }
		{
			typedef ref::type_traits::rebind_wrapper<val_>::type w_;
            w_ w;
            val_ a = 1;
            w = a; BOOST_ASSIGN_V2_CHECK_EQUAL( &w.unwrap(), &a );
        }
		{
			typedef ref::type_traits::rebind_wrapper<val_ const>::type w_;
            w_ w;
            val_ const a = 1;
            w = a; BOOST_ASSIGN_V2_CHECK_EQUAL( &w.unwrap(), &a );
        }
    }

}// xxx_wrapper
}// xxx_ref
}// test_assign_v2
