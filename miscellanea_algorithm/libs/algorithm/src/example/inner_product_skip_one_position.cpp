//////////////////////////////////////////////////////////////////////////////
// example/inner_product_skip_one_position.cpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)#include <iostream>
#include <vector>
#include <iostream>
#include <boost/assign.hpp>
#include <boost/algorithm/inner_product_skip_one_position.hpp>
#include <libs/algorithm/src/example/inner_product_skip_one_position.h>

void example_inner_product_skip_one_position(){
    std::cout << "->example_inner_product_skip_one_position" << std::endl;
    using namespace boost;

    typedef double                      value_type;
    typedef std::vector<value_type>     vec_type;
    typedef vec_type::size_type         size_type;
    typedef std::plus<value_type>       plus_type;
    typedef std::multiplies<value_type> multiplies_type;
    vec_type vec0;
    vec_type vec1;
    {
        using namespace boost::assign;
        vec0+=-1.0; vec0+=1.0;  vec0+=1.0;
        vec1+=1.0;  vec1+=1.0; vec1+=-1.0;
    }

    for(size_type i=0; i<size(vec0); i++){
        value_type x =
            inner_product_skip_one_position<
                vec_type,
                vec_type,
                plus_type,
                multiplies_type
            >(
                vec0,
                vec1,
                i,
                0.0,
                plus_type(),
                multiplies_type()
            );
        std::cout << x << std::endl;
    }

    try{
        vec_type vec2;
        {
            using namespace boost::assign;
            vec2+=-1.0; vec2+=1.0;
        }
        inner_product_skip_one_position(
            vec0,
            vec2,
            0,
            0.0
        );
    }catch(const std::exception& e){

        std::cout << "exception=" << e.what() << std::endl;
    }

    std::cout << "<-" << std::endl;

};
