//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_TRAITS_CONTAINER_IS_STATIC_ARRAY_ER_2010_HPP
#define BOOST_ASSIGN_V2_TRAITS_CONTAINER_IS_STATIC_ARRAY_ER_2010_HPP
#include <boost/type_traits/remove_cv.hpp>
#include <boost/assign/v2/detail/traits/container/has_static_size.hpp>
#include <boost/assign/v2/detail/traits/container/forward_to_value.hpp>

namespace boost{

// this here is necessary to avoid compiler error in <boost/array.hpp>
#if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
    template< class T, std::size_t sz >
    class array;
#endif    

namespace assign{
namespace v2{
namespace container_tag{
	struct static_array{};
}// container_tag
namespace container_traits{

	// TODO also check that operator[] exists

	template<typename T>
    struct is_static_array_impl : has_static_size<T>{};

    template<typename V>
    struct is_static_array : forward_to_value<
    	is_static_array_impl,
    	typename boost::remove_cv<V>::type
    >{};

}// container_traits
}// v2
}// assign
}// boost 

#endif
