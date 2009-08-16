//////////////////////////////////////////////////////////////////////////////
// example/main.cpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <libs/monomials_horner/example/monomials.h>
#include <libs/monomials_horner/example/multi_indexes.h>
#include <libs/monomials_horner/example/multi_factorial.h>

int main()
{
    

    example_monomials(std::cout);
    example_multi_indexes(std::cout);
    example_multi_factorial(std::cout);

    return 0;
}
