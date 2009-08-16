///////////////////////////////////////////////////////////////////////////////
// non_param::algorithm::proportion_less_than.hpp                            //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_NON_PARAM_ALGORITHM_PROPORTION_LESS_THAN_HPP_ER_2009
#define BOOST_NON_PARAM_ALGORITHM_PROPORTION_LESS_THAN_HPP_ER_2009
#include <numeric>
#include <functional>
#include <boost/lambda/lambda.hpp>
#include <boost/iterator/iterator_traits.hpp>

namespace boost{
namespace algorithm{

    // Returns the proportions of elements in *[b,e) whose value is less than x
    //
    // Was motivated by the Cook-Gelman validation method
    template<typename InIt>
    typename iterator_value<InIt>::type
    proportion_less_than(
        InIt b,
        InIt e,
        typename iterator_value<InIt>::type x
    ){
        typedef typename iterator_value<InIt>::type value_type;
        value_type m = static_cast<value_type>(
            std::count_if(b, e, (lambda::_1<x))
        );
        value_type n = static_cast<value_type>(
            std::distance(b,e)
        );
        return m / n;
    }

}// algorithm
}// boost
#endif
