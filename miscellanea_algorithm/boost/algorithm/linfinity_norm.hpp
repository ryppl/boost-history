//////////////////////////////////////////////////////////////////////////////
// linfinity_norm.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_ALGORITHM_LINFINITY_NORM_HPP_ER_2009
#define BOOST_ALGORITHM_LINFINITY_HPP_ER_2009
#include <cmath>
#include <numeric>
#include <boost/range.hpp>
namespace boost{

class linfinity_norm{
    public:
    linfinity_norm(){}

    template<typename R0>
    typename
        iterator_value<typename boost::range_iterator<R0>::type>::type
    operator()(const R0& r0){
        typedef typename
            iterator_value<typename boost::range_iterator<R0>::type>::type
                                                                result_type;
        return std::accumulate(
            boost::begin(r0),
            boost::end(r0),
            (result_type)(0),
            bin_op<result_type>()
        );
    };

    private:
    template<typename T>
    struct bin_op{
        bin_op(){}
        T operator()(T x,T y)const{
            T a = abs(y);
            return (x < a)? a : x;
        }
    };
};

}
#endif
