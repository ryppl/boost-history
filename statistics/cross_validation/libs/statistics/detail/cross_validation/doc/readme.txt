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

The data structure k_fold::partition keeps a dataset partitioned between train 
and test subset. The mf increment() changes the partition to the next of K 
partitions. Parameterized functors take care of extracting the training, input
and output data.

The function train_predict automates the task of producing estimates for each
element of the test sample of a client supplied estimator. 

[ Compiler ]

Mac OS Leopard 10.5         gcc version 4.0.1
                            Intel C++ compiler 11.1

[ Dependencies ]

/usr/local/boost_1_39_0/
/sandbox/statistics/vector_space/ 
/sandbox/statistics/mpl/ 

[ History ]

September 2009  - goodness_of_fit superseded by cross_validation with a better
                interface.
August 2009     - Creation of goodness_of_fit

[ TODO ]

Currently, k_fold_data makes a copy of a dataset. Intrusive or range
(pair of iterator) alternatives may need to be considered.

[ Sources ]

http://en.wikipedia.org/wiki/Cross-validation_%28statistics%29
