/////////////////////////////////////////////////////////////////////////////
// main.cpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <cmath>
#include <boost/math_limit/miscellanea.hpp>
#include <libs/adaptive_rejection_sampling/src/example/test_gaussian.h>

using namespace std;

int main()
{
    using namespace boost;

    typedef double      d_t;
    typedef long double ld_t;
    typedef boost::math_limit::exp_support<d_t>     exp_dsup_t;
    typedef boost::math_limit::exp_support<ld_t>    exp_ldsup_t;
    d_t d_es_l = exp_dsup_t::lowest();
    d_t d_es_h = exp_dsup_t::highest();
    ld_t ld_es_l = exp_ldsup_t::lowest();
    ld_t ld_es_h = exp_ldsup_t::highest();
    d_t d_h = math::tools::max_value<d_t>();
    d_t d_l = - d_h;
    ld_t ld_h = math::tools::max_value<ld_t>();
    ld_t ld_l = - ld_h;

    d_t d_e_es_l = math_limit::truncated_exp(d_es_l);
    d_t d_e_es_h = math_limit::truncated_exp(d_es_h);
    ld_t ld_e_es_l = math_limit::truncated_exp(ld_es_l);
    ld_t ld_e_es_h = math_limit::truncated_exp(ld_es_h);
    d_t d_e_l = math_limit::truncated_exp(d_l);
    d_t d_e_h = math_limit::truncated_exp(d_h);
    ld_t ld_e_l = math_limit::truncated_exp(ld_l);
    ld_t ld_e_h = math_limit::truncated_exp(ld_h);

    const d_t dtol = 1e-10;
    const ld_t ldtol = 1e-10;

    BOOST_ASSERT(fabs(d_e_es_l-d_e_l)<dtol);
    BOOST_ASSERT(fabs(d_e_es_h-d_e_h)<dtol);
    BOOST_ASSERT(fabs(ld_e_es_l-ld_e_l)<ldtol);
    BOOST_ASSERT(fabs(ld_e_es_h-ld_e_h)<ldtol);

    libs::adaptive_rejection_sampling::example_standard_gaussian();
    return 0;
}
