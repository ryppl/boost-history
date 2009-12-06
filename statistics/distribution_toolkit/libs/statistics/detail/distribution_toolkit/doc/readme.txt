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
with boost/random. functions that have as argument distributions in boost/math
are in the same namespace. functions and classes that do not already exist
in boost/math are in namespace boost/statistics/detail/distribution_toolkit.

[ Related links ]

http://www.boost.org/doc/libs/1_40_0/libs/math/doc/sf_and_dist/html/index.html
http://www.boost.org/doc/libs/1_40_0/libs/random/index.html

[ Platform/Compiler ]

Mac OS X 10.6         	gcc 4.2					boost_1_41_0
Win7 V6.1.7100			Visual Studio 2010		boost_1_40_0
                            
[ Dependencies ]

/sandbox/statistics/non_parametric/ 
/sandbox/statistics/detail/fusion/ 
/sandbox/statistics/distribution_common/ 
/sandbox/statistics/random/
/sandbox/statistics/distribution_toolkit/ 
/usr/local/boost_1_40_0/

[ Directories ]

/concept
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
        
[ Bugs ]

See distributions/wrapper/wrapper.hpp

[ History ]

Dec 4 2009 : 
	/math_fwd removed. Instead, any extension to boost::math stays in this 
    namespace. Rationale : allows Koenig Lookup.
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

- range and support for location_scale
- other distributions