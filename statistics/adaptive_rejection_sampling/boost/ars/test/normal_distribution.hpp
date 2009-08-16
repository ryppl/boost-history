///////////////////////////////////////////////////////////////////////////////
// ars::test::normal_distribution.cpp                                        //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ARS_TEST_NORMAL_DISTRIBUTION_HPP_ER_2009
#define BOOST_ARS_TEST_NORMAL_DISTRIBUTION_HPP_ER_2009
#include <iostream>
#include <string>
#include <boost/random/mersenne_twister.hpp>
#include <boost/standard_distribution/distributions/normal.hpp>
#include <boost/ars/test/standard_distribution.hpp>

namespace boost{
namespace ars{
namespace test{

// Same as standard_distribution but for a specific distribution
template<typename T>
void normal_distribution(
    T mu,
    T sigma,
    T init_0, //must be < mu
    T init_1, //must be > mu
    unsigned draw_n,
    unsigned repeat_n,
    unsigned n_max_reject,
    std::ostream& out
)
{

    using namespace boost;
    using namespace math;
    using namespace assign;
    typedef double                                          value_t;
    typedef ars::constant<value_t>                          const_;
    typedef math::normal_distribution<value_t>              mdist_t;
    typedef boost::mt19937                                  urng_t;

    const value_t inf_ = const_::inf_;

    format f("N(0,%1%) : "); f%sigma;
    out << f.str();
    mdist_t mdist(mu,sigma);
    urng_t urng;

    standard_distribution(
        mdist,
        inf_,
        inf_,
        init_0,
        init_1,
        urng,
        n_max_reject,
        draw_n,
        repeat_n,
        out
    );
}

}//test
}//ars
}//boost

#endif
