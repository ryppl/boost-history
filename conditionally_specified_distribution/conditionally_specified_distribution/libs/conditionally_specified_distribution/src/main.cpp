////////////////////////////////////////////////////////////////////////////
// main.cpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <libs/conditionally_specified_distribution/src/example/parameter/normal_given_normal.h>
#include <libs/conditionally_specified_distribution/src/example/sampler/ars_normal.h>
#include <libs/conditionally_specified_distribution/src/example/sampler/exact_normal_given_normal.h>
#include <libs/conditionally_specified_distribution/src/example/sampler/exact_bivariate_normal.h>
#include <libs/conditionally_specified_distribution/src/example/sampler/gibbs_ars_bivariate_normal.h>
#include <libs/conditionally_specified_distribution/src/example/sampler/gibbs_bivariate_normal.h>
#include <libs/conditionally_specified_distribution/src/example/function/pair_independent.h>

using namespace std;

int main()
{
    example_parameter_normal_given_normal();

    //normal (from simple to complex):
    example_sampler_exact_normal_given_normal();
    example_sampler_exact_bivariate_normal();
    example_sampler_ars_normal();
    example_sampler_exact_bivariate_normal();
    example_sampler_gibbs_ars_bivariate_normal();

    example_function_pair_independent();

    return 0;
}
