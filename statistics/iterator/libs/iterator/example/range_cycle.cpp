///////////////////////////////////////////////////////////////////////////////
// example::iterator::range_cycle.cpp                                        //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
#include <iostream>
#include <boost/assign/std/vector.hpp>
#include <boost/range.hpp>
#include <vector>
#include <boost/iterator/cycle_iterator_ext.hpp>
#include <boost/iterator/range_cycle.hpp>
#include <libs/iterator/example/range_cycle.h>

void example_range_cycle(std::ostream& out){
    out << "->example_iterator_cycle_range : ";

    using namespace boost;

    typedef unsigned                                    val_;
    typedef std::vector<val_>                           vals_;
    typedef range_size<vals_>::type                     size_;

    typedef boost::range_cycle<>                        range_cycle_;
    typedef range_cycle_::apply<vals_>::type            cycle_;
    const size_ n = 5;
    const size_ k = 2;

    vals_ vals;
    {   
        using namespace assign;
        for(unsigned i = 0; i<k; i++){
            vals.push_back(i);
        }
    }

    cycle_ cycle = range_cycle_::make(vals,0,n);
    BOOST_ASSERT( !cycle.is_singular() );
    BOOST_ASSERT(size( cycle ) == n);
    for(unsigned i = 0; i<n; i++){
        BOOST_ASSERT(
         *next(boost::begin(cycle),i) == (i%k)
        );
    }
    cycle_ cycle2;
    BOOST_ASSERT( cycle2.is_singular() );
    cycle2 = cycle;
    for(unsigned i = 0; i<n; i++){
        BOOST_ASSERT(
         *next(boost::begin(cycle2),i) == (i%k)
        );
    }
    
//    copy(
//        boost::begin(cycle2),
//        boost::end(cycle2),
//        std::ostream_iterator<val_>(out," ")
//    );
    
    out << "<-" << std::endl;
}
