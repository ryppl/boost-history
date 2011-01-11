//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <libs/assign/v2/example/put/deque.h>
#include <libs/assign/v2/example/put/convert.h>
#include <libs/assign/v2/example/put/range.h>
#include <libs/assign/v2/example/put/pipe.h>
#include <libs/assign/v2/example/put.h>

namespace example_assign_v2{
namespace xxx_put{

    void run(std::ostream& os){
    	xxx_deque::run( os );
    	xxx_range::run( os );
    	xxx_pipe::run( os );
    }

}// xxx_put
}// example_assign_v2
