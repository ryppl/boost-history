//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_ARRAY_ALLOC_INSTANT_FWD_ER_2011_HPP
#define BOOST_ASSIGN_V2_REF_ARRAY_ALLOC_INSTANT_FWD_ER_2011_HPP
#include <boost/assign/v2/ref/array/size_type.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace alloc_tag{

    struct instant_alloc{};

}// alloc_tag
namespace array_aux{

   template<size_type N, typename T> class instant_alloc;

}// array_aux
}// ref
}// v2
}// assign
}// boost

#endif
