//////////////////////////////////////////////////////////////////////////////
// distribution::toolkit::fun_wrap::cdf.hpp                                  //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_FUN_WRAP_CDF_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_FUN_WRAP_CDF_HPP_ER_2009
#include <boost/statistics/detail/distribution_toolkit/fun_wrap/detail/impl.hpp>
#include <boost/statistics/detail/distribution_toolkit/fwd_math/cdf.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace toolkit{
namespace fun_wrap{

    template<typename D> struct cdf_
        : impl<D,statistics::detail::cdf>{};

}// fun_wrap
}// toolkit
}// distribution
}// detail
}// statistics
}// boost

#endif