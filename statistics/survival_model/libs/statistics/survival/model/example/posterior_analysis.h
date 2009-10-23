///////////////////////////////////////////////////////////////////////////////
// statistics::survival::model::example::model::posterior_analysis.h         //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef LIBS_SURVIVAL_EXAMPLE_MODEL_POSTERIOR_ANALYSIS_HPP_ER_2009
#define LIBS_SURVIVAL_EXAMPLE_MODEL_POSTERIOR_ANALYSIS_HPP_ER_2009
#include <ostream>
#include <string>

void example_posterior_analysis(
    // TODO n_bath should be deduced from input (getline)
    const long& n_batch,        // = 5e4; 
    const long& n_batch_mod,    // = 1e3;
    const long& n_proposal,     // = 1e4; 
    const long& n_t_pars,       // = 1e4;
    const double& entry_bound,  // = const_::inf_;
    const std::string& in_path,        // = "/Users/erwann/projets/2009/Xcode/survival/build/Release/    
    const std::string& out_path,       // = "/Users/erwann/projets/2009/Xcode/survival/build/Release/
    std::ostream& out
);

#endif