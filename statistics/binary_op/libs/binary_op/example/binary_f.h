///////////////////////////////////////////////////////////////////////////////
//  binary_op::example::functional::binary_f.h                               //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
#ifndef LIBS_BINARY_OP_EXAMPLE_BINARY_F_H_ER_2009
#define LIBS_BINARY_OP_EXAMPLE_BINARY_F_H_ER_2009
#include <ostream>
// This is to be used by examples.
struct binary_f{
    typedef void result_type;
    typedef unsigned val_t;
    binary_f(std::ostream& out);
    binary_f(const binary_f& that);
    
    result_type operator()(val_t a,val_t b);
    std::ostream& out_;
    val_t a_;
    val_t b_;
};

#endif
