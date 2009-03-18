/////////////////////////////////////////////////////////////////////////////
// function_support.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_RESULT_OF_SUPPORT_HPP_ER_2009
#define BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_RESULT_OF_SUPPORT_HPP_ER_2009
#include <utility>
#include <boost/mpl/identity.hpp>
#include <boost/utility/remove_qualifier.hpp>
#include <boost/conditionally_specified_distribution/result_of/function_argument.hpp>
namespace boost{namespace conditionally_specified_distribution{

    template<typename D>
    struct function_support : mpl::identity<
            std::pair<
                typename function_argument<
                    typename utility::remove_qualifier<D>::type>::type,
                typename function_argument<
                    typename utility::remove_qualifier<D>::type>::type
            >
        >
    {};


}}

#endif // BOOST_DISTRIBUTION_RESULT_OF_SUPPORT_HPP_ER_2009
