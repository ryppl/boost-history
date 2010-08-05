//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/tuple/tuple.hpp>
#include <boost/assign/v2/detail/checking/check_equal.hpp>
#include <boost/assign/v2/detail/functor/constructor.hpp>
#include <libs/assign/v2/test/detail/functor.h>

namespace test_assign_v2{
namespace xxx_detail{
namespace xxx_functor{

    void test()
    {
        using namespace boost::assign::v2;
        
        {
        	typedef boost::tuple<int&, int const&> t_;
        	typedef functor_aux::constructor<t_> f_;
        	int a = 1;
            int b = 2;
        	f_ f;
        	t_ t = f( a, b );
            BOOST_ASSIGN_V2_CHECK_EQUAL( &boost::get<0>( t ), &a );
            BOOST_ASSIGN_V2_CHECK_EQUAL( &boost::get<1>( t ), &b );
        }
    }
    
}// xxx_functor
}// xxx_detail
}// test_assign_v2
