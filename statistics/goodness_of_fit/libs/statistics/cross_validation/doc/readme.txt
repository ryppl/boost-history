//////////////////////////////////////////////////////////////////////////////
// goodness_of_fit::doc::readme                                            //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

[ Contact ]

erwann.rogard@gmail.com

[ Overview ]

C++ tools for cross validation.  

[ Compiler ]

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1

[ Dependencies ]

/usr/local/boost_1_39_0/
sandbox/statistics/functional/ 
/sandbox/statistics/estimator_concept/ 
/sandbox/statistics/mpl/ 
/sandbox/statistics/binary_op/ 
/sandbox/statistics/random/ 
/sandbox/statistics/arithmetic/
/sandbox/statistics/scalar_dist/ 
/sandbox/statistics/standard_distribution/ 
/sandbox/statistics/estimator_concept/ 
/sandbox/statistics/dist_random/ 

[ History ]

August 2009 - Creation

[ TODO ]

Currently, k_fold_data makes copy of the whole dataset. When join(vec0,vec1)
is available, we may be able avoid duplicate data.

[ Sources ]

http://en.wikipedia.org/wiki/Cross-validation_%28statistics%29
