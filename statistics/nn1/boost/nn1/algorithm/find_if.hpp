///////////////////////////////////////////////////////////////////////////////
// nn1::algorithm::find_if.hpp                                               //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_NN1_ALGORITHM_FIND_IF_HPP_ER_2009
#define BOOST_NN1_ALGORITHM_FIND_IF_HPP_ER_2009
#include <numeric>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/nn1/algorithm/find.hpp>

namespace boost{
namespace nn1{

    // Same as nn1 with the addition of a threhold, t. If the the distance to
    // the nearest neighbour (nn1) is less than t, e is returned
    template<
        template<typename> class F,
        typename It,
        typename X,
        typename T
    >
    It find_if(It b, It e, const X& x, T& m, const T& t){ 
        It it = find<F>(b,e,x,m);
        if(m<t){
            return it;
        }else{
            return e;
        }
    }       

    template<
        template<typename> class F,
        typename It,
        typename X,
        typename T
    >
    It find_if(It b, It e, const X& x,const T& t){ 
        typedef typename iterator_value<It>::type cv_val_;
        typedef typename remove_const<cv_val_>::type val_; 
        T m;
        return find_if<F>(b,e,x,m,t);
    }    

}// nn1
}// boost
#endif