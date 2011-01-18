//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/assign/v2/ref/convert_traits/checking.hpp>
#include <libs/assign/v2/test/ref/convert_traits.h>

namespace test_assign_v2{
namespace xxx_ref{
namespace xxx_convert_traits{

    void test()
    { 
    	namespace as2 = boost::assign::v2;
    	namespace ns = as2::ref::convert_traits::checking;
        {
        	ns::same_value_type::do_check<int>();
        }
// TODO, in the future
/*
        {
            namespace dv = ns::distinct_values;
            namespace ari = dv::arithmetic;
            ari::do_check<short>();
            ari::do_check<int>();
        	ari::do_check<float>();
            ari::do_check<double>();
        }
*/
    }

}// convert_traits
}// ref
}// test_assign
