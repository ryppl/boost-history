//////////////////////////////////////////////////////////////////////////////
// distribution_toolkit::doc::readme                                        //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

[ Contact ]

erwann.rogard@gmail.com

Please report your experience (good or bad) if are you using MSVC or gcc on 
any platform other than Mac, from exectuting the examples in
libs/statistics/detail/distribution_toolkit/example

[ Overview ]

These are C++ classes that extend boost/math/distributions. However, it is not
intrusive.  Instead, the distributions in boost::math are brought into the scope 
of distribution_toolkit with forwarding functions.

[ Related links ]

http://www.boost.org/doc/libs/1_40_0/libs/math/doc/sf_and_dist/html/index.html
http://www.boost.org/doc/libs/1_40_0/libs/random/index.html

[ Platform/Compiler ]

Mac OS Leopard 10.5         gcc version 4.0.1
                            Intel C++ compiler 11.1
                            
[ Dependencies ]

boost_1_40_0
/sandbox/statistics/random/ 

To use distributions/wrapper,
libboost_serialization-xgcc42-mt-1_39.a

[ Directories ]

/data
/distributions
    
    Provides extended functionality for distributions in boost::math or adds
    distributions not available under this scope, such as location_scale

    For example,   
        log_unnormalized_pdf(dist,x)
    What for?
    - exp( sum{ log_pdf : i=1,...,n } ) is preferable to prod{ pdf : i=1,..,n }  
    - The normalizing constant is either unnecessary (Bayesian) or can be
    cached under iid.

    make_random(dist) returns a model of RandomDistribution 

/concept
/fun_wrap 

    Decouples the function, such as pdf, from the distribution D:
        fun_wrap::cdf_<D>::instance

/fwd_math 
/iterator
/map_pdf

     e.g. pdf(make_inverse_pdf(dist),x)

    Rationale: the pseudo distribution returned by make_inverse_pdf is more
    convenient than a hypothetical function inverse_pdf(dist,x). For example,
    pseudo distributions can be combined (make_ratio_pdf) and the functionality
    associated with distributions can be reused (e.g. fun_wrap::pdf_).

/meta
/random
    
    Maps a distribution and a uniform random number generator to a variate 
    generator
    
/unscope

    bring some functions in scope boost::statistics::distribution_toolkit to
    scope boost, but only (via enable_if) for those distributions that verify 
    the predicate distribution_toolkit::meta::is_pseudo_scalar_distribution
    
    The rationale for this is to have a common interface for the distributions
    in distribution_toolkit, and others defined throughout boost::statistics
        
[ Bugs ]

See distributions/wrapper/wrapper.hpp

[ History ]

Oct 2   Transfered parts of /meta and /concept to newly created library
        /distribution_common. Concept checks in libs/.../test
Sep 23 2009 : Merged 
        sandbox/statistics/scalar_dist 
        sandbox/statistics/standard_distribution
        sandbox/statistics/dist_random
        sandbox/statistics/empirical_cdf
    into this package, with modifications to simplify the interface.
Sep 16 2009 : deleted /algorithm and added /iterator
July 2009   : First version

