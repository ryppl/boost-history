///////////////////////////////////////////////////////////////////////////////
// view::example::step_iterator.cpp                                          //  
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
#include <boost/view//step_count_down_iterator.hpp>
#include <libs/view/example/step_iterator.h>

void example_step_iterator(std::ostream& os)
{
    os << "->example_step_iterator" << std::endl;
    using namespace boost;
    using namespace view;
    typedef int val_;
    typedef std::vector<val_> vals_;
    typedef range_iterator<vals_>::type vals_it_;
    typedef range_difference<vals_>::type diff_;
    typedef view::detail::step_count_down_iterator<vals_it_> step_it_; 

    const unsigned n = 3;
    const diff_ m = 2;
    vals_ vals;
    vals.reserve( n * m );
    
    for(unsigned i = 0; i<n*m; i++){
        vals.push_back(i);
    }
    vals_it_ b = boost::begin( vals );
    vals_it_ e = boost::end( vals );

    for(unsigned j = 0; j<m; j++){
        vals_it_ it = boost::next(b,j);
        step_it_ sb     
            = view::detail::make_step_count_down_iterator(it,e,m);
        step_it_ se     
            = view::detail::make_end_step_count_down_iterator(it,e,m);
        BOOST_ASSERT(se.count_down() == 0);
        BOOST_ASSERT(std::distance(sb,se)==n);
        {
            step_it_ si = sb;
            unsigned i = 0;
            //jth column
            while(si<se){
                val_ k = i*m + j;
                BOOST_ASSERT(k == *si);
                ++i;
                ++si;
            }
            si = boost::prior(si,n);
            BOOST_ASSERT(si==sb);
        }
        {
            step_it_ si = se;
            unsigned i = n;
            do{
                --si;
                --i;
                val_ k = i*m + j;
                BOOST_ASSERT(*si==k);
            }while(
                si!=sb
            );
            si = boost::next(si,n);
            BOOST_ASSERT(si==se);
        }
    }
    os << " end." << std::endl;
}


