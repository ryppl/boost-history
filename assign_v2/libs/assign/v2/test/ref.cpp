//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <libs/assign/v2/test/ref/array.h>
#include <libs/assign/v2/test/ref/csv_array.h>
#include <libs/assign/v2/test/ref/get.h>
#include <libs/assign/v2/test/ref/aux_.h>
#include <libs/assign/v2/test/ref/wrapper.h>
#include <libs/assign/v2/test/ref.h>

namespace test_assign_v2{
namespace xxx_ref{

    void test(){
        xxx_array::test();
        xxx_csv_array::test();
        xxx_aux_::test();
        xxx_get::test();
        xxx_wrapper::test();
    }

}// xxx_ref
}// test_assign_v2
