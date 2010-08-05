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
#include <boost/mpl/empty_base.hpp>
#include <boost/assign/v2/detail/checking/check_equal.hpp>
#include <boost/assign/v2/ref/detail/static_list/static_list.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>
#include <libs/assign/v2/test/ref/static_list.h>

namespace test_assign_v2{
namespace xxx_ref{
namespace xxx_static_list{

    void test()
    {
    	using namespace boost::assign::v2::ref;
        {
        	typedef assign_tag::copy tag1_;
        	typedef boost::mpl::empty_base tag2_;
        	typedef nth_result_of::static_list<2,
                tag1_, 
                tag2_,
                int
            >::type list2_;
        	int a = 0, b = 1;
            list2_ list2 = static_list<tag1_,tag2_>( a )( b );
        	typedef boost::mpl::int_<0> i0_;
        	typedef boost::mpl::int_<1> i1_;
        	BOOST_ASSIGN_V2_CHECK_EQUAL( &list2.static_lookup( i0_() ) , &a );
        	BOOST_ASSIGN_V2_CHECK_EQUAL( &list2.static_lookup( i1_() ) , &b );
		}
    }

}// xxx_static_list
}// xxx_ref
}// test_assign
