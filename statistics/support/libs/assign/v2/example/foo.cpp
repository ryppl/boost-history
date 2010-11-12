//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <libs/assign/v2/example/include.h>
#include <libs/assign/v2/example/foo.h>

namespace example_assign_v2{

std::ostream& operator<<( std::ostream& os, foo const& t)
{
    os << '(';
    if(t.ptr_a){
        os << *t.ptr_a;
    }else{
        os << "NULL";
    }
    return os << ',' << t.b << ')';
}

}// example_assign_v2

