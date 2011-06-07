//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <libs/assign/v2/test/put/csv_put_basic.h>
#include <libs/assign/v2/test/put/csv_put_ext.h>
#include <libs/assign/v2/test/put/delay_csv_put.h>
#include <libs/assign/v2/test/put/delay_put.h>
#include <libs/assign/v2/test/put/put.h>
#include <libs/assign/v2/test/put.h>

namespace test_assign_v2{
namespace xxx_put{

    void test(){

        xxx_csv_put_basic::test();
        xxx_csv_put_ext::test();
        xxx_delay_csv_put::test();
        xxx_delay_put::test();
        xxx_put::test();

    }

}// xxx_put
}// test_assign_v2
