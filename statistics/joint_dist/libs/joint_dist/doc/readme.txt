//////////////////////////////////////////////////////////////////////////////
// joint_dist::doc::readme                                                  //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

[ Contact ]

erwann.rogard@gmail.com

[ Overview ]

These C++ classes mirror sandbox/statistics/scalar_dist but in the multivariate 
framework.

[ Compiler ]

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1

[ Dependencies ]

/usr/local/boost_1_39_0/ 
/sandbox/statistics/arithmetic/ 
/sandbox/statistics/dist_random 
/sandbox/statistics/mpl/ 
/sandbox/statistics/scalar_dist/ 
/sandbox/statistics/standard_distribution/ 
/sandbox/statistics/vector_space/ 

[ Bugs ]

A possible bug is described in example_1.cpp

[ History ]

July 2009 : Current version

[ distributions ]
    Collects hpp files pertaining to a given distribution

[ distribution ]

    Defines a distribution and possibly associated functions 
    (log_unnormalized_pdf). Currently:
        iid
        independent

[ fun_wrap ]

    Similar to fun_wrap in scalard_dist

[ meta ]

    Similar to meta in scalard_dist. 
    
    is_joint_dist is used to manage the overload set non-intrusively (SFINAE), 
    by client libraries, where either a scalar or a joint distribution is a 
    valid argument to a function template.
    
[ random ]

    Let b_x point to the beginning of a range of scalars
    Let b_row point to beginning of a range of ranges of scalars.

        sample(b_x,joint,urng);
        generate_n(b_row,n,joint,urng);

[ unscope ]

    A version of pdf, cdf etc., defined in scope boost that forwards to that 
    in scope boost::joint_dist. 
    
    The rationale for this, is that some function templates in other libraries 
    in sandbox/statitics apply to scalar and joint distributions alike, and 
    therefore require a common interface. 

[ Output ]

main.cpp

 -> example_example_1
0.213436 -0.49558 
1.57538 -1.0592 
1.83927 1.88577 
0.604675 -0.365983 
-0.578264 -0.634376 
1.02974 0.724106 
-0.115074 0.635774 
-3.00939 -0.913024 
3.24072 -0.886412 
2.55662 -0.573354 

0.213436 1.57538 
1.83927 0.604675 
-0.578264 1.02974 
-0.115074 -3.00939 
3.24072 2.55662 
0.882285 2.76953 
0.465082 -1.78131 
0.260849 0.0615231 
-0.430637 1.23333 
0.538072 1.14551 <-
