///////////////////////////////////////////////////////////////////////////////
// ars::functional::standard_distribution.hpp                                //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ARS_FUNCTIONAL_STANDARD_DISTRIBUTION_HPP_ER_2009
#define BOOST_ARS_FUNCTIONAL_STANDARD_DISTRIBUTION_HPP_ER_2009
#include <boost/type_traits.hpp>
#include <boost/call_traits.hpp>

namespace boost{

namespace math{

    template<typename D>
    typename D::value_type  
    log_unnormalized_pdf( //Fwd Declare
        const D& d,
        const typename D::value_type& x
    );

    template<typename D>
    typename D::value_type  
    derivative_log_unnormalized_pdf( //Fwd Declare
        const D& d,
        const typename D::value_type& x
    );

}

namespace ars{
namespace functional{

// Maps a subset of the free functions associated with a distribution (D) to
// a functor with the required signature.
//
// Requirements:
// D::value_type is defined and we call it T. Let x and d instances of T and D.
// Expression                               Returns
// log_unnormalized_pdf(d,x)                object of type T
// derivative_log_unnormalized_pdf(d,x)     object of type T
// For distributions such as exponential, normal etc. these are defined
// by the same author in standard_distribution/unnormalized_pdf
template<typename D>
class standard_distribution {

    typedef typename remove_const<
        typename remove_reference<
            D
        >::type
    >::type dist_t;

    public:
    typedef typename dist_t::value_type      value_type;
    typedef typename dist_t::policy_type     policy_type;
    typedef standard_distribution<dist_t>    base_type;

    standard_distribution(typename call_traits<D>::param_type d):d_(d){}
    standard_distribution(const standard_distribution& that):d_(that.d_){}

    standard_distribution&
    operator=(const standard_distribution& that){
        if(&that!=this){
            d_ = that.d_;
        }
        return *this;
    }

    void operator()(
        const value_type& x,
        value_type& log_pdf,
        value_type& dlog_pdf
    ){
        //math::log_unnormalized_pdf
        log_pdf = log_unnormalized_pdf(
            this->distribution(),x);
            
        dlog_pdf = derivative_log_unnormalized_pdf(
            this->distribution(),x);
    }

    typename call_traits<D>::const_reference distribution()const{
        return d_;
    }

    private:
        typename call_traits<D>::value_type d_;

};

}
}
}
#endif

