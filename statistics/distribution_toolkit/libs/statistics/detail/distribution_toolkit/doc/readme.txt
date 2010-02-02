//////////////////////////////////////////////////////////////////////////////
// distribution::toolkit::doc::readme                                        //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

[ Contact ]

erwann.rogard@gmail.com

[ Overview ]

These are C++ classes that extend boost/math/distributions and also bridges it
with boost/random. functions that take as argument classes in namespace
boost/math are in the same namespace. functions and classes that do not 
already exist in boost/math are in namespace boost/statistics/detail/
distribution/toolkit.

[ Related links ]

http://www.boost.org/doc/libs/1_40_0/libs/math/doc/sf_and_dist/html/index.html
http://www.boost.org/doc/libs/1_40_0/libs/random/index.html

[ Platform/Compiler ]

Tested January 20, 2010:
Mac OS X 10.6        gcc 4.0                                    boost_1_41_0

Note: gcc 4.2 and boost::format recently don't mix well.

Tested Jan 6 2010:
Win7-32 V6.1.7100    Dev-C++ 5.0 beta 9.2 with Mingw/GCC 3.4.2 	boost_1_41_0
Win7-32 V6.1.7100    Visual Studio 2010                         boost_1_41_0

[ Dependencies ]

/sandbox/statistics/non_parametric/ 
/sandbox/statistics/distribution_common/ 
/sandbox/statistics/random/
/usr/local/boost_1_41_0/

[ Bugs ]
	
[ Directories ]

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

/map_pdf

     e.g. pdf(make_inverse_pdf(dist),x)

    Rationale: the pseudo distribution returned by make_inverse_pdf is more
    convenient than a hypothetical function inverse_pdf(dist,x). For example,
    pseudo distributions can be combined (make_ratio_pdf) and the functionality
    associated with distributions can be reused (e.g. fun_wrap::pdf_).

/meta
/test    
        
[ History ]

Jan 20 2010 :  
	- Began /distributions/poisson
    - Update to sampler.cpp due to changes in /non_parametric/kolmogorov_smirnov

Jan 6 2010 :  Fixed this bug : example/distribution_function outputs a boost 
format runtime error under MSVC and mingw.

Dec 4 2009 : 
	/math_fwd removed. Instead, any extension to boost::math stays in this 
    namespace to permit Koenig Lookup.
    /fun_wrap removed. Instead, functor for pdf, cdf etc are in 
    distribution_common

Nov 6 2009 : Added error-handling policy to location_scale

Oct 11 2009 : 
    removed /data. use instead sandbox/non_parametric/statistic
    added   /test

Oct 2 2009  
        Removed /concept, replaced by /distribution_common. 
        Split the namepsace distribution_toolkit into
        distribution::toolkit. Brought some function uder this namespace into
        statistics::detail. Concept checks in libs/.../test

Sep 23 2009 : Merged 
        sandbox/statistics/scalar_dist 
        sandbox/statistics/standard_distribution
        sandbox/statistics/dist_random
        sandbox/statistics/empirical_cdf
    into this package, with modifications to simplify the interface.
Sep 16 2009 : deleted /algorithm and added /iterator
July 2009   : First version

[ TODO ]
- Eliminate build warnings under MSVC and mingw.
- search TODO inside project
- range and support for location_scale
- other distributions