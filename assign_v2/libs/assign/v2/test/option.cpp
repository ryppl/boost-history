//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <libs/assign/v2/test/option/data.h>
#include <libs/assign/v2/test/option/iterate.h>
#include <libs/assign/v2/test/option/mapped.h>
#include <libs/assign/v2/test/option/repeat.h>
#include <libs/assign/v2/test/option/standard.h>
#include <libs/assign/v2/test/option.h>

namespace test_assign_v2{
namespace xxx_option{

    void test(){
        xxx_fun::test();
        xxx_iterate::test();
        xxx_mapped::test();
        xxx_repeat::test();
        xxx_standard::test();
    }

}// xxx_option
}// xxx_test_assign

