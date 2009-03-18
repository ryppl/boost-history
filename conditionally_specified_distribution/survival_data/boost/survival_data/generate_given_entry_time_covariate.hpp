///////////////////////////////////////////////////////////////////////////////
// generate_given_entry_time_covariate.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SURVIVAL_DATA_RANDOM_FAILURE_CENSORING_HPP_ER_2009_
#define BOOST_SURVIVAL_DATA_RANDOM_FAILURE_CENSORING_HPP_ER_2009_
#include <boost/mpl/assert.hpp>
#include <boost/type_traits.hpp>
#include <vector>
#include <memory>
#include <boost/survival_data/unit.hpp>
namespace boost{
namespace survival_data{

	/// \class data unit
    /// \brief Generates survival data units
	///
	/// \param F (failure) models RandomRegression
	/// \param Ce (censoring) models RandomRegression
	template<
        typename F,
        typename Ce,
        template<typename,typename> class Cont = std::vector,
        template<typename> class Alloc = std::allocator
    >
	class generate_given_entry_time_covariate{
	public:
        typedef typename F::result_type               value_type;
		typedef typename F::input_type  				input_type;
        BOOST_MPL_ASSERT((is_same<value_type,typename Ce::result_type>));
        BOOST_MPL_ASSERT((is_same<input_type,typename Ce::input_type>));
    private:
        typedef Alloc<value_type>                     alloc_t;
        typedef Cont<value_type,alloc_t>              cont_t;
    public:

		typedef unit<cont_t>                          result_type;

        template<typename ArgumentPack>
		generate_given_entry_time_covariate(const ArgumentPack& args)
		:failure_(args),censoring_(args){}

        template<typename A0,typename A1>
		generate_given_entry_time_covariate(const A0& a0,const A1& a1)
		:failure_(a0),censoring_(a1){}

		generate_given_entry_time_covariate(
            const generate_given_entry_time_covariate& that)
		:failure_(that.failure_),censoring_(that.censoring_){}

		generate_given_entry_time_covariate&
		operator=(const generate_given_entry_time_covariate& that){
            if(&that!=this){
                failure_ = that.failure_;
                censoring_ = that.censoring_;
            }
            return *this;
        }
        template<typename U,typename R>
        result_type
        operator()(
            U& urng,
            value_type entry_time,
            const R& covariate
        ){

            result_type unit(entry_time,covariate);
            value_type f = failure_(urng,unit.covariate());
            value_type c = censoring_(urng,unit.covariate());
            bool is_f = (f<c) ? f : c;
            value_type t = c; if(is_f){ t = f; }
            unit.set_event(is_f,t);
            return unit;
        }
    private:
		F		    failure_;
		Ce		    censoring_;
	};

}
}

#endif
