/////////////////////////////////////////////////////////////////////////////
// sampler.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_KEYWORD_SAMPLER_HPP_ER_2009
#define BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_KEYWORD_SAMPLER_HPP_ER_2009
#include <boost/mpl/void.hpp>
#include <boost/parameter/keyword.hpp>
namespace boost{
namespace conditionally_specified_distribution{
namespace sampler{

//The kwd random_number_generator need not be enclosed in
// namepsace sampler{}
//because it's very unlikely to conflict with keywords for parameters.

namespace tag{
    template<typename Id> struct random_number_generator_;
    template<typename Id> struct ars_init_interval_;
    template<typename Id> struct ars_interval_cdf_;
    template<typename Id> struct ars_max_init_recursion_;
}

namespace{
    template<typename Id = mpl::void_>
    struct kwd{
        typedef tag::random_number_generator_<Id> rng;
        typedef tag::ars_init_interval_<Id>            ars_i;
        typedef tag::ars_interval_cdf_<Id>             ars_c;
        typedef tag::ars_max_init_recursion_<Id>       ars_m_i_r;

        static
            ::boost::parameter::keyword<rng>&        random_number_generator;
        static
            ::boost::parameter::keyword<ars_i>&      ars_init_interval;
        static
            ::boost::parameter::keyword<ars_c>&      ars_interval_cdf;
        static
            ::boost::parameter::keyword<ars_m_i_r>&  ars_max_init_recursion;
    };
    template<typename Id>
    ::boost::parameter::keyword<tag::random_number_generator_<Id> >&
            kwd<Id>::random_number_generator =
        ::boost::parameter::keyword<
            tag::random_number_generator_<Id> >::get();
    template<typename Id>
    ::boost::parameter::keyword<tag::ars_init_interval_<Id> >&
            kwd<Id>::ars_init_interval =
        ::boost::parameter::keyword<
            tag::ars_init_interval_<Id> >::get();
    template<typename Id>
    ::boost::parameter::keyword<tag::ars_interval_cdf_<Id> >&
            kwd<Id>::ars_interval_cdf =
        ::boost::parameter::keyword<
            tag::ars_interval_cdf_<Id> >::get();
    template<typename Id>
    ::boost::parameter::keyword<tag::ars_max_init_recursion_<Id> >&
            kwd<Id>::ars_max_init_recursion =
        ::boost::parameter::keyword<
            tag::ars_max_init_recursion_<Id> >::get();
}//anonymous
}//sampler
}//distribution
}//boost

#endif
