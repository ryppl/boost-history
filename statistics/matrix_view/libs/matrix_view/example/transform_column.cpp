///////////////////////////////////////////////////////////////////////////////
// view::example::transform_column.cpp                                //  
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
#include <boost/iterator/transform_iterator.hpp>
#include <boost/matrix_view/algorithm/transform_column.hpp>
#include <libs/matrix_view/example/transform_column.h>

void example_transform_column(std::ostream& out)
{
    out << "-> example_transform_column" << std::endl;
    
    using namespace boost;
    typedef int                                     val_;
    typedef std::vector<val_>                       vec_;
    typedef range_size<vec_>::type                  size_;

    const unsigned n_col = 2;
    const unsigned n_row = 5;
    {
        typedef range_iterator<vec_>::type              it_;

        vec_ vec;
        vec_ column;
        {
            using namespace boost::assign; 
            size_ n = n_col * n_row;
            for(unsigned i = 0; i< n; i++){
                vec.push_back(i);
            }
        }
        for(unsigned j = 0; j<n_col; j++){
            column.clear();
            view::copy_column<n_col>(
                boost::begin(vec),
                boost::end(vec),
                j,
                std::back_inserter(column)
            );
            for(unsigned i = 0; i<n_row; i++){
                BOOST_ASSERT(
                    vec[ i * n_col + j ] == column[i]
                );
            }
        }

    }

    out << "<-" << std::endl;
}