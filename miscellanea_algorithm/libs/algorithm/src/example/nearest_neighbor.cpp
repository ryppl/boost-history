//////////////////////////////////////////////////////////////////////////////
// example/nearest_neighbor.cpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>
#include <iostream>//TODO remove
#include <boost/assert.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/algorithm/find_nearest_neighbor.hpp>
#include <libs/algorithm/src/example/nearest_neighbor.h>

void example_nearest_neighbor()
{
    std::cout << "->example_find_nearest_neighbor" << std::endl;

    using namespace boost;
    typedef std::vector<int>                        vec_type;
    typedef std::vector<vec_type>                   vecs_type;
    vec_type vec0, vec1, vec2, vec3, vec_a;
    vecs_type vecs;
    {
        using namespace boost::assign;
        vec0  += 0,  1   ,2 ;
        vec1  += 3,  4   ,5 ;
        vec2  += 6,  7   ,8 ;
        vec3  += 9,  10  ,11;
        vec_a += 3,  4   ,5 ;
        vecs_type vecs;
    }
    vecs.push_back(vec0);
    vecs.push_back(vec1);
    vecs.push_back(vec2);
    vecs.push_back(vec3);

    BOOST_ASSERT(boost::size(vecs)!=0);

    boost::find_nearest_neighbor<vecs_type>::result_type res =
        boost::make_find_nearest_neighbor(vecs)(vec_a);

    copy(
        boost::begin(*res),
        boost::end(*res),
        std::ostream_iterator<int>(std::cout," ")
    );//should return 4,5,6

    std::cout << "<-" << std::endl;

}

