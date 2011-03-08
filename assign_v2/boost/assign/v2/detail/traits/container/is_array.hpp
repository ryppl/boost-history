//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_TRAITS_CONTAINER_IS_ARRAY_ER_2010_HPP
#define BOOST_ASSIGN_V2_TRAITS_CONTAINER_IS_ARRAY_ER_2010_HPP
#include <boost/assign/v2/detail/traits/container/is.hpp>

namespace boost{

#if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
    template< class T, std::size_t sz >
    class array;
#endif

namespace assign{
namespace v2{
namespace container_tag{
    struct array{};
}// container_tag
namespace container_traits_aux{

    template<typename V> struct is_array : ::boost::mpl::false_{};
    template<typename T, std::size_t sz> struct is_array<
        boost::array<T, sz>
    > : ::boost::mpl::true_{};

}// containert_type_aux

    BOOST_ASSIGN_V2_TRAITS_CONTAINER_IS(array)

}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_TRAITS_CONTAINER_IS_ARRAY_ER_2010_HPP
