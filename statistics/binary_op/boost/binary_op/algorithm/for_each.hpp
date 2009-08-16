///////////////////////////////////////////////////////////////////////////////
// binary_op::algorithm::for_each.hpp                                        //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_BINARY_OP_ALGORITHM_FOR_EACH_HPP_ER_2009
#define BOOST_BINARY_OP_ALGORITHM_FOR_EACH_HPP_ER_2009
#include <algorithm>
#include <boost/call_traits.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/binary_op/functional/untupler.hpp>

namespace boost{
namespace binary_op{
                    
    // Extends std::for_each to a binary sequence
    template<
        unsigned I0,
        unsigned I1,
        typename F,     
        typename It_0,
        typename It_1
    >
    F for_each(
        It_0 b_0,
        It_0 e_0,
        It_1 b_1,
        F f
    );

    template<
        typename F,
        typename It_0,
        typename It_1
    >
    F for_each(
        It_0 b_0,
        It_0 e_0,
        It_1 b_1,
        F f
    );


    // Implementation //

    template<
        unsigned I0,
        unsigned I1,
        typename F,     
        typename It_0,
        typename It_1
    >
    F for_each(
        It_0 b_0,
        It_0 e_0,
        It_1 b_1,
        F f
    ){
        return std::for_each(
            boost::make_zip_iterator(
                boost::make_tuple(b_0, b_1)
            ),
            boost::make_zip_iterator(
                boost::make_tuple(e_0, next(b_1,std::distance(b_0,e_0)))
            ),
            untupler<F,I0,I1>(f)
        );
    }

    template<
        typename F,
        typename It_0,
        typename It_1
    >
    F for_each(
        It_0 b_0,
        It_0 e_0,
        It_1 b_1,
        F f
    ){
        return for_each<0,1,F,It_0,It_1>(
            b_0, e_0, b_1, f
        );
    }
     
            
}//algorithm
}//boost

#endif