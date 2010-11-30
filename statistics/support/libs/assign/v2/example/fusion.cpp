////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                       //
//                                                                        //
//  Copyright (C) 2003-2004 Thorsten Ottosen                              //
//  Copyright (C) 2010 Erwann Rogard                                      //
//  Use, modification and distribution are subject to the                 //
//  Boost Software License, Version 1.0. (See accompanying file           //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)      //
////////////////////////////////////////////////////////////////////////////
#include <boost/assign/v2/ref/static_array/alloc/lazy_alloc.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/assign/v2/detail/checking/check_equal.hpp>
#include <boost/assign/v2/ref/fusion.hpp>

#include <libs/assign/v2/example/fusion.h>

namespace example_assign_v2{
namespace xxx_fusion{

    void run(std::ostream& os)
    {
    	using namespace boost::assign::v2;
        typedef int t0_;
        typedef int const t1_; 
    	t0_ a = 1;
        typedef ref::assign_tag::copy tag1_;
        typedef ref::alloc_tag::lazy_alloc tag2_;

        typedef ref::nth_result_of::fusion<
        	tag1_, tag2_, t0_ // NOT nil_arg_type
        > meta_result_;
        typedef boost::mpl::apply1<
        	meta_result_,
            boost::mpl::vector<t0_,t1_>
        >::type result_;
        result_ result = ref::fusion<tag1_, tag2_, t0_>( _nil )( a )( 2 );
        BOOST_ASSIGN_V2_CHECK_EQUAL( result[ 0 ] , a );
        BOOST_ASSIGN_V2_CHECK_EQUAL( result[ 1 ] , 2 );
    }
    
}// xxx_fusion
}// example_assign_v2
