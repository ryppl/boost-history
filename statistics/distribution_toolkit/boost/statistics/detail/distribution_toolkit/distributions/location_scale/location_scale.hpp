//////////////////////////////////////////////////////////////////////////////
// distribution::toolkit::distributions::location_scale::location_scale.hpp  //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_LOCATION_SCALE_LOCATION_SCALE_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_LOCATION_SCALE_LOCATION_SCALE_HPP_ER_2009
#include <vector>
#include <algorithm>
#include <iterator>
#include <boost/mpl/assert.hpp>
#include <boost/utility.hpp>
#include <boost/type_traits.hpp> // remove_reference etc.
#include <boost/range.hpp>
#include <boost/statistics/detail/distribution_common/meta/inherit_policy.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/is_scalar_distribution.hpp>
#include <boost/statistics/detail/distribution_common/meta/value.hpp>
//#include <boost/math/policies/policy.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace toolkit{


    // Represents a location scale family
    //
    // X = mu + sigma * Z
    // pdf_X(x) = (1/sigma) pdf_Z((x-mu)/sigma)
    // cdf_X(x) = cdf_Z((x-mu)/sigma)
    //
    // TODO: range and support
    template<typename Z>
    class location_scale_distribution 
        : public distribution::common::meta::inherit_policy<Z>{
        public:

        typedef Z distribution_type;

        BOOST_MPL_ASSERT((
            meta::is_scalar_distribution<Z>
        ));

        typedef typename distribution::common::meta::value<
            distribution_type
        >::type value_type;

        location_scale_distribution();
        location_scale_distribution(
            value_type mu,
            value_type sigma,
            const Z& z
        );

        location_scale_distribution(const location_scale_distribution& that);
        
        location_scale_distribution&
        operator=(const location_scale_distribution& that);
        
        value_type mu()const;
        value_type sigma()const;

        const Z& z()const;

        private:
            value_type mu_;
            value_type sigma_;
            Z z_;
    };

    // Implementation // 

    template<typename Z>
    location_scale_distribution<Z>::location_scale_distribution()
    :mu_(0),sigma_(1),z_(){}
    
    template<typename Z>
    location_scale_distribution<Z>::location_scale_distribution(
        value_type mu,
        value_type sigma,
        const Z& z
    ):mu_(mu),sigma_(sigma),z_(z){}

    template<typename Z>
    location_scale_distribution<Z>::location_scale_distribution(
        const location_scale_distribution& that
    ):mu_(that.mu_),sigma_(that.sigma_),z_(that.z_){}

    template<typename Z>
    location_scale_distribution<Z>&
    location_scale_distribution<Z>::operator=(
        const location_scale_distribution& that
    ){
        if(&that!=this){
            mu_ = that.mu_;
            sigma_ = that.sigma_;
            z_ = that.z_;
        }
        return *this;
    }

    template<typename Z>
    typename location_scale_distribution<Z>::value_type 
    location_scale_distribution<Z>::mu()const{ return this->mu_; }

    template<typename Z>
    typename location_scale_distribution<Z>::value_type 
    location_scale_distribution<Z>::sigma()const{ return this->sigma_; }

    template<typename Z>
    const Z&
    location_scale_distribution<Z>::z()const{ return this->z_; }


}// distribution
}// toolkit

}// detail
}// statistics
}// boost
#endif 
