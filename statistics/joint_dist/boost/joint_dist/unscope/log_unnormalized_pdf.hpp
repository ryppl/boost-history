//////////////////////////////////////////////////////////////////////////////
// joint_dist::unscope::log_unnormalized_pdf.hpp                            //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_JOINT_DIST_UNSCOPE_LOG_UNNORMALIZED_PDF_HPP_ER_2009
#define BOOST_JOINT_DIST_UNSCOPE_LOG_UNNORMALIZED_PDF_HPP_ER_2009
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/joint_dist/meta/is_joint_dist.hpp>

namespace boost{

    namespace joint_dist{
        template<typename D,typename X>
        typename D::value_type log_unnormalized_pdf(const D& d,const X& x);
    }

    template<typename D,typename X>
    typename boost::lazy_enable_if<
        D,
        mpl::identity<typename D::value_type>
    >
    log_unnormalized_pdf(const D& d,const X& x){
        return joint_dist::log_unnormalized_pdf<D>(d,x);
    }

}

#endif