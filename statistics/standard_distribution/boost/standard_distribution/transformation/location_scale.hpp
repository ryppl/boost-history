//////////////////////////////////////////////////////////////////////////////
// standard_distribution::transformation::location_scale.hpp                //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STANDARD_DISTRIBUTION_TRANSFORMATION_LOCATION_SCALE_HPP_ER_2009
#define BOOST_STANDARD_DISTRIBUTION_TRANSFORMATION_LOCATION_SCALE_HPP_ER_2009
#include <vector>
#include <algorithm>
#include <iterator>
#include <boost/format.hpp>
#include <boost/utility.hpp>
#include <boost/call_traits.hpp>

#include <boost/range.hpp>
//#include <boost/math/policies/policy.hpp>

namespace boost{
namespace math{

    // Represents a location scale family
    //
    // X = mu + sigma * Z
    // pdf_X(x) = (1/sigma) pdf_Z((x-mu)/sigma)
    // cdf_X(x) = cdf_Z((x-mu)/sigma)
    //
    // TODO: range and support
    template<typename Z>
    class location_scale_distribution{
        public:
        typedef typename remove_reference<
            typename remove_const<
                Z
            >::type
        >::type distribution_type;

        typedef typename distribution_type::value_type value_type;
        typedef typename distribution_type::policy_type policy_type;

        location_scale_distribution();
        location_scale_distribution(
            typename call_traits<Z>::param_type z,
            value_type mu,
            value_type sigma
        );

        location_scale_distribution(const location_scale_distribution& that);
        
        location_scale_distribution&
        operator=(const location_scale_distribution& that);
        
        value_type mu()const;
        value_type sigma()const;

        typename call_traits<Z>::const_reference z()const;

        private:
            typename call_traits<Z>::value_type z_;
            value_type mu_;
            value_type sigma_;
    };

    template<typename Z>
    std::ostream& operator<<(
        std::ostream& out, 
        const location_scale_distribution<Z>& d
    );

    template<typename Z>
    typename location_scale_distribution<Z>::value_type
    pdf(
        const location_scale_distribution<Z>& d,
        const typename 
            location_scale_distribution<Z>::value_type& x
    );

    template<typename Z>
    typename location_scale_distribution<Z>::value_type
    cdf(
        const location_scale_distribution<Z>& d,
        const typename 
            location_scale_distribution<Z>::value_type& x
    );

    template<typename Z>
    typename location_scale_distribution<Z>::value_type
    log_unnormalized_pdf(
        const location_scale_distribution<Z>& d,
        const typename 
            location_scale_distribution<Z>::value_type& x
    );

    template<typename Z>
    typename location_scale_distribution<Z>::value_type
    variance(
        const location_scale_distribution<Z>& d
    );

    template<typename Z>
    typename location_scale_distribution<Z>::value_type
    mean(
        const location_scale_distribution<Z>& d
    );

    // Implementation // 

    template<typename Z>
    std::ostream& operator<<(
        std::ostream& out, 
        const location_scale_distribution<Z>& d
    ){
        const char* msg = "location_scale_distribution( %1% + %2% %3%)";
        // Warning : os << ls.z() usually not defined (TODO)
        format f(msg); f % d.mu() % d.sigma() % d.z();
        return (out << f.str());
    }

    template<typename Z>
    location_scale_distribution<Z>::location_scale_distribution()
    :z_(),mu_(0),sigma_(1){}
    
    template<typename Z>
    location_scale_distribution<Z>::location_scale_distribution(
        typename call_traits<Z>::param_type z,
        value_type mu,
        value_type sigma
    ):z_(z),mu_(mu),sigma_(sigma){}

    template<typename Z>
    location_scale_distribution<Z>::location_scale_distribution(
        const location_scale_distribution& that
    ):z_(that.z_),mu_(that.mu_),sigma_(that.sigma_){}

    template<typename Z>
    location_scale_distribution<Z>&
    location_scale_distribution<Z>::operator=(
        const location_scale_distribution& that
    ){
        if(&that!=this){
            z_ = that.z_;
            mu_ = that.mu_;
            sigma_ = that.sigma_;
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
    typename call_traits<Z>::const_reference
    location_scale_distribution<Z>::z()const{ return this->z_; }


    template<typename Z>
    typename location_scale_distribution<Z>::value_type
    pdf(
        const location_scale_distribution<Z>& d,
        const typename 
            location_scale_distribution<Z>::value_type& x
    ){
        typedef typename 
            location_scale_distribution<Z>::value_type value_t;
        value_t z = (x-d.mu())/d.sigma();
        value_t result = pdf(d.z(),z);
        result /= d.sigma();
        return result;
    }

    template<typename Z>
    typename location_scale_distribution<Z>::value_type
    cdf(
        const location_scale_distribution<Z>& d,
        const typename location_scale_distribution<Z>::value_type& x
    ){
        typedef typename 
            location_scale_distribution<Z>::value_type value_t;
        value_t z = (x-d.mu())/d.sigma();
        return cdf(
            d.z(),
            z
        );
    }

    template<typename Z>
    typename location_scale_distribution<Z>::value_type
    log_unnormalized_pdf(
        const location_scale_distribution<Z>& d,
        const typename location_scale_distribution<Z>::value_type& x
    ){
        typedef typename location_scale_distribution<Z>::value_type value_t;
        value_t z = (x-d.mu())/d.sigma();
        value_t result = math::log_unnormalized_pdf(d.z(),z);
        // -log ( sigma ) is a constant so it is ignored
        return result;
    }

    template<typename Z>
    typename location_scale_distribution<Z>::value_type
    variance(
        const location_scale_distribution<Z>& d
    ){
        return variance(d.z()) * d.sigma() * d.sigma();
    }

    template<typename Z>
    typename location_scale_distribution<Z>::value_type
    mean(
        const location_scale_distribution<Z>& d
    ){
        return mean(d) + mean(d.z());
    }

}
}
#endif // LOCATION_SCALE_DISTRIBUTION_HPP_INCLUDED
