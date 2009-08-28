///////////////////////////////////////////////////////////////////////////////
// binary_op::meta::remove_ref.hpp                                           //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_BINARY_OP_META_REMOVE_REF_HPP_ER_2009
#define BOOST_BINARY_OP_META_REMOVE_REF_HPP_ER_2009
#include <boost/type_traits.hpp>
#include <boost/tuple/tuple.hpp>

namespace boost{
namespace binary_op{
        
    template<typename T>
    struct tuple_remove_ref{

        typedef typename tuples::element<0,T>::type first_;
        typedef typename tuples::element<1,T>::type second_;

        typedef tuple<
            typename remove_reference<first_>::type,
            typename remove_reference<second_>::type
        > type;

    };
    
}// binary_op
}// boost
    
#endif