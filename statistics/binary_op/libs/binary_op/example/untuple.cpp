///////////////////////////////////////////////////////////////////////////////
// binary_op::example::functional::untupler.cpp                               //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
#include <boost/type_traits/is_same.hpp>
#include <boost/static_assert.hpp>
#include <boost/assert.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/binary_op/functional/untupler.hpp>
#include <libs/binary_op/example/binary_f.h>
#include <libs/binary_op/example/untuple.h>

#include <boost/ref.hpp>

void example_untupler(std::ostream& out){
    out << "-> example_untupler : ";

    using namespace boost;
    using namespace binary_op;
    typedef untupler<binary_f,0,1> unt_;
    
    const unsigned a = 8;
    const unsigned b = 9;
    
    BOOST_AUTO(t,make_tuple(a,b));
    binary_f f_(out);
    BOOST_ASSERT(f_.a_ == 0);
    BOOST_ASSERT(f_.b_ == 0);

    unt_ unt0(f_); 
        
    typedef 
        boost::is_same<binary_f::result_type, unt_::result_type> pred_0;
    BOOST_STATIC_ASSERT(pred_0::value);

    unt0(t);
    BOOST_ASSERT(f_.a_ == 0);
    BOOST_ASSERT(f_.b_ == 0);


    out << "<-" << std::endl;
    
}