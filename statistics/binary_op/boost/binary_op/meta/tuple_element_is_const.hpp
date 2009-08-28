///////////////////////////////////////////////////////////////////////////////
// binary_op::meta::tuple_element_is_const.hpp                               //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_BINARY_OP_META_IS_TUPLE_ELEMENT_IS_CONST_HPP_ER_2009
#define BOOST_BINARY_OP_META_IS_TUPLE_ELEMENT_IS_CONST_HPP_ER_2009
#include <boost/mpl/bool.hpp>
#include <boost/type_traits.hpp>
#include <boost/tuple/tuple.hpp>

namespace boost{
namespace binary_op{
    
    template<unsigned i,typename T>
    struct tuple_element_is_const 
        : is_const<typename tuples::element<i,T>::type>{};

}
}

#endif