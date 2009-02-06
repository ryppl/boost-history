//////////////////////////////////////////////////////////////////////////////
// l2_distance_squared.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_ALGORITHM_L2_DISTANCE_SQUARED_HPP_ER_2009
#define BOOST_ALGORITHM_L2_DISTANCE_SQUARED_HPP_ER_2009
#include <functional>
#include <boost/mpl/placeholders.hpp>
#include <boost/algorithm/functor_square.hpp>
#include <boost/algorithm/accumulate_difference.hpp>
namespace boost{

struct l2_distance_squared{
    typedef std::plus<mpl::_1>                                  plus_type;
    typedef functor_square<mpl::_1>                             square_type;

    typedef accumulate_difference<plus_type, square_type>       type;
};

}
#endif
