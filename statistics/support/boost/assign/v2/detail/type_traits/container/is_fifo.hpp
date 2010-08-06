//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_TYPE_TRAITS_CONTAINER_IS_FIFO_ER_2010_HPP
#define BOOST_ASSIGN_V2_TYPE_TRAITS_CONTAINER_IS_FIFO_ER_2010_HPP
#include <boost/mpl/bool.hpp>
#include <boost/assign/v2/detail/fwd/container.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace container_tag{
	struct fifo{};
}// container_tag
namespace container_type_traits{

	template<typename V>
    struct is_fifo : boost::mpl::false_{};
    
    template<typename T,typename A>
    struct is_fifo< std::queue<T,A> > : boost::mpl::true_{};

}// container_type_traits
}// v2
}// assign
}// boost

#endif
