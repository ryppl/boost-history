////////////////////////////////////////////////////////////////////////////
// math_limit::miscellanea.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_LIMITS_MISCELLANEA_HPP_ER_2009
#define BOOST_MATH_LIMITS_MISCELLANEA_HPP_ER_2009
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <boost/assert.hpp>
#include <boost/format.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/static_assert.hpp>
#include <boost/math/special_functions/next.hpp>
#include <boost/math/tools/precision.hpp>
#include <boost/numeric/conversion/converter.hpp>
#include <boost/math/special_functions/fpclassify.hpp>
namespace boost{
namespace math_limit{

        //Discussed in
        //comp.lang.c++/browse_thread/thread/976701405e6ad0cf?hl=en#
        template<typename RealType>
        struct exp_support{
            static RealType highest(){
                struct check{
                    // Warning: fails with  RealType = long double with
                    // i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1
                    check(RealType x){
                        BOOST_ASSERT(!math::isinf(exp(x)));
                    }
                };
                static RealType result
                    = math::tools::log_max_value<RealType>();
                static check const the_check( result );
                return result;
            }

            static RealType lowest(){
                struct check{
                    //TODO is this what needs to be checked?
                    check(RealType x){
                        BOOST_ASSERT(!math::isnan(exp(x)));
                    }
                };
                static RealType result
                    = math::tools::log_min_value<RealType>();
                static check const the_check( result );
                return result;
            }
        };


        // Warning : if possible avoid truncated expression
        // Better to dispatch on inf
        template<typename RealType>
        RealType
        truncated_exp(RealType a){
            typedef math_limit::exp_support<RealType> sup_t;
            RealType e = 0.0;
            RealType h = sup_t::highest();
            RealType l = sup_t::lowest();
            if(a < h){
                if(a > l){
                    e = exp(a); // -inf < a < inf
                }else{
                    e = 0.0;
                }
            }else{
                e = math::tools::max_value<RealType>(); // a = inf
            }
            BOOST_ASSERT(!std::isnan(e));
            return e;
        }


    struct scale_to_max{
        //news://news.gmane.org:119/gq3755$vb9$1@ger.gmane.org
        template<class T>// (x/c)+(y/c) <= max
        static T plus(
            const T& x,
            const T& y
        ) {
            double max =  (std::numeric_limits<T>::max)();
            if(x > max - y) {
                double c = x/max + y/max;
                while(!boost::math::isinf(x/c + y/c)) {
                    c = boost::math::float_prior(c);
                }
                while(boost::math::isinf(x/c + y/c)) {
                    c = boost::math::float_next(c);
                }
                return(c);
            } else {
                return(static_cast<T>(1));
            }
        }
        template<class T>// (x/c)+(y/c) <= max
        static T divides(
            const T& x,
            const T& y
        ) {
            double max =  (std::numeric_limits<T>::max)();
            if(x / max > y) {
                double c = (x/max)/y;
                while(!boost::math::isinf((x/c)/y)) {
                    c = boost::math::float_prior(c);
                }
                while(boost::math::isinf((x/c)/y)) {
                    c = boost::math::float_next(c);
                }
                return(c);
            } else {
                return(static_cast<T>(1));
            }
        }

    };


}
}

#endif // MISCELLANEA_HPP_INCLUDED
