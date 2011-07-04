///////////////////////////////////////////////////////////////////////////////
//  accumulator_ecdf                                                         //
//                                                                           //
//  Copyright (C) 2005 Eric Niebler                                          //
//  Copyright (C) 2011 Erwann Rogard                                         //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef LIBS_ACCUMULATORS_ECDF_KS_GEN_H_ER_2011
#define LIBS_ACCUMULATORS_ECDF_KS_GEN_H_ER_2011
#include <ostream>

void test_ks_gen(
    std::ostream& os,
    double mean = 1.0,
    long n = 1,
    long factor = 2,
    long size = 20
);

#endif

