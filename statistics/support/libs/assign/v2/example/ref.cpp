//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <libs/assign/v2/example/ref/array.h>
#include <libs/assign/v2/example/ref.h>

namespace example_assign_v2{
namespace xxx_ref{

    void run(std::ostream& os){
    	os << "** xxx_ref" << std::endl;
    	xxx_array::run( os );
    }

}// xxx_ref
}// example_assign_v2
