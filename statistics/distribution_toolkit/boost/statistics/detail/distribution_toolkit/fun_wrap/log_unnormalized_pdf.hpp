//////////////////////////////////////////////////////////////////////////////
// distribution_toolkit::fun_wrap::detail::log_unnormalized_pdf.hpp         //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_FUN_WRAP_LOG_UNNORMALIZED_PDF_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_FUN_WRAP_LOG_UNNORMALIZED_PDF_HPP_ER_2009
#include <boost/statistics/detail/distribution_toolkit/fun_wrap/detail/impl.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution_toolkit{

    template<typename T,typename D> 
    T log_unnormalized_pdf(const D& d,const T& x);

namespace fun_wrap{    
    
    template<typename D>
    struct log_unnormalized_pdf_ 
            : impl<D,log_unnormalized_pdf>{};

}// fun_wrap
}// distribution_toolkit
}// detail
}// statistics
}// boost

#endif
