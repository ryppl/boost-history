//////////////////////////////////////////////////////////////////////////////
// example/skip_range_iterator.cpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <vector>
#include <iterator>
#include <boost/range.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/iterator/skip_range_iterator.hpp>
#include <libs/iterator/src/example/skip_range_iterator.h>


void example_skip_range_iterator(){
    std::cout << "->example_skip_range_iterator" << std::endl;
    using namespace boost;
    typedef std::vector<double>                     vector_type;
    typedef vector_type::size_type                  size_type;
    //TODO replacing vector_type by const vector_type
    //causes:
    //invalid initialization of reference of type ‘double&’
    //from expression of type ‘const double’|
    typedef range_iterator<vector_type>::type       iter_type;
    typedef iterator_range<iter_type>::type         ir_type;
    typedef skip_range_iterator<iter_type>          skipr_iter_type;

    vector_type vec;
    {
        using namespace boost::assign;
        vec += 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0;
    }

    iter_type b = begin(vec);
    iter_type e = end(vec);
    vector_type::size_type sz = size(vec);
    for(size_type  i = 1; i <= sz; i++){
        std::cout << "i=" << i << std::endl;
        for(size_type j = i; j<=sz; j++)
        {
            std::cout << " j=" << j << std::endl;
            iter_type k = b;
            iter_type l = b;
            std::advance(k,i);
            std::advance(l,j);
            ir_type ri0(k,l);
            skipr_iter_type skipr_b(b,ri0);
            skipr_iter_type skipr_e(e,ri0);
            copy(
                skipr_b,
                skipr_e,
                std::ostream_iterator<double>(std::cout<<"  "," ")
            ); std::cout << std::endl;
        }
    }
    std::cout << "<-" << std::endl;
};
