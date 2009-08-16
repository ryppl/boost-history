//////////////////////////////////////////////////////////////////////////////
// standard_distribution::doc::readme                                       //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

[ Contact ]

erwann.rogard@gmail.com

[ Overview ]

These are C++ classes that extend boost/math/distributions.  

[ Useful links ]

http://www.boost.org/doc/libs/1_35_0/libs/math/doc/sf_and_dist/html/index.html

[ Compiler ]

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1

[ Dependencies ]

boost_1_39_0

/usr/local/boost_1_39_0/ 
/sandbox/statistics/scalar_dist/ 
/sandbox/statistics/random/
/sandbox/statistics/standard_distribution/

Link to libboost_serialization-xgcc42-mt-1_39.a

[ History ]

July 2009 : Current version

[ os ]

    Adds the ostream& operator<<(ostream&, d) to each distribution class
    
[ unnormalized_pdf ]

    Implements 
        log_unnormalized_pdf(dist,x)
        derivative_log_unnormalized_pdf(dist,x)

    What for?
    - exp( sum{ log_pdf } ) is often preferable to prod{ pdf }  
    - The normalizing constant is either unnecessary (Bayesian) or can be
    cached
    - The derivative is useful in applications such as derivate-adaptive-
    rejection sampling (Gilks, 1992).

[ is_scalar_dist ]
    
    Implements 
        is_scalar_dist<D>

[ normalizing_constant ]

    normalizing_constant(dist)
    
    Only the normal is implemented right now (TODO).

[ transformation ]

    location_scale implements X = sigma Z + mu

[ primitives ]

    A class such as normal_distribution is associated with 
    normal_distribution_primitives that is serializable and equality comparable.
        
[ Note ]

Exception handling is either copy-pasted from boost/math/distributions, 
with any change is indicated within the code itself or has yet to be 
implemented (TODO).


[ Output ]

main.cpp:

-> example_math_location_scale->
log_unnormalized_pdf(mstud,2.132) = -2.0608 
    deleg(mstud,2.132) = -2.0608
log_unnormalized_pdf(mls_stud,2.132) = -5.14341 
    deleg(mls_stud,2.132) = -5.14341
log_unnormalized_pdf(prod_dist,2.132) = -7.20421 
    deleg(prod_dist,2.132) = -7.20421
log_unnormalized_pdf(inv_dist,2.132) = 2.0608 
    deleg(inv_dist,2.132) = 2.0608
log_unnormalized_pdf(ratio_dist,2.132) = 3.08261 
    deleg(ratio_dist,2.132) = 3.08261
<-
 -> example_serialize<-