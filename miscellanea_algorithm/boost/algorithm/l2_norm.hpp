//////////////////////////////////////////////////////////////////////////////
// l2_norm.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_ALGORITHM_L2_NORM_HPP_ER_2009
#define BOOST_ALGORITHM_L2_NORM_HPP_ER_2009
#include <numeric>
#include <cmath>
#include <boost/range.hpp>
#include <boost/range/value_type.hpp>
namespace boost{

class l2_norm{
    public:
    l2_norm(){}
    template<typename R0>
    typename range_value<R0>::type
    operator()(const R0& r0){
        typedef typename range_value<R0>::type result_type;
        return std::sqrt(std::accumulate(
            boost::begin(r0),
            boost::end(r0),
            (result_type)(0),
            bin_op<result_type>()
        ));
    };

    private:

    template<typename T>
    struct bin_op{
        bin_op(){}
        T operator()(T x,T y)const{ return x + y*y; }
    };
};

}
#endif
