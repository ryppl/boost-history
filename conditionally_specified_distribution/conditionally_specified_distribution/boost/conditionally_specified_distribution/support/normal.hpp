/////////////////////////////////////////////////////////////////////////////
// normal.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_SUPPORT_NORMAL_HPP_ER_2009
#define BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_SUPPORT_NORMAL_HPP_ER_2009
#include <boost/utility/dont_care.hpp>
#include <boost/conditionally_specified_distribution/result_of/function_support.hpp>
#include <boost/conditionally_specified_distribution/crtp/normal.hpp>
#include <boost/conditionally_specified_distribution/support/detail/negative_max_to_max.hpp>
namespace boost{
namespace conditionally_specified_distribution{
namespace function{

template <typename D>
inline
typename function_support<D>::type
support(const crtp::normal<D>& d)
{
    const D& d_ = static_cast<const D&>(d);

    return detail::support_negative_max_to_max(d_);
}

template <typename D>
inline
typename function_support<D>::type
support(const crtp::normal<D>& d,utility::dont_care)
{
    return support(d);
}

}
}
}
#endif // BOOST_DISTRIBUTION_SUPPORT_NORMAL_HPP_ER_2009
