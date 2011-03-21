//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <libs/assign/v2/test/deque.h>
#include <libs/assign/v2/test/modifier.h>
#include <libs/assign/v2/test/fun.h>
#include <libs/assign/v2/test/value/pipe.h>
#include <libs/assign/v2/test/value/ptr.h>
#include <libs/assign/v2/test/value/put.h>
#include <libs/assign/v2/test/value.h>

namespace test_assign_v2{
namespace xxx_value{

    void test()
    {
        xxx_deque::test();
        xxx_fun::test();
        xxx_modifier::test();
        xxx_pipe::test();
        xxx_ptr::test();
        xxx_put::test();
    }

}// xxx_value
}// test_assign_v2

