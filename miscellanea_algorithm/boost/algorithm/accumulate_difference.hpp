//////////////////////////////////////////////////////////////////////////////
// accumulate_difference.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_ALGORITHM_ACCUMULATE_DIFFERENCE_HPP_ER_2009
#define BOOST_ALGORITHM_ACCUMULATE_DIFFERENCE_HPP_ER_2009
#include <numeric>
#include <functional>
#include <boost/range.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/bind.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/range/value_type.hpp>

namespace boost{
/// apply<Binary_outer,T>::type f; f(x,y)
/// apply<Unary_inner,T>::type f; f(x)
template<class Binary_outer,class Unary_inner>
class accumulate_difference{
    public:
    accumulate_difference(){}

    template<typename R0, typename R1>
    typename range_value<R0>::type
    operator()(const R0& r0, const R1& r1)const{
        typedef typename range_value<R0>::type               result_type;
        typedef typename range_value<R1>::type               result1_type;
        typedef typename mpl::apply<Binary_outer,result_type>::type
                                                            outer_type;
        typedef typename mpl::apply<Unary_inner,result_type>::type
                                                            unary_type;
        typedef std::minus<result_type>                     minus_type;

        return std::inner_product(
            begin(r0), end(r0), begin(r1), (result_type)(0),
            outer_type(),
            bind<result_type>(unary_type(),bind(minus_type(),_1,_2))
        );
    }

};


}
#endif
