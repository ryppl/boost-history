///////////////////////////////////////////////////////////////////////////////
// ars::example::standard_distribution.cpp                                   //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <boost/statistics/detail/distribution_toolkit/distributions/gamma/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/normal/include.hpp>
#include <boost/ars/test/gamma_distribution.hpp>
#include <boost/ars/test/normal_distribution.hpp>
#include <libs/ars/example/standard_distribution.h>

// TODO all other concave standard distributions:
// See paper by Mark Bagnoli

void example_standard_distribution(std::ostream& out){
    out << "-> example_standard_distribution " << std::endl;

    using namespace boost::statistics::detail;

    // This example checks the convergence of ars for standard distributions
    // by verifying that the kolmogorov-smirnov statistic converges to zero

    typedef double value_;

    const unsigned n1 = 5e0;    // # loops
    const unsigned n2 = 1e1;   	// # subsamples on first loop  
    const unsigned n3 = 1e0;   	// size of subsample 
    const unsigned n4 = 1e1;   	// At each loop, n2 *= n4
    const unsigned max_n_reject = 10;

	ars::test::standard_distribution::header(out);

    // The initial values are chosen to test the robustness of the
    // implementation, within the range allowed by the algorithm.
    {   // Domain = [0,inf)
        value_ shape = 3.0;
        value_ scale = 1.0;
        value_ mode = (shape - 1.0) * scale; //2
        typedef ars::test::gamma_distribution test_;
        test_::call(
            shape,          
            scale,          
            mode + 100.0,   //init0
            mode + 100.01,  //init1
            n1,
            n2,          
            n3,        
            n4,
            max_n_reject,        
            out
        ); 
        test_::call(
            shape,      //shape
            scale,      //scale
            mode + 0.01,//init0
            mode + 0.02, //init1
            n1,
            n2,          
            n3,        
            n4,
            max_n_reject,        
            out
        );
        
    }
    {   // Domain = (-inf,inf)
        value_ mu = 0.0;
        value_ sigma = 2.0;
        typedef ars::test::normal_distribution test_;
        test_::call(
            mu,
            sigma,
            mu -100.0, //-100.0, //init0
            mu + 0.01, //+ 0.01,    //init1
            n1,
            n2,          
            n3,        
            n4,
            max_n_reject,        
            out
        );
        
        test_::call(
            mu,
            sigma,
            mu - 0.01,   //init0
            mu + 100.0,  //init1
            n1,
            n2,          
            n3,        
            n4,
            max_n_reject,        
            out
        );
        
        test_::call(
            mu,
            sigma,
            -0.01,      //init0
            0.01,       //init1
            n1,
            n2,          
            n3,        
            n4,
            max_n_reject,        
            out
        );
        
    }
}
