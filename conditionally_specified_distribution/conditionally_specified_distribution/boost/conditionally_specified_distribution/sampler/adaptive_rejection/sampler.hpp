/////////////////////////////////////////////////////////////////////////////
// sampler.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_SAMPLER_ADAPTIVE_REJECTION_HPP_ER_2009
#define BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_SAMPLER_ADAPTIVE_REJECTION_HPP_ER_2009
#include <boost/assert.hpp>
#include <vector>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/eval_if.hpp>
//#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/utility/dont_care.hpp>
#include <boost/shared_features/set.hpp>
#include <boost/shared_features/parameter.hpp>
//#include <boost/adaptive_rejection_sampler/simulator.hpp>
#include <boost/adaptive_rejection_sampling/sampler.hpp>
#include <boost/conditionally_specified_distribution/support/include.hpp>
#include <boost/conditionally_specified_distribution/result_of/include.hpp>
#include <boost/conditionally_specified_distribution/keyword/sampler.hpp>
#include <boost/conditionally_specified_distribution/function/detail/adapter.hpp>
#include <boost/conditionally_specified_distribution/sampler/adaptive_rejection/initializer.hpp>
namespace boost{namespace conditionally_specified_distribution{
namespace sampler{

namespace impl{

    template<template<typename,typename> class Cont>
    struct apply_reserve{
            template<typename Impl,typename T>
            void operator()(Impl& impl, T n){}
    };
    template<>
    struct apply_reserve<std::vector>{
            template<typename Impl,typename T>
            void operator()(Impl& impl, T n){
                impl.reserve(n);
            }
    };

    template<typename Par>
    class adaptive_rejection_sampler_base{
        public:
            typedef typename function_argument<Par>::type value_type;
            typedef std::size_t                           size_type;
            struct identifier
                : conditionally_specified_distribution::identifier<Par>{};

    };
    // Instance (i) of InitialzationPolicy : i.update(par,impl,args)
    // Instance (s) of State: s.set(args,x)
    template<
        typename Par,
        typename State,
        template<typename,typename> class Initialize,
        template<typename,typename> class Container,
        template<typename> class Allocator,
        template<typename> class Unif01
    >
    class adaptive_rejection_sampler
        : public adaptive_rejection_sampler_base<typename Par::traits>
    {
        typedef
            adaptive_rejection_sampler_base<typename Par::traits> super_t;
        typedef typename conditionally_specified_distribution
            ::identifier<super_t>::type                         id;
        typedef Initialize<typename super_t::value_type,id>     init_t;
        // common urng, interval and max_init_
        // TODO last 2 may require id-templatized kwd
        typedef kwd<> kwd;

        std::size_t static default_max_init_recursion(){
            static std::size_t result = 100;
            return result;
        }
        std::size_t static default_reserve(){
            static std::size_t result = 10;
            return result;
        }
        public:

        template<typename Args>
        adaptive_rejection_sampler(const Args& args)
        : init_(args),
        max_init_recursion_(
         args[kwd::ars_max_init_recursion|default_max_init_recursion()]
        ),
        reserve_(default_reserve()){}

        adaptive_rejection_sampler(const adaptive_rejection_sampler& that)
        : init_(that.init_),
        max_init_recursion_(that.max_init_recursion_),
        reserve_(that.reserve_){}

        //TODO check
        adaptive_rejection_sampler&
        operator=(const adaptive_rejection_sampler& that){
            if(&that!=this){
                init_ = that.init_;
                max_init_recursion_ = that.max_init_recursion_;
                reserve_ = that.reserve_;
            }
            return *this;
        }

        // U models UniformRandomNumberGenerator

        template<typename Args>
        void
        sample(const Args& args)const{
            this->update_par(args[shared_features::kwd_set],args);
            return this->sample_impl(
                args,
                args[shared_features::kwd_set],
                args[kwd::random_number_generator]
            );
        }

        private:
        template<typename Set,typename Args>
        void update_par(Set& set,const Args& args)const{
            typedef typename
                Set::template result_of_extract<Par>::type    extr_par_t;
            extr_par_t& ref_par = set.template extract<Par>();
            ref_par.update(args);
        };

        template<typename Args,typename Set,typename U>
        void
        sample_impl(const Args& args,Set& set, U& urng)const{
            typedef Unif01<typename super_t::value_type>        unif01_t;
            typedef boost::variate_generator<U&,unif01_t>       gen_t;
            typedef typename init_t::result_type                interval_t;
            typedef typename
                Set::template result_of_extract<Par>::type    extr_par_t;
            typedef typename
                Set::template result_of_extract<State>::type  extr_state_t;
            typedef function::detail::adapter<extr_par_t,Args>  adapter_t;
            typedef ::boost::adaptive_rejection_sampling::sampler<
                adapter_t,
                Container,
                Allocator
            >  impl_t;
            const extr_par_t& par = set.template extract<Par>();
            adapter_t adapter (par,args);
            //TODO (*)
            //static impl_t impl(
            //    adapter,
            //    this->max_init_recursion_
            //);

            impl_t impl(
                adapter,
                this->max_init_recursion_
            );

            interval_t interval = (this->init_)();
            // TODO (*)
            // This causes problem due I think to dependence adapter
            // on Args (wrap around an abstract base class?)
            // impl.reset_distribution_function(adapter);
            typedef apply_reserve<Container> apply_res_t;
            apply_res_t()(impl,reserve_);
            impl.initialize(interval.first,interval.second);

            gen_t gen(urng,unif01_t());
            typename super_t::value_type x = impl(gen);

            extr_state_t& extr_state = set.template extract<State>();
            extr_state.set(args,x);
            (this->init_).update(impl);
            if(reserve_<impl.size_range_point()){
                reserve_ = impl.size_range_point();
            }
        }
        mutable init_t                          init_;
        typename super_t::size_type             max_init_recursion_;
        mutable typename super_t::size_type     reserve_;
    };
}//impl

namespace feature{
    // Warning : pair_quantile_averaged_over_iterations is likely
    // to require fewer iterations than pair_quantile.
    // Non-markovian : consequential for convergence?

    template<
        typename Par,
        typename State,
        template<typename,typename> class Initialize
            = pair_quantile_averaged_over_iterations,
        template<typename,typename> class Container = std::vector,
        template<typename> class Allocator = std::allocator,
        template<typename> class Unif01 = boost::uniform_real
    >
    struct adaptive_rejection_sampler : shared_features::depends_on<
        mpl::vector<Par,State>
    >{
        typedef impl::adaptive_rejection_sampler_base<
            typename Par::traits>                       traits;
        typedef impl::adaptive_rejection_sampler<
            Par,
            State,
            Initialize,
            Container,
            Allocator,
            Unif01
        > impl;
    };
}//feature
}//sampler
}//distribution
}//boost

#endif
