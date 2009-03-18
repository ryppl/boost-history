/////////////////////////////////////////////////////////////////////////////
// negative_max_to_max.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_SUPPORT_FROM_NEGATIVE_MAX_TO_MAX_HPP_ER_2009
#define BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_SUPPORT_FROM_NEGATIVE_MAX_TO_MAX_HPP_ER_2009
#include <utility>
#include <boost/conditionally_specified_distribution/result_of/function_support.hpp>
#include <boost/math/tools/precision.hpp>
namespace boost{
namespace conditionally_specified_distribution{
namespace function{
namespace detail{

    template<typename D>
    inline
    typename function_support<D>::type
    support_negative_max_to_max(
        const D& d
    ){
        typedef typename function_support<D>::type fs;

        using boost::math::tools::max_value;
        return fs(
            -max_value<typename fs::first_type>(),
            max_value<typename fs::second_type>()
        );
    };

}//detail
}//function
}//conditionally_specified_distribution
}//boost


#endif // BOOST_DISTRIBUTION_SUPPORT_FROM_NEGATIVE_MAX_TO_MAX_HPP_ER_2009
