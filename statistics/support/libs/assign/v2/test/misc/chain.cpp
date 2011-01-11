//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/assign/v2/chain/checking/twin_values.hpp>
#include <boost/assign/v2/chain/checking/distinct_values.hpp>
#include <libs/assign/v2/test/misc/chain.h>

namespace test_assign_v2{
namespace xxx_misc{
namespace xxx_chain{

    void test()
    {
    	namespace as2 = boost::assign::v2;
        namespace ns = as2::checking::chain;
        ns::twin_values::do_check<int>();
      	ns::distinct_values::do_check<short>();
      	ns::distinct_values::do_check<int>();
      	ns::distinct_values::do_check<float>();
      	ns::distinct_values::do_check<double>();
    }

}// xxx_chain
}// xxx_misc
}// test_assign_v2
