///////////////////////////////////////////////////////////////////////////////
// view::example::columns_iterator.cpp                                       //  
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
#include <boost/view//columns_iterator.hpp>
#include <libs/view/example/step_iterator.h>

void example_columns_iterator(std::ostream& os)
{
    os << "->example_columns_iterator" << std::endl;
    using namespace boost;
    using namespace view;
    typedef int val_;
    typedef std::vector<val_> vals_;
    typedef range_iterator<vals_>::type vals_it_;
    typedef range_difference<vals_>::type diff_;
    typedef view::detail::columns_iterator<vals_it_> cols_it_;   
    typedef iterator_value<cols_it_>::type col_; 
    typedef range_iterator<col_>::type col_it_;

    const unsigned n = 3;
    const diff_ m = 2;
    vals_ vals;
    vals.reserve( n * m );
    
    for(unsigned i = 0; i<n*m; i++){
        vals.push_back(i);
    }
    vals_it_ b = boost::begin( vals );
    vals_it_ e = boost::end( vals );

    cols_it_ cols_b(
        b,
        e,
        m
    );
    cols_it_ cols_e = view::detail::make_end_columns_iterator(
        b,
        e,
        m
    );
    cols_it_ cols_i = cols_b;
    BOOST_ASSERT(std::distance(cols_i,cols_e)==m);
    // Outer loop = columns
    unsigned j = 0;
    while(cols_i<cols_e){
        col_ col = *cols_i;
        col_it_ col_b = boost::begin(col);
        col_it_ col_e = boost::end(col);
        BOOST_ASSERT(std::distance(col_b,col_e)==n);
        unsigned i = 0;
        while(col_b!=col_e)
        {
            BOOST_ASSERT(i<n);
            val_ k = i*m + j;
            BOOST_ASSERT(*col_b == k);
            ++col_b;
            ++i;
        }
        BOOST_ASSERT(j<m);
        ++cols_i;
        ++j;
    }
    os << " end." << std::endl;
}
