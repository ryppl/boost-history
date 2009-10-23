///////////////////////////////////////////////////////////////////////////////
// statistics::survival::model::src::main.cpp                                //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <boost/statistics/survival/constant.hpp>
#include <libs/statistics/survival/model/example/exponential.h>
#include <libs/statistics/survival/model/example/posterior_analysis.h>

int main(){
    using namespace boost;

    example_exponential(std::cout);
    typedef statistics::survival::constant<double> const_;

    example_posterior_analysis(
    // TODO n_bath should be deduced from input (getline)
        5e4, // n_batch,        
        1e3, // n_batch_mod,    
        1e4, // n_proposal,      
        1e4, // n_t_pars,      
        const_::inf_, //entry_bound
        "/Users/erwann/projets/2009/Xcode/survival/build/Release/",  // in_path
        "/Users/erwann/projets/2009/Xcode/survival/build/Release/",  // out_path
        std::cout
    );
    
    return 0;
}