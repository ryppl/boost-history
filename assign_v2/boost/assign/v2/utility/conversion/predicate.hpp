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
#include <boost/assign/v2/detail/traits/value_container/category.hpp>
#include <boost/assign/v2/detail/traits/value_container/has_push.hpp>
#include <boost/assign/v2/detail/traits/ptr_container/meta.hpp>
#include <boost/mpl/or.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace conversion_aux{

	template<typename C>
    struct is_array : ::boost::mpl::apply1<
    	ptr_container_aux::through_value_container<
        	value_container_aux::is_array
        >,
		C
    >{};

	template<typename C>
    struct has_push : ::boost::mpl::apply1<
    	ptr_container_aux::through_value_container<
        	value_container_aux::has_push_deduced_value
        >,
		C
    >{};

    template<typename C, typename R>
    struct use_put : ::boost::mpl::or_<
    	is_array<C>,
        has_push<C>
    >{};
    
}// conversion_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_UTILITY_CONVERSION_TRAITS_ER_2010_HPP
