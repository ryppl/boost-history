//////////////////////////////////////////////////////////////////////////////
// joint_dist::distribution::iid.hpp                                        //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_JOINT_DIST_DISTRIBUTION_IID_HPP_ER_2009
#define BOOST_JOINT_DIST_DISTRIBUTION_IID_HPP_ER_2009
#include <cmath>
#include <stdexcept>
#include <numeric>
#include <functional>
#include <ostream>
#include <boost/mpl/bool.hpp>
#include <boost/range.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/format.hpp>
#include <boost/scalar_dist/fun_wrap/log_unnormalized_pdf.hpp>
#include <boost/scalar_dist/iterator/distribution_function.hpp>
#include <boost/joint_dist/distribution/detail/dim.hpp>
#include <boost/joint_dist/meta/is_joint_dist.hpp>

namespace boost{
namespace joint_dist{

    // An iid distribution
    template<typename D>
    class iid : D, public detail::dim{
        public:
        typedef typename D::value_type                  value_type;
        typedef typename D::policy_type                 policy_type;
        typedef D                                       element_type;
    
        iid();
        iid(const D& d,size_type dim);
        iid(const iid& that);
        iid& operator=(const iid& that);

        const element_type& element()const;

    };

    template<typename D>
    struct is_joint_dist< iid<D> > : mpl::bool_<true> {};

    template<typename D>    
    std::ostream& operator<<(std::ostream& out, const iid<D>& that);

    template<typename D,typename R>
    typename D::value_type log_unnormalized_pdf(
        const iid<D>& dist,
        const R& sample
    );

    template<typename D,typename R>
    typename D::value_type pdf(
        const iid<D>& dist,
        const R& sample
    );

    // Implementation //

    template<typename D>
    const typename iid<D>::element_type& 
    iid<D>::element()const{ 
        return static_cast<const D&>(*this);
    }

    template<typename D> iid<D>::iid() : D(), dim(0){}

    template<typename D> 
    iid<D>::iid(const D& d,size_type n):D(d),dim(n){}

    template<typename D> iid<D>::iid(const iid& that):D(that),dim(that){}

    template<typename D> iid<D>& 
    iid<D>::operator=(const iid& that){
        if(&that!=this){
            D::operator=(that);
            dim::operator=(that);
        }
        return *this;
    }

    template<typename D>
    std::ostream& operator<<(std::ostream& out, const iid<D>& that){
        static const char* msg = "iid{%1%:i=0,...,%2%}";
        return (out<<
            ( 
                format(msg) % that.element() % that.dimension() 
            ).str()
        );
    }

    template<typename D,typename R>
    typename D::value_type log_unnormalized_pdf(const iid<D>& dist,const R& x){
        const char* msg = "log_unnormalized_pdf(%1%) : size(x) = != dim = %2%";
        if( boost::size(x) != dist.dimension() ){
            throw std::runtime_error(
                (format(msg)%size(x)%dist.dimension()).str()
            );
        }
        typedef iid<D> iid_;
        typedef typename iid_::value_type val_;
        val_ init = static_cast<val_>(0);
        val_ log_pdf 
            = std::accumulate(
                math::make_distribution_function_iterator<
                    math::fun_wrap::log_unnormalized_pdf_
                >(dist.element(),boost::begin(x)),
                math::make_distribution_function_iterator<
                    math::fun_wrap::log_unnormalized_pdf_
                >(dist.element(),boost::end(x)),
                init
            );

        return log_pdf;
    };

}// joint_dist
}// boost 

#endif
