///////////////////////////////////////////////////////////////////////////////
// view::example::row_iterator.cpp                                    //  
//                                                                           //
//  (C) Copyright 2009 Erwann Rogard                                         //
//  Use, modification and distribution are subject to the                    //
//  Boost Software License, Version 1.0. (See accompanying file              //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////

/*
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>
#include <boost/range.hpp>
#include <boost/foreach.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/matrix_view/iterator/row_iterator.hpp>
#include <libs/matrix_view/example/row_iterator.h>

void example_row_iterator(std::ostream& out)
{
    out << "-> example_row_iterator" << std::endl;
    
    using namespace boost;
    typedef int                                     val_;
    typedef std::vector<val_>                       vec_;
    typedef range_size<vec_>::type                  size_;
    int stride = 2;

    {
        typedef range_iterator<vec_>::type          it_;
        typedef view::row_iterator<it_>      row_it_;
        typedef view::row_iterator<it_>      row_it_;

        vec_ vec;
        vec_ vecc;
        {
            using namespace boost::assign; 
            vec += 0,1,2,3,4,5,6,7,8;
        }
        //important to use the const qualifier
        it_ base_b = boost::begin(vec);
        it_ base_e = boost::end(vec);
        row_it_ b(base_b,stride); 
        row_it_ i = b;
        row_it_ e = view::make_end_row_iterator(
            base_b,base_e,stride
        );
        while(i<e){
            row_it_::value_type r = *i;
            std::copy(boost::begin(r),boost::end(r),back_inserter(vecc));
            ++i;
        }
        {
            size_ i = 0;
            BOOST_FOREACH(const val_& x, vecc){
                x == vec[i];
                ++i;
            }
        }
    }

    {
        typedef vec_::const_iterator                it_;
        typedef view::row_iterator<it_> row_it_;
        typedef view::row_iterator<it_> row_it_;

        vec_ vec,vec_a;
        { 
            using namespace boost::assign; 
            vec += 0,1,2,3,4,5,6,7; 
        }
        it_ base_b = boost::begin(vec);
        it_ base_e = boost::end(vec);
        row_it_  b(base_b,stride);
        out << "std::distance(b,e) : ";
        
        for(it_ base_i = base_b; base_i<base_e; base_i++){
            row_it_ e = view::make_end_row_iterator(
                base_b,base_i,stride
            );
            out << std::distance(b,e) << ' '; 
        } 
    }


    out << "<-" << std::endl;
}

*/
