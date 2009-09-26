/////////////////////////////////////////////////////////////////////////////////
// distribution_toolkit::distributions::wrapper::is_pseudo_scalar_distribution.hpp //
//                                                                             //
//  (C) Copyright 2009 Erwann Rogard                                           //
//  Use, modification and distribution are subject to the                      //
//  Boost Software License, Version 1.0. (See accompanying file                //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)           //
/////////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_DISTRIBUTIONS_WRAPPER_IS_PSEUDO_SCALAR_DISTRIBUTION_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_DISTRIBUTIONS_WRAPPER_IS_PSEUDO_SCALAR_DISTRIBUTION_HPP_ER_2009
#include <boost/statistics/detail/distribution_toolkit/meta/is_pseudo_scalar_distribution.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/wrapper/wrapper.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution_toolkit{
namespace meta{

  template<typename D>
  struct is_pseudo_scalar_distribution< wrapper<D> >
  {
  
    typedef wrapper<D> w_;
    typedef typename w_::distribution_type d_;
    typedef typename is_pseudo_scalar_distribution<d_>::type type;
  
  };

}// meta
}// distribution_toolkit
}// detail
}// statistics
}// boost

#endif