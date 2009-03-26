////////////////////////////////////////////////////////////////////////////
// Conditionally Specified Distribution
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////


////////////
/ Overview /
////////////

This collection of C++ classes is a framework for managing a
multivariate distribution that can be characterized by a collection of
submodels, as arising in Bayesian modeling.

The framework is built around the library shared_features, which
allows to bundle multiple classes that depend on each other through
a common class (set).

////////////////
/ Requirements /
////////////////

boost_1_37_0
sandbox/conditionally_specified_distribution/adaptive_rejection_sampling
sandbox/conditionally_specified_distribution/conditionally_specified_distribution
sandbox/conditionally_specified_distribution/math_limit
sandbox/conditionally_specified_distribution/utility
sandbox/conditionally_specified_distribution/shared_features
sandbox/conditionally_specified_distribution/survival_data (anticipated)

/////////////////////////////////////////////////////
/ Example of a conditionally specified distribution /
/////////////////////////////////////////////////////

(x,y) are jointly normal with parameters mu_x,mu_y,rho,sigma_x, sigma_y.
An equivalent representation is
{x~N(mu_x,sigma_x^2) , y|x~N(mu_y|x,sigma_{y|x}^)},
where :
    mu_y|x = mu_y + beta(x)(x-mu_x)
    beta(x) =  rho*(sigma_y/sigma_x);
    sigma_y|x = sqrt(1.0-rho^2)*sigma_y;

We can get an exact draw from (x,y) by sampling from x, then y|x.
Alternatively, given starting values (x0, y0), the Gibbs procedure is
(x[1]|y[0],y[1]|x[1])...(x[i]|y[i-1],y[i]|x[i])...(x[n]|y[n-1],y[n]|x[n]).
which results in (x[n],y[n]) --d--> (x,y) as n ---> infinity.

////////////////
/ Organization /
////////////////
Folders:
* crtp
* function
* keyword
* parameter
* result_of
* sampler
* support

/////////
/ Usage /
/////////
Familiarize yourself with the library shared_features first.

Consider a conditional distribution Y|X (naturally this can be extended
to a collection).

To represent the state of X (or Y), we would typically reuse a feature
defined in shared_features, for example:
    typedef shared_features::feature::scalar<T> state_x;

We now assume that Y|X has a pdf form F. For example, if (X,Y) are jointly
normal, then F is univariate normal.

If F is not already a part of the library, we would add:
namespace crtp{
    template<typename D>
    struct F{};
}

We provide an implementation for the parameter of Y|X=x in namespace
parameter. To begin, put some associated type in a base class
namespace impl{
    template<typename StateX,typename Other>
    struct parameter_Y_base{
        // (nullary) meta functions
        struct function_argument{typedef ... type};
        ...
    };
}

If any of these types is needed by a client, we call them via meta-
function fowards that are defined in folder result_of. For example,
    function_value<D>::type.

We now turn to the implementation:
namespace impl{
    template<typename StateX,typename Other>
    struct parameter_Y
        : parameter_Y_base,
        crtp::F<parameter_Y<StateX,Other> >
    {
        template<typename Args>
        void update(const Args& args){
            //typically calls args[kwd_set].extract<StateX>()
        }
        // ... accessors such as mu(), sigma()
    };
}
We now make Y|X a feature which specifies its dependencies :
namespace feature{
    template<typename StateX,typename Other>
    class parameter_Y : depends_on1<StateX>{
        typedef impl::parameter_Y_base<StateX,Other> traits;
        typedef impl::parameter_Y<StateX,Other> impl;
    };
}

If F not already a part of the library, we create free functions as
needed such as:
namespace function{
    template<typename D>
    typename function_value<D>::type
    unnormalized_log_pdf(
        const crtp::F<D>& d,
        typename function_argument<D>::type x
    ){
        const D& d_ = static_cast<const D&>(d);
        ...
    }
}
namespace support{
    ... similar set up
}

If F can be sampled exactly, we would would create a feature for that
sampler:
namespace impl{
 ...
}
namespace feature{
    template<typename Par,typename State>
    struct sampler_F : depends_on<
        mpl::vector<Par,State>
    >{
        template<typename Args>
        void sample(const Args& args)const{
          //typically queries set.template extract<Par>();
          //and modifies set.template extract<State>();
        }
    }
}
If F cannot be sampled exactly, we would use (or, if necessary, define) a
generic sampler ( e.g. adaptive rejection sampler) in a similar way.

Here's how we would use of the above to sample Y|X=x

    typedef shared::feature::scalar<T>        state_x;
    typedef shared::feature::scalar<T>        state_y;
    typedef parameter_Y                       par_y;
    typedef sampler_F<
        par_y,
        state_y
    >  sampl_y;

    typedef mpl::vector<sampl_y>               features;
    typedef shared_features::set<features>     set_t;

    const value_type state_x_val = some value;

    set_t set;
    set.initialize(
        (
            shared::kwd<>::scalar      = state_x_val,
            ...
        )
    );//Read shared_features on keyword to discriminate between features
    BOOST_ASSERT(( set.extract<state_x>()() == state_x_val ));

    typedef mpl::vector<sampl_y>                            samplers;
    typedef shared::contains_feature_of<samplers,mpl::_>    is_sampler;

    typedef boost::mt19937                                  urng_type;
    const unsigned seed  = 0;
    urng_type urng((urng_type)(seed));
    typedef std::vector<T> draws_t;
    const unsigned n = 1e6;
    draws_t draws;
    draws.reserve(n);
    for(unsigned i=0; i<n; ++i){
        set.visit_if<is_sampler>(
            shared::functor::sample(),
            (sampl::kwd<>::random_number_generator = urng)
        );
        draws.push_back(
            set.extract<state_y>()()
        );
    }

//////////////
/ Discussion /
//////////////
In general, for simplicy, the base parameter is defined as
struct base{
    struct result_of_foo{};
    ...
}
Accordingly in folder result_of:
template<typename D>
struct foo<D> : mpl::identity<typename D::result_of_foo>{};

In other words, it is generally assumed that the result of D::foo(Args) is
independent of Args.

The exceptions are foo = first and foo = second, whose result depend on
the function signature. Going forward, it may be necessary to generalize
this.

/////////////////
/ Related Tools /
/////////////////

For MCMC convergence assessment, you might want to look into our library
sandbox/acvf (auto-covariance function)

////////////////////////
/ Output from main.cpp /
////////////////////////
->example_parameter_normal_given_normal
mu_y|x=0.1
beta_y|x=0.75
mu_y|x=0.175
<-
->example_exact_sample_normal_given_normal
1000000 samples from y|x, with (x,y) jointly normal,  and x fixed at -0.1
mu_x = -0.1 sigma_x = 1 rho_xy = 0.5 mu_y = 0.1 sigma_y = 1.5 so that
mu_y|x = 0.1, sigma_y|x = 1.29904and a normal sampler.
sample statistics :
 mu_y|x  : 0.0999346
 mu_y|x  : 1.29991
<-
->example_sampler_exact_bivariate_normal
1000000 samples from (x,y) jointly normal, mu_x = -0.1 sigma_x = 1
rho_xy = -0.5 mu_y = 0.1 sigma_y = 1.5 using the decomposition (x, y|x)
and a normal sampler for each component.
sample statistics :
 mu_x  : -0.101468
 sigma_x : 1.00053
 mu_y  : 0.102181
 sigma_y : 1.5014
 rho_xy :-0.500278
<-
-> example_ars_normal()
1000000 samples from N(-5.1,10^2) using an adaptive rejection sampler,
initialized withx1 = -24.7 and x2 = 14.5
sample statistics :
 mu :-5.08317
sigma : 9.75805
<-
->example_sampler_exact_bivariate_normal
1000000 samples from (x,y) jointly normal, mu_x = -0.1 sigma_x = 1
rho_xy = -0.5 mu_y = 0.1 sigma_y = 1.5 using the decomposition (x, y|x)
and a normal sampler for each component.
sample statistics :
 mu_x  : -0.101468
 sigma_x : 1.00053
 mu_y  : 0.102181
 sigma_y : 1.5014
 rho_xy :-0.500278
<-
->example_sampler_gibbs_ars_bivariate_normal
1000000 samples from (x,y) jointly normal, mu_x = -0.1 sigma_x = 1
rho_xy = 0.9 mu_y = 0.1 sigma_y = 1.5 using the decomposition (y|x, x|y)
and an ars sampler for each component.
i=0
i=100000
i=200000
i=300000
i=400000
i=500000
i=600000
i=700000
i=800000
i=900000
sample statistics :
 mu_x  : -0.0984778
 sigma_x : 0.974992
 mu_y  : 0.102567
 sigma_y : 1.46175
 rho_xy :0.896784
<-
->example_function_pair_independent
 OK <-


///////////
/ History /
///////////

On 2009/03/18, added:

parameter::dependent::detail::regression_coefficient::as_string(args)

