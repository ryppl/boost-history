///////////////////////////////////////////////////////////////////////////////
// view::example::rows_iterator.cpp                                          //  
//                                                                           //
//  (C) Copyright 2009 Erwann Rogard                                         //
//  Use, modification and distribution are subject to the                    //
//  Boost Software License, Version 1.0. (See accompanying file              //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>
#include <boost/range.hpp>
#include <boost/foreach.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/matrix_view/iterator/rows_iterator.hpp>
#include <libs/matrix_view/example/step_iterator.h>

void example_rows_iterator(std::ostream& os)
{
    os << "->example_rows_iterator" << std::endl;
    using namespace boost;
    using namespace view;
    typedef int val_;
    typedef std::vector<val_> vals_;
    typedef range_iterator<vals_>::type vals_it_;
    typedef range_difference<vals_>::type diff_;
    typedef view::detail::rows_iterator<vals_it_> rows_it_;   
    typedef iterator_value<rows_it_>::type row_; 
    typedef range_iterator<row_>::type row_it_;

    const unsigned n = 3;
    const diff_ m = 2;
    vals_ vals;
    vals.reserve( n * m );
    
    for(unsigned i = 0; i<n*m; i++){
        vals.push_back(i);
    }
    vals_it_ b = boost::begin( vals );
    vals_it_ e = boost::end( vals );

    rows_it_ rows_b(
        b,
        e,
        m
    );
    rows_it_ rows_e = view::detail::make_end_rows_iterator(
        b,
        e,
        m
    );
    rows_it_ rows_i = rows_b;

    BOOST_ASSERT(
        std::distance(rows_i,rows_e)==n
    );
    // Outer loop = rows
    unsigned i = 0;
    while(rows_i<rows_e){
        row_ row = *rows_i;
        row_it_ row_b = boost::begin(row);
        row_it_ row_e = boost::end(row);
        os << "std::distance(row_b,row_e) = " << std::distance(row_b,row_e) << std::endl;
        BOOST_ASSERT(std::distance(row_b,row_e)==m);
        unsigned j = 0;
        while(row_b!=row_e)
        {
            BOOST_ASSERT(j<m);
            val_ k = i*m + j;
            BOOST_ASSERT(*row_b == k);
            ++row_b;
            ++j;
        }
        BOOST_ASSERT(i<n);
        ++rows_i;
        ++i;
    }

    os << " end." << std::endl;
}