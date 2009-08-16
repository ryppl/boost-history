//////////////////////////////////////////////////////////////////////////////
// random::location_scale.hpp                                               //
//                                                                          //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_RANDOM_LOCATION_SCALE_HPP_ER_2009
#define BOOST_RANDOM_LOCATION_SCALE_HPP_ER_2009
#include <vector>
#include <algorithm>
#include <iterator>
#include <boost/range.hpp>
//#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/chi_squared.hpp>
namespace boost{
namespace random{

    // Samples from a location-scale distribution
    //
    // X = sigma Z + mu
    template<typename Z>
    class location_scale_distribution{
        public:
            typedef typename remove_cv<
                typename remove_reference<Z>::type
            >::type z_type;
            typedef typename z_type::input_type input_type;
            typedef typename z_type::result_type result_type;

        location_scale_distribution(){}
        location_scale_distribution(
            typename call_traits<Z>::param_type z,
            const result_type& mu,
            const result_type& sigma
        )
        :z_(z),mu_(mu),sigma_(sigma){}

        template<typename U>
        result_type operator()(U& urng){ return (this->impl(urng)); }

        template<typename U>
        result_type operator()(U& urng)const{ return (this->impl(urng)); }

        const result_type& mu()const{ return mu_; }
        const result_type& sigma()const{ return sigma_; }
        const Z& z(){ return z_ ;}

        private:
        typename call_traits<Z>::value_type z_;
        result_type mu_;
        result_type sigma_;
        template<typename U>
        result_type impl(U& urng){
            return (this->mu()) + (this->sigma()) * (this->z_)(urng);
        }
            
    };


}// random
}// boost

#endif
