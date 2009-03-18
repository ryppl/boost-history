/////////////////////////////////////////////////////////////////////////////
//  main.cpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <libs/random_regression/src/example/as_random_distribution.h>
#include <libs/random_regression/src/example/as_random_regression.h>
#include <libs/survival_data/src/example/unit.h>
#include <libs/survival_data/src/example/generate.h>
#include <libs/survival_data/src/example/generate_fixed_design_no_censoring.h>
#include <libs/survival_data/src/example/transform_if_to_unit_at_time.h>
#include <libs/survival_data/src/example/stream_to_dataset.h>

int main(){

    example_as_random_distribution();
    example_as_random_regression();
    example_survival_data_unit();
    example_survival_data_generate();
    example_survival_data_generate_fixed_design_no_censoring();
    example_survival_data_transform_if_to_unit_at_time();
    //Warning : make sure to set the path in the cpp file :
    example_survival_data_stream_to_dataset();

    return 0;
};
