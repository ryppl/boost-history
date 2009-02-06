//////////////////////////////////////////////////////////////////////////////
// example/norm.cpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <vector>
#include <boost/algorithm/l2_norm.hpp>
#include <boost/algorithm/linfinity_norm.hpp>
#include <boost/algorithm/l2_distance_squared.hpp>
#include <boost/algorithm/linfinity_distance.hpp>
#include <boost/assign.hpp>
#include <boost/range.hpp>
#include <libs/algorithm/src/example/norm.h>

void example_norm(){
        std::cout << "->example_squared_l2_norm()" << std::endl;

        typedef std::vector<double> vector_type;
        vector_type vec0(2); vec0[0]=-1.0; vec0[1]=1.0;
        vector_type vec1(2); vec1[0]=1.0; vec1[1]=-1.0;

        std::cout
            << " l2_norm(vec0) = "
            << boost::l2_norm()(vec0)
            << std::endl;

        std::cout
            << " linfinity_norm(vec0) = "
            << boost::linfinity_norm()(vec0)
            << std::endl;
        std::cout
            << " l2_distance_squared (vec0,vec1) = "
            << boost::l2_distance_squared::type()(vec0,vec1)
            << std::endl;
        std::cout
            << " linfinity_distance (vec0,vec1) = "
            << boost::linfinity_distance::type()(vec0,vec1)
            << std::endl;
        std::cout << "<-" << std::endl;
};

