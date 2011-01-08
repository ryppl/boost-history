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
#include <libs/assign/v2/test/chain.h>

namespace test_assign_v2{
namespace xxx_chain{

    void test()
    {
        using namespace boost::assign::v2::checking::chain;
        twin_values::do_check<int>();
      	distinct_values::do_check<short>();
      	distinct_values::do_check<int>();
      	distinct_values::do_check<float>();
      	distinct_values::do_check<double>();
    }

}// xxx_chain
}// test_assign_v2
