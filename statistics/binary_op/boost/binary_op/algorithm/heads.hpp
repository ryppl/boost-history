///////////////////////////////////////////////////////////////////////////////
// binary_op::algorithm::heads.hpp                                           //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_BINARY_OP_ALGORITHM_HEADS_HPP_ER_2009
#define BOOST_BINARY_OP_ALGORITHM_HEADS_HPP_ER_2009
#include <algorithm>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/tuple/tuple.hpp>

namespace boost{
namespace binary_op{

    // Extracts the i^th element from each tuple in [b,e)
    template<unsigned i,typename It,typename It1>
    It1 elements(
        It b,       // tuples
        It e,
        It1 io
    );

    template<typename It,typename It1>
    It1 heads(
        It b,       // tuples
        It e,
        It1 io
    );

    template<typename It,typename It1>
    It1 tails(
        It b,       // tuples
        It e,
        It1 io
    );

    // Implementation //

    template<unsigned i,typename It,typename It1>
    It1 elements(
        It b,       // tuples
        It e,
        It1 io
    ){

        typedef typename iterator_value<It>::type tuple_;
        typedef typename add_reference<tuple_>::type ref_tuple_;

        typedef typename boost::tuples::element<i,tuple_>::type elem_; 

        // Why a loop rather than transform? Because creating a
        // delegate function<...(const tuple&)> for get<i> is not
        // straightforward
        for( ; b<e; b++){
            elem_ elem = get<i>(*b);
            *io;
            (*io) = elem;
            ++io;
        }
        return io;

    }

    template<typename It,typename It1>
    It1 heads(
        It b,       // tuples
        It e,
        It1 io
    ){
        return elements<0>(b,e,io);
    }

    template<typename It,typename It1>
    It1 tails(
        It b,       // tuples
        It e,
        It1 io
    ){
        return elements<1>(b,e,io);
    }

}// algorithm
}// boost

#endif