/////////////////////////////////////////////////////////////////////////////
// example/standard_gaussian.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_ADAPTIVE_REJECTION_SAMPLING_STANDARD_GAUSSIAN_HPP_ER_2009
#define BOOST_ADAPTIVE_REJECTION_SAMPLING_STANDARD_GAUSSIAN_HPP_ER_2009
#include <boost/math_limit/miscellanea.hpp>

namespace boost{
namespace adaptive_rejection_sampling{
	/// \class standard_gaussian_evaluator
	/// \brief Example modeling LogConcaveEvaluator
    template<typename RealType>
	class standard_gaussian_evaluator{
            typedef math_limit::limits<RealType> limits_t;
		public:
            typedef RealType                      value_type;
            typedef standard_gaussian_evaluator base_type;
			value_type unnormalized_log_pdf(value_type x)const{
                    return -x*x/2.0;
            }
			value_type derivative_unnormalized_log_pdf(value_type x)const{
                    return -x;
            }
			value_type min()const{
                return limits_t::negative_infinity();
            }
			value_type max()const{
                return limits_t::infinity();
            }
	};

}
}

#endif // STANDARD_GAUSSIAN_HPP_INCLUDED
