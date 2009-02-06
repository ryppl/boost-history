//////////////////////////////////////////////////////////////////////////////
// example/insert_element_every_n_step_iterator.cpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <vector>
#include <iterator>
#include <functional>
#include <boost/assert.hpp>
#include <boost/range.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/iterator/insert_element_every_n_step_iterator.hpp>
#include <libs/iterator/src/example/insert_element_every_n_step_iterator.h>

void example_insert_element_every_n_step_iterator(){

    std::cout << "->example_skip_one_step_iterator" << std::endl;
    using namespace boost;
    typedef double                                      value_type;
    typedef std::vector<value_type>                     vector_type;
    typedef range_iterator<vector_type>::type           base_iter_type;
    typedef vector_type::size_type                      size_type;

    vector_type vec(10,0);

    typedef boost::insert_element_every_n_step_iterator<base_iter_type>
                                                        iter_type;
    value_type x = 1;
    size_type  n = 2;


    base_iter_type base_b = begin(vec);
    iter_type b(base_b,x,n);
    iter_type i(b);
    std::advance(i,n+1);
    BOOST_ASSERT( distance(b,i) == 3);
    BOOST_ASSERT( (*i) == 1);
    ++i;
    BOOST_ASSERT( distance(b,i) == 4);
    BOOST_ASSERT( (*i) == 0);


    vector_type out;
    copy(
        make_begin_insert_element_every_n_step_iterator(
            begin(vec),
            x, n
        ),
        make_end_insert_element_every_n_step_iterator(
            begin(vec),
            end(vec),
            x, n
        ),
        back_inserter(out)
    ); //should end in 1,0,0
    base_iter_type base_i = end(out);

    BOOST_ASSERT((*(--base_i))==0);
    BOOST_ASSERT((*(--base_i))==0);
    BOOST_ASSERT((*(--base_i))==1);

    copy(
        begin(out),
        end(out),
        std::ostream_iterator<value_type>(std::cout, " ")
    );

    std::cout << "<-" << std::endl;

};
