///////////////////////////////////////////////////////////////////////////////
// ars::test::gamma_distribution.cpp                                         //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ARS_TEST_GAMMA_DISTRIBUTION_HPP_ER_2009
#define BOOST_ARS_TEST_GAMMA_DISTRIBUTION_HPP_ER_2009
#include <iostream>
#include <string>
#include <boost/random/mersenne_twister.hpp>
#include <boost/standard_distribution/distributions/gamma.hpp>
#include <boost/ars/test/standard_distribution.hpp>

namespace boost{
namespace ars{
namespace test{

// Same as standard_distribution but for a specific distribution
template<typename T>
void gamma_distribution(
    T shape,  // must be > 1
    T scale,
    T init_0, // must be >0 and < init_1
    T init_1, // must be > mode = (m-1) * theta
    unsigned draw_n,
    unsigned repeat_n,
    unsigned n_max_reject,
    std::ostream& out
)
{

    using namespace boost;
    using namespace math;
    using namespace assign;
    typedef double                                          val_;
    typedef ars::constant<val_>                             const_;
    typedef math::gamma_distribution<val_>                  mdist_t;
    typedef boost::mt19937                                  urng_;

    format f("Gamma(%1%,%2%) : "); f%shape%scale;
    out << f.str();
    mdist_t mdist(shape,scale);
    urng_ urng;

    const val_ inf_ = const_::inf_;

    boost::ars::test::standard_distribution(
        mdist,
        static_cast<val_>(0),   // x_min
        inf_,                   // x_max
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
