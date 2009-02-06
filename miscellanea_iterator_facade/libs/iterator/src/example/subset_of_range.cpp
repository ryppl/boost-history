//////////////////////////////////////////////////////////////////////////////
// example/subset_of_range.cpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <vector>
#include <boost/range.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/iterator/range_result.hpp>
#include <boost/iterator/range_same.hpp>
#include <boost/iterator/range_left.hpp>
#include <boost/iterator/range_right.hpp>
#include <boost/iterator/range_middle.hpp>
#include <boost/iterator/range_result_size.hpp>
#include <libs/iterator/src/example/subset_of_range.h>


void example_subset_of_range(){
    std::cout << "->example_subset_of_range " << std::endl;
    using namespace boost;

    typedef double                  value_type;
    typedef std::vector<value_type> vec_type;
    vec_type vec0;
    const std::size_t in_size = 4;

    {
        using namespace boost::assign;
        vec0+=0.1,-0.2, 0.3, -0.4;
    }

    const vec_type vec = vec0;

    typedef mpl::size_t<2> size_2;
    typedef mpl::size_t<3> size_3;
    typedef mpl::size_t<4> size_4;

    typedef range_same          range_same_type;
    typedef range_left<3>       left_range_type;
    typedef range_right<3>      right_range_type;
    typedef range_middle<1,2>   middle_range_type;

    typedef range_result_size<range_same_type,in_size>::type
                                                    same_result_size_type;
    typedef range_result_size<left_range_type,in_size>::type
                                                    left_result_size_type;
   typedef range_result_size<right_range_type,in_size>::type
                                                    right_result_size_type;
   typedef range_result_size<middle_range_type,in_size>::type
                                                    middle_result_size_type;

    BOOST_MPL_ASSERT((
        is_same<same_result_size_type,size_4>
    ));
    BOOST_MPL_ASSERT((
        is_same<left_result_size_type,size_3>
    ));
    BOOST_MPL_ASSERT((
        is_same<right_result_size_type,size_3>
    ));
    BOOST_MPL_ASSERT((
        is_same<middle_result_size_type,size_2>
    ));

    BOOST_ASSERT(size(range_same_type()(vec))==4);
    BOOST_ASSERT(size(left_range_type()(vec))==3);
    BOOST_ASSERT(*begin(left_range_type()(vec))==0.1);
    BOOST_ASSERT(size(right_range_type()(vec))==3);
    BOOST_ASSERT(*begin(right_range_type()(vec))==-0.2);
    BOOST_ASSERT(size(middle_range_type()(vec))==2);
    BOOST_ASSERT(*begin(middle_range_type()(vec))==-0.2);
    std::cout << "<-" << std::endl;
};
