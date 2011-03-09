//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_UTILITY_CONVERSION_TRAITS_ER_2010_HPP
#define BOOST_ASSIGN_V2_UTILITY_CONVERSION_TRAITS_ER_2010_HPP
#include <boost/assign/v2/detail/traits/container/is_array.hpp>
#include <boost/assign/v2/detail/traits/container/has_push.hpp>
#include <boost/mpl/or.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace convert_aux{

    template<typename C, typename R>
    struct use_put : ::boost::mpl::or_<
        v2::container_traits::is_array<C>,
        v2::container_traits::has_push<C>
    >{};

}// convert_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_UTILITY_CONVERSION_TRAITS_ER_2010_HPP
