//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <libs/assign/v2/test/put/pipe/csv.h>
#include <libs/assign/v2/test/put/pipe/forward_pars.h>
#include <libs/assign/v2/test/put/pipe/functor.h>
#include <libs/assign/v2/test/put/pipe/stl.h>
#include <libs/assign/v2/test/put/pipe/range.h>
#include <libs/assign/v2/test/put/pipe/ext.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{

    void test(){
    	xxx_csv::test();
    	xxx_forward_pars::test();
    	xxx_functor::test();
    	xxx_stl::test();
    	xxx_range::test();
    	xxx_ext::test();
    }
    
}// xxx_pipe
}// xxx_put
}// xxx_test_assign

