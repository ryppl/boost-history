///////////////////////////////////////////////////////////////////////////////
// nn1::algorithm::find.hpp                                                  //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_NN1_ALGORITHM_FIND_HPP_ER_2009
#define BOOST_NN1_ALGORITHM_FIND_HPP_ER_2009
#include <numeric>
#include <boost/type_traits.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/mpl/nested_type.hpp>
#include <boost/nn1/algorithm/min_element.hpp>

namespace boost{
namespace nn1{

    // Requirements:
    // Let F_x = F<X>, Y an abitrary type, 
    // F_x f(x);    
    // f(y)           returns the distance from x to y.
    //
    // Operations:
    // find(b,e,x,m) returns the nn1 to x in [b,e) and its distance
    // is put in m.
    template<
        template<typename> class F,
        typename It,
        typename X,
        typename T
    >
    It find(It b, It e, const X& x, T& m){ 
        typedef typename boost::iterator_value<It>::type value_t;
        
        typedef F<X> arg_;
        typedef typename boost::mpl::nested_type<arg_>::type dist_;
        dist_ dist(x);
    
        return nn1::min_element(
            make_transform_iterator(b,dist),
            make_transform_iterator(e,dist),
            m
        ).base();
    }

    template<
        template<typename> class F,
        typename It,
        typename V
    >
    It find(It b, It e, const V& x){ 
        typedef typename iterator_value<It>::type const_val_;
        typedef typename remove_const<const_val_>::type val_; 
        val_ m;
        return find<F>(b,e,x,m);
    }

}// nn1
}// boost
#endif