///////////////////////////////////////////////////////////////////////////////
// binary_op::meta::remove_ref_cv.hpp                                        //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_BINARY_OP_META_REMOVE_REF_CV_HPP_ER_2009
#define BOOST_BINARY_OP_META_REMOVE_REF_CV_HPP_ER_2009
#include <boost/binary_op/meta/tuple_remove_cv.hpp>
#include <boost/binary_op/meta/tuple_remove_ref.hpp>

namespace boost{
namespace binary_op{
            
    template<typename T>
    struct tuple_remove_ref_cv : tuple_remove_cv<
        typename tuple_remove_ref<T>::type
    >{};
    
}// binary_op
}// boost
    
#endif
    