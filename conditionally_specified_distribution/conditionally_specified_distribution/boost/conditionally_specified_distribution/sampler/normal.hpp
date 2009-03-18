/////////////////////////////////////////////////////////////////////////////
// normal.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_SAMPLER_NORMAL_HPP_ER_2009
#define BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_SAMPLER_NORMAL_HPP_ER_2009
#include <boost/assert.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/assert.hpp>
//#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/utility/dont_care.hpp>
#include <boost/shared_features/set.hpp>
#include <boost/shared_features/parameter.hpp>
#include <boost/conditionally_specified_distribution/result_of/include.hpp>
#include <boost/conditionally_specified_distribution/keyword/sampler.hpp>

namespace boost{
namespace conditionally_specified_distribution{
namespace sampler{

namespace impl{
    template<typename Par>
    struct normal_base{
        typedef typename function_argument<Par>::type value_type;
    };
    template<
        typename Par,
        typename State,
        template<typename> class RNormal, //models RandomDistribution
        template<typename> class Unif01
    >
    class normal : public normal_base<typename Par::traits>
        //might want to add crtp if need arises
    {
        typedef Par                                 par_t;
        typedef State                               state_t;
        typedef normal_base<typename Par::traits>   super_t;

        public:
        normal(){};
        normal(utility::dont_care){}

        template<typename Args>
        void sample(Args& args)const{
            this->update_par(args[shared_features::kwd_set],args);
            this->sample_impl(
                args,
                args[shared_features::kwd_set],
                args[kwd<>::random_number_generator]
            );
        }

        private:
        template<typename Set,typename Args>
        void update_par(Set& set, const Args& args)const{
            typedef typename
                Set::template result_of_extract<par_t>::type extr_par_t;
            extr_par_t& ref_par = set.template extract<par_t>();
            ref_par.update(args);
        }

        // U models UniformRandomNumberGenerator
        template<typename Args,typename Set,typename U>
        void sample_impl(
            const Args& args,
            Set& set,
            U& u
        )const{
            typedef RNormal<typename super_t::value_type>   rnorm_t;
            typedef Unif01<typename super_t::value_type>    unif_t;
            typedef boost::variate_generator<U&,unif_t>     gen_t;
            static rnorm_t                                  rnorm;
            typedef typename
                Set::template result_of_extract<par_t>::type extr_par_t;
            typedef typename
                Set::template result_of_extract<state_t>::type extr_state_t;

            const extr_par_t& cref_par = set.template extract<par_t>();
            typename super_t::value_type m      = cref_par.mu(args);
            typename super_t::value_type s      = cref_par.sigma(args);

            gen_t gen(u,unif_t());
            typename super_t::value_type z = rnorm(gen);
            typename super_t::value_type x = m + z * s;
            extr_state_t& state = set.template extract<state_t>();
            state.set(args,x);
        }

    };

}
namespace feature{
    template<
        typename Par,
        typename State,
        template<typename> class RNormal = boost::normal_distribution,
        template<typename> class Unif01 = boost::uniform_real
    >
    struct normal : shared_features::depends_on<
        mpl::vector<Par,State>
    >{

        typedef impl::normal_base<typename Par::traits>     traits;
        typedef impl::normal<Par,State,RNormal,Unif01>      impl;
    };
}


}
}
}

#endif // BOOST_DISTRIBUTION_SAMPLER_NORMAL_HPP_ER_2009
