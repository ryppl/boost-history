//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/assign/v2/utility/chain/check.hpp>
#include <libs/assign/v2/test/utility/chain.h>

namespace test_assign_v2{
namespace xxx_utility{
namespace xxx_chain{

    void test()
    {
    	namespace as2 = boost::assign::v2;
        namespace ns = as2::checking::chain;
        ns::do_check<int>();
        ns::do_check<double>();
    }

}// xxx_chain
}// xxx_utility
}// test_assign_v2
