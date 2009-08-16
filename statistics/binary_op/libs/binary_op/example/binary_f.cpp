///////////////////////////////////////////////////////////////////////////////
// binary_op::example::functional::binary_f.cpp                              //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
#include <ostream>
#include <libs/binary_op/example//binary_f.h>

// Not an example, but to be used by examples.
binary_f::binary_f(std::ostream& out):out_(out),a_(0),b_(0){}
binary_f::binary_f(const binary_f& that)
:out_(that.out_),a_(that.a_),b_(that.b_){}
    
binary_f::result_type binary_f::operator()(val_t a,val_t b){
    out_ << '(' << a << ',' << b << ')';
    a_ = a;
    b_ = b;
}
