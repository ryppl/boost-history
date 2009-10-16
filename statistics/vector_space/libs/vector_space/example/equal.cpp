///////////////////////////////////////////////////////////////////////////////
// vector_space::example::equal.cpp                                          //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <iterator>
#include <boost/assert.hpp>
#include <boost/vector_space/functional/equal.hpp>
#include <libs/vector_space/example/equal.h>

void example_equal(std::ostream& out) {
    out << "-> example_equal : ";
    using namespace boost;
    namespace vs = boost::statistics::detail::vector_space;
    
    typedef unsigned val_;
    typedef std::vector<val_> row_;
    typedef std::vector<row_> mat_;
    typedef range_iterator<mat_>::type iterator_row_;
    typedef range_iterator<row_>::type iterator_elem_;
    typedef range_reference<mat_>::type ref_row_;
    typedef range_reference<row_>::type ref_elem_;
    typedef vs::equal<row_&> equal_;
        
    const unsigned n = 3;
    const val_ eps = 1; // defaul eps = 0, so |i-j| < 1 need, i,j integers
    row_ row(n);
    equal_ equal(row,eps);
/*

    std::cout << equal << std::endl;
    mat_ mat(n,row);
    val_ val = 0;
    for(unsigned i = 0; i<n; i++){
        val = i * n;
        ref_row_ ref_row = mat[i]; 
        for(unsigned j = 0; j<n; j++){
            val += j;
            row[j] = val;
            ref_row[j] = val;
        }
        std::copy(   
            begin( row ),
            end( row ),
            std::ostream_iterator<int>(out," ")
        ); 
        out << std::endl;
        BOOST_ASSERT(
            equal(ref_row)
        );
    }
*/
    out << "<-" << std::endl;
}