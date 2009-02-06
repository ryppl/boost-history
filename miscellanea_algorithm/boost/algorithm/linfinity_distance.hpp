//////////////////////////////////////////////////////////////////////////////
// linfinity_distance.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_ALGORITHM_LINFINITY_DISTANCE_HPP_ER_2009
#define BOOST_ALGORITHM_LINFINITY_DISTANCE_HPP_ER_2009
#include <functional>
#include <boost/algorithm/functor_max.hpp>
#include <boost/algorithm/functor_abs.hpp>
#include <boost/algorithm/accumulate_difference.hpp>
namespace boost{

struct linfinity_distance{
    typedef functor_max<mpl::_1>   max_type;
    typedef functor_abs<mpl::_1>   abs_type;
    typedef accumulate_difference<max_type,abs_type> type;
};


}
#endif
