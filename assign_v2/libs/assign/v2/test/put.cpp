//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <libs/assign/v2/test/put/pipe.h>
#include <libs/assign/v2/test/put/ptr.h>
#include <libs/assign/v2/test/put/put.h>
#include <libs/assign/v2/test/put.h>

namespace test_assign_v2{
namespace xxx_put{

    void test(){

        xxx_ptr::test();
        xxx_put::test();
        xxx_pipe::test();

    }

}// xxx_put
}// test_assign_v2
