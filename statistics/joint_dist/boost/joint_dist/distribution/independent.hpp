//////////////////////////////////////////////////////////////////////////////
// joint_dist::distribution::independent.hpp                                //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_JOINT_DIST_DISTRIBUTION_INDEPENDENT_HPP_ER_2009
#define BOOST_JOINT_DIST_DISTRIBUTION_INDEPENDENT_HPP_ER_2009
#include <cmath>
#include <vector>
#include <ostream>
#include <numeric>
#include <boost/mpl/bool.hpp>
#include <boost/call_traits.hpp>
#include <boost/range.hpp>
#include <boost/foreach.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/scalar_dist/fun_wrap/log_unnormalized_pdf.hpp>
#include <boost/joint_dist/distribution/detail/dim.hpp>
#include <boost/joint_dist/meta/is_joint_dist.hpp>

namespace boost{
namespace joint_dist{

    // Joint distribution of a collection of indepdendent variables, each from 
    // the same distribution, but with different parameters
    template<typename R>
    class independent : detail::dim{
        public:
        typedef typename remove_reference<R>::type  cv_r_;
        typedef typename remove_cv<cv_r_>::type     elements_type;
        typedef 
            typename range_value<elements_type>::type element_type;

        typedef typename element_type::value_type    value_type;
        typedef typename element_type::policy_type   policy_type;
    
        independent();
        independent(typename call_traits<R>::param_type vec);
        independent(const independent&);
        independent& operator=(const independent& that);

        typename call_traits<R>::const_reference elements()const;
    
        private:
        typename call_traits<R>::value_type distributions_;
    };        

    template<typename D>
    struct is_joint_dist< independent<D> > : mpl::bool_<true> {};

    template<typename R>
    std::ostream& operator<<(std::ostream& out, const independent<R>& d);

    template<typename R,typename R1>
    typename independent<R>::value_type 
    log_unnormalized_pdf(const independent<R>& dist,const R1& x);

    // Implementation //
    
    template<typename R>
    std::ostream& operator<<(std::ostream& out, const independent<R>& d){
        out << "independent"<< '(' << ' ';
        typedef independent<R> indeps_;
        typedef typename indeps_::element_type elem_;
        BOOST_FOREACH(const elem_& elem,d.elements()){
            out << elem << ' ';
        }
        out << ')';
        return out;
    }

    template<typename R>
    independent<R>::independent():distributions_(),dim(){}

    template<typename R>
    independent<R>::independent(typename call_traits<R>::param_type vec)
    :distributions_(vec),dim(boost::size(vec)){}

    template<typename R>
    independent<R>::independent(const independent& that)
    :distributions_(that.distributions_),dim(that){}

    template<typename R>
    independent<R>& 
    independent<R>::operator=(const independent& that){
        if(&that!=this){
            (this->distributions) = that.distributions;
            dim::operator=(that);
        }
        return *this;
    }

    template<typename R>
    typename call_traits<R>::const_reference 
    independent<R>::elements()const{
        return this->distributions_;
    }
    
    template<typename R,typename R1>
    typename independent<R>::value_type
    log_unnormalized_pdf(const independent<R>& dist,const R1& x){
        typedef independent<R>                          indeps_;
        typedef typename indeps_::value_type            val_;
        typedef typename indeps_::element_type     dist_;
        val_ init = static_cast<val_>(0);
        BOOST_ASSERT(
            boost::size(x) == boost::size(dist.elements())
        );
        typedef math::fun_wrap::log_unnormalized_pdf_<dist_> f_;
        val_ log_pdf = std::inner_product(
            boost::begin(dist.elements()),
            boost::end(dist.elements()),
            boost::begin(x),
            init,
            lambda::_1 + lambda::_2,
            lambda::bind(
                f_::instance,
                lambda::_1,
                lambda::_2
            )
        );
        return  log_pdf;
    }

}// joint_dist
}// boost 

#endif
