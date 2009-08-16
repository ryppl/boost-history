///////////////////////////////////////////////////////////////////////////////
// matrix_view::algorithm::transform_column.hpp                              //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_MATRIX_VIEW_ALGORITHM_TRANSFORM_COLUMN_HPP_ER_2009
#define BOOST_MATRIX_VIEW_ALGORITHM_TRANSFORM_COLUMN_HPP_ER_2009
#include <iterator>
#include <boost/utility.hpp>
#include <boost/lambda/lambda.hpp>

namespace boost{
namespace matrix_view{

    // Visits the subsequence defined by a stride and an offset, and transforms
    // it by f. 
    //
    // | b |___|___|___|___|___| ... |___| e | 
    // | 0 |___|___| 1 |___|___| ... |n-1|___| 
    //
    // A k-step iterator + algorithm would be more general, but until then...
    template<unsigned k,typename F,typename It,typename ItO>
    ItO transform_column(
        It b,
        It e,
        unsigned offset,
        F f, 
        ItO i_o
    ){
        typedef typename iterator_difference<It>::type diff_;
        diff_ d = std::distance(b,e);
        BOOST_ASSERT(d>=0);
        if(d<offset){
            return i_o;
        }
        b = next(b,offset);
        diff_ n = d / k;
        i_o = f(*b);                         
        for(unsigned i = 1; i<n; ++i){
            std::advance(b,k);            
            i_o = f(*b);
            ++i_o;
        }
        return i_o;
    }


    template<unsigned k,typename It,typename ItO>
    ItO copy_column(
        It b,
        It e,
        unsigned offset,
        ItO i_o
    ){
        return transform_column<k>(b, e, offset, boost::lambda::_1, i_o);
    }

}// algorithm
}// boost

#endif
