//////////////////////////////////////////////////////////////////////////////
// statistics::model::doc::readme                                           //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

[ Contact ]

erwann.rogard@gmail.com

[ Overview ]

This C++ package contains 

i) Wrappers are individual or bundled references to quantities such as data, 
parameters and model abstractions, that arise in frequentist or Bayesian 
modeling. They give explicit meaning to the above quantities and simplify the
syntax of functions that takes these quantities as arguments.

ii) STL-like algorithms for computing likelihoods or posterior distributions, 
by aggretating the contributions of data-units in an (for now) iid setting.

iii) An importance_sampler

Examples can be found in statistics/survival/libs/example

[ Compiler ]

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1

[ Dependencies ]

/boost_1_39_0/ 

/sandbox/statistics/scalar_dist/ 
/sandbox/statistics/joint_dist/ 
/sandbox/statistics/binary_op/
/sandbox/statistics/importance_sampling/
/sandbox/statistics/importance_weights/

[ History ]

July 2009 : Current version

[ Notation ]

LPdf and Model are concepts defined within this package.  

        Type   Object       Abstraction             Concept
        name   name
    -   T       t           scalar                  is_scalar
    -   X       x           covariate               
    -   Y       y           response                
    -   P       p           parameter               
    -   D       d           prior distribution      LPdf 
    -   M       m           likelihood model        Model
    -   Rx      rx          covariates              ForwardRange
    -   Ry      ry          responses               ForwardRange
    -   O       o                                   OutputIterator

Note that X, Y, P are not retricted to scalars. For example,    
    X == std::vector<T>
    Y == survival::data::event<T>

    Class                           Alias   Object name
    model_parameter_<M,P>           mp_      mp
    model_dataset_<M,Rx,Ry>         md_      md
    prior_model_dataset_<D,M,Rx,Ry> pmd_     pmd
          
[b_x,e_x) and [b_p,e_p) denote iterator ranges to covariates and
parameter values, respectively.

[ Concepts ]

- HasLogLikelihood
    m models Model iff
    Expression                                  Return type
    log_likelihood(make_model_data(m,x,y),p)    T

- HasLogUnnormalizedPdf
    d models LPdf iff
    Expression                                  Returns 
    log_unnormalized_pdf(d,p)                   T

Note that : 
- is_scalar_dist<D> => HasLogUnnormalizedPdf 
- joint_dist<D> => HasLogUnnormalizedPdf 
See unscope in either package.

[ wrap ]
    Example of wrappers:
        md_ md(m,rx,ry);
        md.covariates();
        
    Often we only need a temporary, in which case it is preferable to call an 
    associated creator function that deduces the types of its arguments:
        make_model_dataset(m,rx,ry);

[ functional ]
    Functors that support [ algorithm ]
        
[ algorithm ]
    Iterate over a dataset or parameter values to produce a likelihood or
    set of likelihood (or posterior) values 

    Expression                          Return type
    log_likelihood(mp,b_x,e_x,b_y)      T
    log_likelihoods(md,b_p,e_p,o)       O
    log_posteriors(pmd,b_p,e_p,o)       O

[ TODO ]

- Some simple examples.
- Check integration with Boost.Probability i.e. T =  boost::log_likelihood 
- Think about non-iid
- Think about derivatives.