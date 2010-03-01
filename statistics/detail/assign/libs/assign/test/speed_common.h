//////////////////////////////////////////////////////////////////////////////
// test::speed_common.h                                                     //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Copyright 2010 Manuel Peinado Gallego
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
///////////////////////////////////////////////////////////////////////////////
#ifndef LIBS_ASSIGN_TEST_SPEED_COMMON_ER_2010_H
#define LIBS_ASSIGN_TEST_SPEED_COMMON_ER_2010_H
#include <vector>
#include <string>

double uniform_deviate ( int seed );
//int rand(int M, int N);
char rand_letter();
std::string rand_str(int len);
std::vector<int> rand_vec(int);

namespace mpg
{
    namespace detail
    {
        double clock_diff_to_sec(long clock_diff);

        template<class Proc>
        double time_it_impl(Proc proc, int N=1);        

        template<class Proc, class Result>
        double time_it_impl(Proc proc, Result & result, int N=1);
    }
        
    template<class Proc>
    double time_it(Proc proc);
    
    template<class Proc, class Result>
    double time_it(Proc proc, Result & result);

    double rand_dbl();
        
    double rand_dbl(double M, double N);
    
    // http://www.eternallyconfuzzled.com/arts/jsw_art_rand.aspx
    int rand(int M, int N);
    
    char rand_letter();

    std::string rand_str(int len);

}   


#endif