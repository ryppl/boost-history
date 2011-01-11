//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <libs/assign/v2/example/misc/chain.h>
#include <libs/assign/v2/example/misc/convert.h>
#include <libs/assign/v2/example/misc.h>

namespace example_assign_v2{
namespace xxx_misc{

    void run(std::ostream& os)
    {
    	xxx_chain::run( os );
    	xxx_convert::run( os );
    }

}// xxx_misc
}// example_assign_v2

