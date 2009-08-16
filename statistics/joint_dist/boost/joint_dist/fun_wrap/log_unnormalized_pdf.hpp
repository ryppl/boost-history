///////////////////////////////////////////////////////////////////////////////
// joint_dist::fun_wrap::log_unnormalized_pdf.hpp                            //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_JOINT_DIST_FUN_WRAP_LOG_UNNORMALIZED_PDF_ER_2009
#define BOOST_JOINT_DIST_FUN_WRAP_LOG_UNNORMALIZED_PDF_ER_2009
#include <boost/joint_dist/fun_wrap/detail/fun_wrap.hpp>

namespace boost{
namespace joint_dist{

    template<typename D,typename R> 
    typename D::value_type log_unnormalized_pdf(const D& d,const R& x);
    
    namespace fun_wrap{
        template<typename D,typename R> 
        struct log_unnormalized_pdf_
            : detail::fun_wrap<D,R,log_unnormalized_pdf>{};
    }

}// joint_dist
}// boost

#endif
