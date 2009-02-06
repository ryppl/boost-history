///////////////////////////////////////////////////////////////////////////////
// main.cpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <libs/iterator/src/example/skip_range_iterator.h>
#include <libs/iterator/src/example/vector2matrix.h>
#include <libs/iterator/src/example/skip_one_step_iterator.h>
#include <libs/iterator/src/example/insert_element_every_n_step_iterator.h>
#include <libs/iterator/src/example/subset_of_range.h>
int main(){
    example_vector2matrix();
    example_skip_range_iterator();
    example_skip_one_step_iterator();
    example_skip_one_step_iterator();
    example_insert_element_every_n_step_iterator();
    example_subset_of_range();
    return 0;
}
