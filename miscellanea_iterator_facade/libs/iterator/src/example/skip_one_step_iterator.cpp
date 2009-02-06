//////////////////////////////////////////////////////////////////////////////
// example/skip_one_step_iterator.cpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <vector>
#include <iterator>
#include <boost/range.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/iterator/skip_one_step_iterator.hpp>
#include <libs/iterator/src/example/skip_one_step_iterator.h>


template<typename R>
//void func(R& range,std::size_t i){//fine
    //typedef R                                           range_type;//fine
void func(const R& range,std::size_t i){//fails
    typedef const R                                       range_type;//fails
    typedef typename boost::range_iterator<range_type>::type    iter_type;
    typedef boost::skip_one_step_iterator<iter_type>    skipr_iter_type;
        iter_type b = boost::begin(range);
        iter_type e = boost::end(range);
        iter_type skip = b;
        std::advance(skip,i);
        skipr_iter_type sb = boost::make_skip_one_step_begin_iterator(b,skip);
        skipr_iter_type se = boost::make_skip_one_step_end_iterator(e,skip);
        copy(
            sb,
            se,
            std::ostream_iterator<double>(std::cout<<"  "," ")
        ); std::cout << std::endl;
};

void example_skip_one_step_iterator(){
    std::cout << "->example_skip_one_step_iterator" << std::endl;
    using namespace boost;
    typedef std::vector<double>                         vector_type;
    typedef vector_type::size_type                      size_type;

    vector_type vec;
    {
        using namespace boost::assign;
        vec += 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0;
    }


    //invalid initialization of reference of type ‘double&’ from expression of
    // type ‘const double’|
    //this error disappears if range_type replaced by R in definition of func
    for(size_type  i = 1; i < size(vec); i++){
        std::cout << "i=" << i << std::endl;
        func(vec,i);
    }
    std::cout << "<-" << std::endl;
};
