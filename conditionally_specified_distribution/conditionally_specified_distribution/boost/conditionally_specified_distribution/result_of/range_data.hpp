/////////////////////////////////////////////////////////////////////////////
// range_data.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_RESULT_OF_RANGE_DATA_HPP_ER_2009
#define BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_RESULT_OF_RANGE_DATA_HPP_ER_2009
#include <boost/utility/remove_qualifier.hpp>

namespace boost{
namespace conditionally_specified_distribution{
namespace result_of{

    template<typename F>
    struct range_data
    : utility::remove_qualifier<F>::type::result_of_range_data
    {};

}
}
}


#endif // BOOST_DISTRIBUTION_RESULT_OF_DATASET_HPP_ER_2009
