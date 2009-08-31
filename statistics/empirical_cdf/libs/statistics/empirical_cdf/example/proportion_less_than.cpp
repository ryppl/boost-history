///////////////////////////////////////////////////////////////////////////////
// empirical_cdf::example::algorithm::proportion_less_than.h                 //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
#include <vector>
#include <boost/assign/std/vector.hpp>
#include <boost/statistics/empirical_cdf/algorithm/proportion_less_than.hpp>
#include <libs/statistics/empirical_cdf/example/proportion_less_than.h>

void example_algorithm_proportion_less_than(std::ostream& out){
    out << "-> example_algorithm_proportion_less_than : ";
    using namespace boost;
    using namespace statistics;

    // Types
    typedef unsigned val_;
    typedef std::vector<val_> vec_;

    // Constants
    const val_ j = 4; 

    // Initialization
    vec_ vec;
    {
        using namespace assign;
        vec += 0,1,2,3,4,5,6,7,8,9;
    }

    // Computations
    empirical_cdf::proportion_less_than(
        boost::begin(vec),
        boost::end(vec),
        j
    );
    
    out << "<-" << std::endl;
}