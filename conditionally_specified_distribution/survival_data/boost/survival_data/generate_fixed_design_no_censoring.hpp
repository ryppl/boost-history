///////////////////////////////////////////////////////////////////////////////
//  generate_fixed_design_no_censoring.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SURVIVAL_DATA_GENERATE_FIXED_DESIGN_NO_CENSORING_HPP_ER_2009
#define BOOST_SURVIVAL_DATA_GENERATE_FIXED_DESIGN_NO_CENSORING_HPP_ER_2009
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/void.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/exponential_distribution.hpp>
#include <boost/generator/clock.hpp>
#include <boost/generator/constant.hpp>
#include <boost/generator/cycle_through_shared_containers.hpp>
#include <boost/generator/keyword.hpp>
#include <boost/random_regression/as_random_distribution.hpp>
#include <boost/random_regression/as_random_regression.hpp>
#include <boost/random_regression/exponential.hpp>
#include <boost/survival_data/generate.hpp>
namespace boost{
namespace survival_data{

    template<typename ResultType,typename Id>
    struct default_fail{
        typedef exponential_distribution<ResultType>    exp_t;
    // Only meant to illustrate (possible) use of id
        typedef random_regression::exponential<
            exp_t,
            uniform_real,
            Id
        > type;
    };
    template<typename Fail = default_fail<double,mpl::void_>::type >
    class generate_fixed_design_no_censoring{
        public:
        typedef typename Fail::input_type input_type;
        private:
        typedef typename Fail::result_type value_type;

        typedef std::vector<value_type>                   cont_t;
        typedef generator::clock<value_type>              clock_type;
        typedef generator::ref<clock_type>                ref_clock_type;
        typedef generator::cycle_through_shared_containers<cont_t>
                                                              gen_cov_t;
        typedef generator::max_value<value_type>              cens0_t;
        typedef as_random_distribution<cens0_t,input_type>    gen_cens_t;

        typedef generate<
            ref_clock_type, //important to have ref if stop and go
            gen_cov_t,
            Fail,
            gen_cens_t
        >                                                     impl_t;
        public:
        typedef typename impl_t::result_type                  result_type;

//        template<
//            typename ClockArg,
//            typename CovArg,
//            typename FailArg
//        >
//        generate_fixed_design_no_censoring(
//            const ClockArg& a0,
//            const CovArg&   a1,
//            const FailArg&  a2
//        ):impl_(a0,a1,a2,cens0_t()){}

        template<typename Design,typename FailArg>
        generate_fixed_design_no_censoring(
            value_type start_time,
            value_type delta_time,
            const Design& design, // e.g. std::vector<range_covariate_t>
            const FailArg& fail_args
        ):impl_(
            make_ref_clock(start_time,delta_time),
            gen_cov_t(
                design,
                0
            ),
            fail_args,
            cens0_t()
        ){
        }

        generate_fixed_design_no_censoring(
            const generate_fixed_design_no_censoring& that
        ):impl_(that.impl_){}

        generate_fixed_design_no_censoring&
        operator=(
            const generate_fixed_design_no_censoring& that
        ){
            if(&that!=this){
                impl_ = that.impl_;
            }
            return *this;
        }
        template<typename U>
        result_type operator()(U& u){
            return impl_(u);
        }

        private:
        generate_fixed_design_no_censoring();
        ref_clock_type
        make_ref_clock(value_type start,value_type delta){
            typedef shared_ptr<clock_type> s_t;
            s_t s(new clock_type(start,delta));
            ref_clock_type ref_clock(s);

            return ref_clock;
        }
        impl_t impl_;
    };

}//survival_data
}//boost

#endif
