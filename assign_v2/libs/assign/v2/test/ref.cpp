//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <libs/assign/v2/test/ref/array.h>
#include <libs/assign/v2/test/ref/convert_traits.h>
#include <libs/assign/v2/test/ref/list.h>
#include <libs/assign/v2/test/ref/list_tuple.h>
#include <libs/assign/v2/test/ref/wrapper.h>
#include <libs/assign/v2/test/ref.h>

namespace test_assign_v2{
namespace xxx_ref{

    void test(){

        xxx_array::test();
        xxx_convert_traits::test();
        xxx_array::test();
        xxx_list::test();
        xxx_list_tuple::test();
        xxx_wrapper::test();

    }

}// xxx_ref
}// test_assign_v2
