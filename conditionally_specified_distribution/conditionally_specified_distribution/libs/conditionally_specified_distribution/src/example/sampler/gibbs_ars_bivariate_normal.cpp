////////////////////////////////////////////////////////////////////////////
// gibbs_ars_bivariate_normal.cpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <numeric>
#include <vector>
#include <cmath>
#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/range.hpp>

//#include <boost/iterator/vector2matrix_iterator.hpp>

#include <boost/shared_features/feature/keyword.hpp>
#include <boost/shared_features/set.hpp>
#include <boost/shared_features/mpl_features.hpp>
#include <boost/shared_features/feature/scalar.hpp>
#include <boost/shared_features/functor/sample.hpp>

#include <boost/conditionally_specified_distribution/keyword/parameter.hpp>
#include <boost/conditionally_specified_distribution/parameter/normal.hpp>
#include <boost/conditionally_specified_distribution/parameter/dependent/normal_given_normal.hpp>

#include <boost/conditionally_specified_distribution/sampler/adaptive_rejection/sampler.hpp>
#include <boost/conditionally_specified_distribution/keyword/sampler.hpp>
#include <libs/conditionally_specified_distribution/src/example/sampler/gibbs_ars_bivariate_normal.h>

void example_sampler_gibbs_ars_bivariate_normal(){
    std::cout << "->example_sampler_gibbs_ars_bivariate_normal" << std::endl;

    using namespace boost;
    namespace shared = shared_features;
    namespace csd = conditionally_specified_distribution;
    namespace par = csd::parameter;
    namespace sampl = csd::sampler;

    // data type
    typedef double value_type;

    // identifiers
    typedef mpl::size_t<0>                  id_x;
    typedef mpl::size_t<1>                  id_y;
    typedef mpl::pair<id_x,id_y>            id_xy;
    typedef id_xy                           id_rho_xy;

    // atomic features
    typedef shared::feature::scalar<value_type,id_x>        state_x; //1
    typedef shared::feature::scalar<value_type,id_y>        state_y; //2
    typedef par::feature::normal<value_type,id_x>           par_x;   //3
    typedef par::feature::normal<value_type,id_y>           par_y;   //4
    typedef shared::feature::scalar<value_type,id_rho_xy>   rho_xy;  //5

    // dependent feature
    typedef
        par::feature::normal_given_normal<
            par_y,
            state_x,
            par_x,
            rho_xy
        > //6
                                                         par_y_given_x;
    typedef
        par::feature::normal_given_normal<
            par_x,
            state_y,
            par_y,
            rho_xy
        > //7
                                                         par_x_given_y;

    //sampler
    typedef sampl::feature::adaptive_rejection_sampler<
        par_x_given_y,
        state_x
    >  sampl_x_given_y;
    typedef sampl::feature::adaptive_rejection_sampler<
        par_y_given_x,
        state_y
    >  sampl_y_given_x;

    typedef mpl::vector<
        sampl_y_given_x,
        sampl_x_given_y
    >                                                   implicit_features;
    typedef shared_features::set<implicit_features>     set_t;
    typedef set_t::mpl_features::type                   wrapped_features;

    typedef mpl::transform<
        wrapped_features,
       shared::detail::feature_tag<mpl::_1>
    >::type                                            features;
    typedef shared::detail::features<implicit_features>::sorted_vector
                                                       sorted_vector;
    typedef mpl::size<features>::type                   actual_size;
    typedef mpl::size_t<9>                              desired_size;
    BOOST_MPL_ASSERT((
        mpl::equal_to<actual_size,desired_size>
    ));

    // input

    const value_type state_x_val     = -0.1;
    const value_type state_y_val     = 0.1;
    const value_type mu_x_val        = -0.1;
    const value_type sigma_x_val     = 1.0;
    const value_type mu_y_val        = 0.1;
    const value_type sigma_y_val     = 1.5;
    const value_type rho_val         = 0.9;

// The greater rho_val, the more likely a stability issues
// such as exp(-inf). These issues, however, go away if
// initializer = pair_quantiles_averaged_over_iterations
// but see sampler::adaptive_rejection::sampler.hpp for further comment.
// Warning: The output below shows a residual bias in sigma_x (0.97 vs 1.0)
// Output
// n = 10000000;
// x:
// mu  : -0.1 vs m :-0.0996011
// sigma : 1 vs s :0.973064
// y:
// mu  : 0.1 vs m :0.100748
// sigma : 1.5 vs s :1.45992
// rho  : 0.9 vs s_xy :0.896383

    BOOST_ASSERT((rho_val<1.0) && (rho_val>-1.0));

    set_t set;

    set.initialize(
        (
            shared::kwd<id_x>::scalar               = state_x_val,
            shared::kwd<id_y>::scalar               = state_y_val,
            par::kwd<id_x>::mu                      = mu_x_val,
            par::kwd<id_x>::sigma                   = sigma_x_val,
            par::kwd<id_y>::mu                      = mu_y_val,
            par::kwd<id_y>::sigma                   = sigma_y_val,
            shared::kwd<id_xy>::scalar              = rho_val
        )
    );

    typedef mpl::vector<sampl_x_given_y>                    sampler_x;
    typedef shared::contains_feature_of<sampler_x,mpl::_>   is_sampler_x;
    typedef mpl::vector<sampl_y_given_x>                    sampler_y;
    typedef shared::contains_feature_of<sampler_y,mpl::_>    is_sampler_y;

    typedef boost::mt19937                                  urng_type;
    const unsigned seed  = 0;
    urng_type urng((urng_type)(seed));

    BOOST_ASSERT((
        set.extract<state_y>()() == state_y_val
    ));
    typedef std::vector<value_type> draws_t;
    const unsigned n = 1e6;
    draws_t draws_x;
    draws_x.reserve(n);
    draws_t draws_y;
    draws_y.reserve(n);

    std::string str = "%1% samples from (x,y) jointly normal, ";
    str += "mu_x = %2% ";
    str += "sigma_x = %3% ";
    str += "rho_xy = %4% ";
    str += "mu_y = %5% ";
    str += "sigma_y = %6% ";
    str += "using the decomposition (y|x, x|y) ";
    str += "and an ars sampler for each component.";
    format the_format(str);
    the_format % n ;
    the_format % set.extract<par_x>().mu();
    the_format % set.extract<par_x>().sigma();
    the_format % set.extract<rho_xy>()();
    the_format % set.extract<par_y>().mu();
    the_format % set.extract<par_y>().sigma();
    std::cout << the_format.str() << std::endl;


    for(unsigned i=0; i<n; ++i){
        //if(i%1e4==0){
        //    std::cout << "i=" << i << std::endl;
        //}
        set.visit_if<is_sampler_x>(
            shared::functor::sample(),
            (sampl::kwd<>::random_number_generator = urng)
        );
        draws_x.push_back(
            set.extract<state_x>()()
        );
        set.visit_if<is_sampler_y>(
            shared::functor::sample(),
            (sampl::kwd<>::random_number_generator = urng)
        );
        draws_y.push_back(
            set.extract<state_y>()()
        );
    }

    std::cout << "sample statistics : " << std::endl;

    typedef range_iterator<draws_t>::type  iter_t;

    value_type m_x = std::accumulate(
        begin(draws_x),end(draws_x),(value_type)(0));
    m_x/=(value_type)(n);
    value_type s_x = std::inner_product(
        begin(draws_x),end(draws_x),begin(draws_x),(value_type)(0));
    s_x -= n*m_x*m_x;
    s_x = sqrt(s_x/(value_type)(n));

    std::cout
        << " mu_x  : " << m_x << std::endl;
    std::cout
        << " sigma_x : " << s_x << std::endl;

    value_type m_y = std::accumulate(
        begin(draws_y),end(draws_y),(value_type)(0));
    m_y/=(value_type)(n);
    value_type s_y = std::inner_product(
        begin(draws_y),end(draws_y),begin(draws_y),(value_type)(0));
    s_y -= n*m_y*m_y;
    s_y = sqrt(s_y/(value_type)(n));

    std::cout
        << " mu_y  : " << m_y << std::endl;
    std::cout
        << " sigma_y : " << s_y << std::endl;

    transform(begin(draws_x),end(draws_x),begin(draws_x),
        bind(
            std::minus<value_type>(),
            _1,
            m_x
        )
    );
    transform(begin(draws_y),end(draws_y),begin(draws_y),
        bind(
            std::minus<value_type>(),
            _1,
            m_y
        )
    );

    value_type s_xy = std::inner_product(
        begin(draws_x),end(draws_x),begin(draws_y),(value_type)(0));
    s_xy = s_xy/(value_type)(n);
    s_xy /= (s_x * s_y);

    std::cout
        << " rho_xy :"    << s_xy << std::endl;

    std::cout << "<-" << std::endl;

}

