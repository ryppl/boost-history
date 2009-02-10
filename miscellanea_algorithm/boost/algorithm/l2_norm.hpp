//////////////////////////////////////////////////////////////////////////////
// l2_norm.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_ALGORITHM_L2_NORM_HPP_ER_2009
#define BOOST_ALGORITHM_L2_NORM_HPP_ER_2009
#include <cmath>
#include <boost/range.hpp>
#include <boost/range/value_type.hpp>
#include <boost/algorithm/l2_norm_squared.hpp>
namespace boost{

class l2_norm : l2_norm_squared{//EBO
    typedef l2_norm_squared super_t;
    public:
    l2_norm():super_t(){}
    template<typename R0>
    typename range_value<R0>::type
    operator()(const R0& r0)const{
        typedef typename range_value<R0>::type result_type;
        const super_t& ref = static_cast<const super_t&>(*this);
        result_type sq = ref(r0);
        return std::sqrt(sq);
    };

};

}
#endif
