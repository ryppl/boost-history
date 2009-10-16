//////////////////////////////////////////////////////////////////////////////
// vector_space::doc::readme                                                //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

[ Contact ]

erwann.rogard@gmail.com

[ Overview ]

These are C++ tools for basic vector space operations (e.g. norm) that work
with STL containers.

For example, let R0/r0 and R1/r1 denote ranges (ForwardRange) of scalar type and
object pairs. This tests for || r0 - r1 || == 0 :

	equal<R0> eq(r0);
	bool r0_eq_r1 = eq(r1);

[ Compiler ]

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1

[ Dependencies ]

/usr/local/boost_1_40_0/ 
/Users/erwann/sandbox/statistics/detail/tuple/ 
/Users/erwann/sandbox/statistics/mpl/ 

[ History ]

October 8th 2009    : 
    vector_space namespace embedded in statistics::detail
    implem of lazy_difference modified
    
July 2009           : First version

[Output]

main.cpp

-> example_difference : <- 
-> example_equal : (1)
0 1 3 
3 4 6 
6 7 9 
<-