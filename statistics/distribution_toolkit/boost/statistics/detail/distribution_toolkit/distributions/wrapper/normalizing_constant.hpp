/////////////////////////////////////////////////////////////////////////////////
// distribution_toolkit::distributions::wrapper::normalizing_constant.hpp      //
//                                                                             //
//  (C) Copyright 2009 Erwann Rogard                                           //
//  Use, modification and distribution are subject to the                      //
//  Boost Software License, Version 1.0. (See accompanying file                //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)           //
/////////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_WRAPPER_NORMALIZING_CONSTANT_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_WRAPPER_NORMALIZING_CONSTANT_HPP_ER_2009
#include <boost/concept/assert.hpp>
#include <boost/statistics/detail/distribution_common/meta/value.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/wrapper/wrapper.hpp>

namespace boost{
namespace statistics{
namespace detail{

    template<typename D>
    typename distribution::common::meta::value< 
        distribution_toolkit::wrapper<D> 
    >::type
    normalizing_constant(
        const distribution_toolkit::wrapper<D>& w
    ){
        return statistics::detail::normalizing_constant(w.const_reference());
    }

}// detail
}// statistics
}// boost

#endif