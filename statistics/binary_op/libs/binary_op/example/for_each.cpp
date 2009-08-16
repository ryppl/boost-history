///////////////////////////////////////////////////////////////////////////////
//  binary_op::example::for_each.cpp                                         //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
#include <ostream>
#include <vector>
#include <functional>
#include <boost/assert.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/range.hpp>
#include <boost/bind.hpp>
#include <boost/binary_op/algorithm/for_each.hpp>
#include <libs/binary_op/example//binary_f.h>
#include <libs/binary_op/example/for_each.h>

void example_for_each(std::ostream& out){
    out << "-> example_binary_for_each :";

    typedef unsigned val_;
    
    using namespace boost;
    using namespace binary_op;
    
    typedef std::vector<val_> vec_t;
    
    vec_t vec_0;
    vec_t vec_1;
    {
        using namespace boost::assign;
        vec_0 += 0, 1, 2, 3, 4, 5, 6, 7;
        vec_1 += 7, 6, 5, 4, 3, 2, 1, 0;
    }

    
    out << "by value : ";
    binary_f f_0 = for_each(
        begin(vec_0),
        end(vec_0),
        begin(vec_1),
        binary_f(out)
        //  ret<??>(out << '(' << lambda::_1 << ',' << lambda::_2 << ')' )
    );//(0,7)(1,6)(2,5)(3,4)(4,3)(5,2)(6,1)(7,0)
    out << ' ';

    BOOST_ASSERT(vec_0.back() == f_0.a_);    
    BOOST_ASSERT(vec_1.back() == f_0.b_);    

    out << "<-" << std::endl;
}

