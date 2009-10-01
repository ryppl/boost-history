/////////////////////////////////////////////////////////////////////////////////
// distribution_toolkit::distributions::wrapper::normalizing_constant.hpp      //
//                                                                             //
//  (C) Copyright 2009 Erwann Rogard                                           //
//  Use, modification and distribution are subject to the                      //
//  Boost Software License, Version 1.0. (See accompanying file                //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)           //
/////////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_DISTRIBUTIONS_WRAPPER_NORMALIZING_CONSTANT_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_DISTRIBUTIONS_WRAPPER_NORMALIZING_CONSTANT_HPP_ER_2009
#include <boost/concept/assert.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/value.hpp>
#include <boost/statistics/detail/distribution_toolkit/concept/unary/normalizing_constant.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/wrapper/wrapper.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution_toolkit{

    template<typename D>
    typename meta::value< wrapper<D> >::type
    normalizing_constant(
        const wrapper<D>& w
    ){
        typedef wrapper<D> w_;
        typedef typename w_::distribution_type d_;
        BOOST_CONCEPT_ASSERT((
            concept::NormalizingConstant<d_>
        ));

        return normalizing_constant(w.const_reference());
    }

}// distribution_toolkit
}// detail
}// statistics
}// boost

#endif