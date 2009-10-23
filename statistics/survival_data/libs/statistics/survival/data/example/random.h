///////////////////////////////////////////////////////////////////////////////
// statistics::survival::data::example::random.h                             //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef LIBS_SURVIVAL_DATA_EXAMPLE_RANDOM_HPP_ER_2009
#define LIBS_SURVIVAL_DATA_EXAMPLE_RANDOM_HPP_ER_2009
#include <ostream>
#include <string>

void example_random(
    const unsigned&      n_record,          // = 1e2;
    const unsigned&      n_batch,           // = 5e4;
    const double&        mu,                // = 0.0;
    const double&        sigma,             // = 5.0;
    const double&        t,                 // = 0.0
    const double&        delta_t,           // = 0.0
    const std::string&   out_path,
    std::ostream& out
);

#endif