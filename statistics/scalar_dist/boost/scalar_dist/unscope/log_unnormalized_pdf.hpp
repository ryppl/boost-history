//////////////////////////////////////////////////////////////////////////////
// scalar_dist::unscope::log_unnormalized_pdf.hpp                           //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SCALAR_DIST_UNSCOPE_LOG_UNNORMALIZED_PDF_HPP_ER_2009
#define BOOST_SCALAR_DIST_UNSCOPE_LOG_UNNORMALIZED_PDF_HPP_ER_2009
#include <boost/utility/enable_if.hpp>
#include <boost/scalar_dist/meta/is_scalar_dist.hpp>

namespace boost{

    namespace math{
        template<typename D,typename T> 
        T log_unnormalized_pdf(const D& d,const T& x);
    }

    template<typename D,typename T>
    typename boost::enable_if<
        math::is_scalar_dist<D>,
        T 
    >
    log_unnormalized_pdf(const D& d,const T& x){
        return math::log_unnormalized_pdf<D>(d,x);
    }

}

#endif
