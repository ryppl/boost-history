///////////////////////////////////////////////////////////////////////////////
// binary_op::algorithm::tuples.hpp                                          //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_BINARY_OP_ALGORITHM_TUPLES_HPP_ER_2009
#define BOOST_BINARY_OP_ALGORITHM_TUPLES_HPP_ER_2009
#include <algorithm>
#include <boost/utility.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/iterator/zip_iterator.hpp>

namespace boost{
namespace binary_op{

    
    // Writes to b_t a sequence of tuples formed by combining the sequences
    // [b_1,e_1) and [b_2,e2)
    template<typename It1,typename It2,typename ItT>
    ItT tuples(
        It1 b_1,
        It1 e_1,
        It2 b_2,
        ItT b_t
    );

    // Implementation //

    template<typename It1,typename It2,typename ItT>
    ItT tuples(
        It1 b_1,
        It1 e_1,
        It2 b_2,
        ItT b_t
    )
    {
        typedef typename iterator_difference<It1>::type diff_1_;
        typedef typename iterator_difference<It2>::type diff_2_;
        diff_1_ diff_1 = std::distance(b_1,e_1);
        diff_2_ diff_2 = static_cast<diff_2_>(diff_1);
        It2 e_2 = boost::next(b_2,diff_2);
        return std::transform(
            make_zip_iterator( make_tuple( b_1, b_2 ) ),
            make_zip_iterator( make_tuple( e_1, e_2 ) ),
            b_t
        );    
    }
    
}// algorithm
}// boost

#endif