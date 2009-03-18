///////////////////////////////////////////////////////////////////////////////
// survival_data::generate.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SURVIVAL_DATA_GENERATE_HPP_ER_2009
#define BOOST_SURVIVAL_DATA_GENERATE_HPP_ER_2009
#include <vector>
#include <boost/random_regression/as_random_distribution.hpp>
#include <boost/survival_data/generate_given_entry_time_covariate.hpp>
#include <boost/survival_data/unit.hpp>
namespace boost{
namespace survival_data{
	/// \class generate
    /// \brief Generates survival data units
	///
	/// Models RandomDistribution
    /// Clock models RandomDistribution or Generator
    /// Cov   models RandomDistribution or Generator
    /// Fail  models RandomRegression or RandomDistribution
    /// Cens  models RandomRegression or RandomDistribution
	template<
        typename Clock,typename Cov,typename Fail,typename Cens,
        template<typename,typename> class Cont = std::vector,
        template<typename> class Alloc = std::allocator
	>
	class generate{
        typedef generate_given_entry_time_covariate<
            as_random_regression<Fail>,
            as_random_regression<Cens>,
            Cont,
            Alloc
        >   impl_type;

	public:
        typedef typename impl_type::input_type            input_type;
        typedef typename impl_type::result_type           result_type;
    private:
        typedef as_random_distribution<Clock,input_type> as_r_clock_t;
    public:
        template<typename ArgumentPack>
		generate(const ArgumentPack& args)
		:clock_(args),
		gen_cov_(args),
        impl_(args){}

        template<
            typename ClockArg,
            typename CovArg,
            typename FailArg,
            typename CensArg
        >
		generate(
            //If Clock = ref<Clock0>, pass Clock, not Clock0
            const ClockArg& a0,
            const CovArg&   a1,
            const FailArg&  a2,
            const CensArg&  a3
        ) //no const intentional
		:clock_(a0),
		gen_cov_(a1),
		impl_(a2,a3){}

		generate(const generate& that)
		:clock_(that.clock_),
		gen_cov_(that.gen_cov_),
		impl_(that.impl_){}

		generate&
		operator=(const generate& that){
            if(&that!=this){
                clock_          = that.clock_;
                gen_cov_        = that.gen_cov_;
                impl_           = that.impl;
            }
            return *this;
        }
        template<typename U>
        result_type operator()(U& urng){
            return impl_(
                urng,
                clock_(urng),
                gen_cov_(urng)
            );
        }

        const as_r_clock_t& clock()const{ return clock_; }

    private:
        as_r_clock_t                                   clock_;
        as_random_distribution<Cov,input_type>         gen_cov_;
        impl_type   impl_;
	};

}
}

#endif
