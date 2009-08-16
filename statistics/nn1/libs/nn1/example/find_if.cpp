///////////////////////////////////////////////////////////////////////////////
// nn1::example::find_if.cpp                                                 //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
#include <vector>
#include <boost/foreach.hpp>
//#include <boost/nn1/find_nn1.hpp>//temporary
#include <boost/nn1/algorithm/find_if.hpp>
#include <boost/nn1/detail/ui_abs_to.hpp>
#include <libs/nn1/example/find_if.h>

void nn1_example_find_if(std::ostream& out){

    out << "-> nn1_example_find_if : ";
    using namespace boost;
    using namespace nn1;
    
    typedef unsigned val_; //Do not change
    typedef std::vector<val_> vec_;
    const unsigned n = 2;
        
    vec_ vec(n);
    {
        for(unsigned i = 0; i<n; i++){
            vec[i] = i;
        } 
    }
    
    val_ t = 1;
    BOOST_FOREACH(val_ i,vec){
        val_ j = *find_if<nn1::detail::ui_abs_to>(
            begin(vec),end(vec),i,t);
        BOOST_ASSERT(j==i);
    }

    t = 0;
    BOOST_FOREACH(val_ i,vec){
        BOOST_ASSERT(
            find_if<nn1::detail::ui_abs_to>(
                begin(vec),end(vec),i,t) == end(vec)
        );
    }
    
    out << "<-" << std::endl;

}
