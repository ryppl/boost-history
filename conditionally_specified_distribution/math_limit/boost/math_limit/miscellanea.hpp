////////////////////////////////////////////////////////////////////////////
// math_limit::miscellanea.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_LIMITS_MISCELLANEA_HPP_ER_2009
#define BOOST_MATH_LIMITS_MISCELLANEA_HPP_ER_2009
#include <boost/math/tools/precision.hpp>
namespace boost{
namespace math_limit{

        template<typename RealType>
        struct limits{
            static RealType log_max_value(){
                return math::tools::log_max_value<RealType>();
            }
            static RealType log_min_value(){
                return math::tools::log_min_value<RealType>();
            }
            static RealType negative_infinity(){
                return -infinity();
            }
            static RealType min_value(){
                //beware math::tools::min_value<RealType>() is the
                // min positive value (for floats);
                return math::tools::min_value<RealType>();
            }
            static RealType infinity(){
                return math::tools::max_value<RealType>();
            }
            static RealType is_negative_infinity(RealType x){
                return !(x>negative_infinity());
            }
            static RealType is_infinity(RealType x){
                return !(x<infinity());
            }
        };

    template<typename RealType>
    RealType
    safer_exp(RealType a){
        typedef math_limit::limits<RealType> limits_t;
        RealType e = 0.0;
        RealType lmx = limits_t::log_max_value();
        RealType lmi = limits_t::log_min_value();
        if(a < lmx){
            if(a > lmi){
                e = exp(a); // -inf < a < inf
            }else{
                e = 0.0;
            }
        }else{
            e = limits_t::infinity(); // a = inf
        }
        BOOST_ASSERT(!std::isnan(e));
        return e;
    }

}
}

#endif // MISCELLANEA_HPP_INCLUDED
