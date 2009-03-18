/////////////////////////////////////////////////////////////////////////////
// container_element.cpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <vector>
#include <boost/mpl/int.hpp>
#include <boost/shared_features/parameter.hpp>
#include <boost/shared_features/feature/keyword.hpp>
#include <boost/shared_features/feature/container_element.hpp>
#include <boost/shared_features/feature/container.hpp>
#include <libs/shared_features/src/example/container_element.h>

void example_feature_container_element(){
    std::cout
        << "->example_feature_container_element"
        << std::endl;

    using namespace boost;
    namespace shared = shared_features;

    typedef double value_type;
    typedef std::vector<value_type> vec_t;
    vec_t vec(3);
    vec[0] = 0.0;
    vec[1] = 0.1;
    vec[2] = 0.2;

    typedef mpl::int_<1> id_1;
    typedef shared::feature::container<vec_t> feature_cont_t;
    typedef shared::feature::container_element<feature_cont_t,id_1>
                                                feature_elem_1_t;

    typedef shared::set<
        mpl::vector<feature_elem_1_t>
    > set_t;

    set_t set;

    set.initialize((
        shared::kwd<>::container = vec,
        shared::kwd<id_1>::position = 1
    ));
    std::cout
        << "OK<-"
        << std::endl;


}
