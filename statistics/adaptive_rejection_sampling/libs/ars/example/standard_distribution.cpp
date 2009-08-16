///////////////////////////////////////////////////////////////////////////////
// ars::example::standard_distribution.cpp                                   //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <boost/standard_distribution/distributions/gamma.hpp>
#include <boost/standard_distribution/distributions/normal.hpp>
#include <boost/ars/test/gamma_distribution.hpp>
#include <boost/ars/test/normal_distribution.hpp>
#include <libs/ars/example/standard_distribution.h>

// TODO all other concave standard distributions:
// See paper by Mark Bagnoli

void example_standard_distribution(std::ostream& out){
    out << "-> example_standard_distribution " << std::endl;

    typedef double value_;

    // The initial values are chosen to test the robustness of the
    // implementation, within the range allowed by the algorithm.
    {   // Domain = [0,inf)
        value_ shape = 3.0;
        value_ scale = 1.0;
        value_ mode = (shape - 1.0) * scale; //2
        boost::ars::test::gamma_distribution<value_>(
            shape,      //shape
            scale,      //scale
            mode + 100.0,//init0
            mode + 100.01, //init1
            1,          //draw_n = 1,
            2e1,        //repeat_n = 1e2,
            1e1,        //n_max_reject = 1e1
            out
        ); out << std::endl;
        boost::ars::test::gamma_distribution<value_>(
            shape,      //shape
            scale,      //scale
            mode + 0.01,//init0
            mode + 0.02, //init1
            1,          //draw_n = 1,
            2e1,        //repeat_n = 1e2,
            1e1,        //n_max_reject = 1e1
            out
        );
        out << std::endl;
    }
    {   // Domain = (-inf,inf)
        value_ mu = 0.0;
        value_ sigma = 2.0;
        boost::ars::test::normal_distribution<value_>(
            mu,
            sigma,
            mu -100.0, //-100.0, //init0
            mu + 0.01, //+ 0.01,    //init1
            1,      //draw_n,
            1e2,    //repeat_n,
            1e1,    //n_max_reject
            out
        );
        out << std::endl;
        boost::ars::test::normal_distribution<value_>(
            mu,
            sigma,
            mu - 0.01,   //init0
            mu + 100.0,  //init1
            1,      //draw_n,
            1e2,    //repeat_n,
            1e1,    //n_max_reject
            out
        );
        out << std::endl;
        boost::ars::test::normal_distribution<value_>(
            mu,
            sigma,
            -0.01, //init0
            0.01,    //init1
            1,      //draw_n,
            1e2,    //repeat_n,
            1e1,    //n_max_reject
            out
        );
        out << std::endl;
    }
}
