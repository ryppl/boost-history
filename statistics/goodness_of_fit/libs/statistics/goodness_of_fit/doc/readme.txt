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

The data structure k_fold_data keeps a dataset partitioned between train and 
test subset. The mf increment() changes the partition to the next of K 
partitions. The implementation relies on a circular buffer. 

The class k_fold_estimate automates the task of producing estimates for each
element of the test sample of a client supplied estimator (e). A derived class
takes care of extracting the argument, (a), of a test data point, t, that
is passed to (e). For example, if (e) is a regression estimator, t = (x,y), so 
a = x.

[ Compiler ]

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1

[ Dependencies ]

/usr/local/boost_1_39_0/
/sandbox/statistics/vector_space/ 
/sandbox/statistics/functional/ 
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

Currently, k_fold_data makes copy of the whole dataset and splits it between
a train and a test dataset. If/when we have a join_view(vec0,vec1) we may be 
able avoid duplicate data.

[ Sources ]

http://en.wikipedia.org/wiki/Cross-validation_%28statistics%29
