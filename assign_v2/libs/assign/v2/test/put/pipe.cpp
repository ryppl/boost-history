//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <libs/assign/v2/test/put/pipe/fun.h>
#include <libs/assign/v2/test/put/pipe/args_list.h>
#include <libs/assign/v2/test/put/pipe/modifier.h>
#include <libs/assign/v2/test/put/pipe/operator.h>
#include <libs/assign/v2/test/put/pipe/range.h>
#include <libs/assign/v2/test/put/pipe.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{

    void test()
    {
        xxx_args_list::test();
        xxx_modifier::test();
        xxx_fun::test();
        xxx_operator::test();
        xxx_range::test();
    }

}// xxx_pipe
}// xxx_put
}// xxx_test_assign

