///////////////////////////////////////////////////////////////////////////////
// distribution::joint::fun_wrap::log_unnormalized_pdf.hpp                   //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_FUN_WRAP_LOG_UNNORMALIZED_PDF_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_FUN_WRAP_LOG_UNNORMALIZED_PDF_ER_2009
#include <boost/statistics/detail/distribution/common/meta/value.hpp>
#include <boost/statistics/detail/distribution/joint/fun_wrap/detail/fun_wrap.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace joint{

    namespace fun_wrap{
        template<typename D,typename R> 
        struct log_unnormalized_pdf_
            : detail::fun_wrap<D,R,statistics::detail::log_unnormalized_pdf>{};
    }

}// joint
}// distribution
}// detail
}// statistics
}// boost

#endif
