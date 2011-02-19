//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CONVERT_TRAITS_ER_2010_HPP
#define BOOST_ASSIGN_V2_CONVERT_TRAITS_ER_2010_HPP
#include <boost/mpl/or.hpp>
#include <boost/assign/v2/detail/traits/container/is_array.hpp>
#include <boost/assign/v2/detail/traits/container/has_push.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace convert_aux{

    template<typename T, typename U>
    struct use_put : ::boost::mpl::or_<
        v2::container_traits::is_array<T>,
        v2::container_traits::has_push<T>
    >{};

}// convert_aux
}// v2
}// assign
}// boost

#endif
