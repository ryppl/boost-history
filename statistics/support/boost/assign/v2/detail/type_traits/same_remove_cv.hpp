//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_TYPE_TRAITS_SAME_REMOVE_CV_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_TYPE_TRAITS_SAME_REMOVE_CV_ER_2010_HPP
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_cv.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace type_traits{

	template<typename T,typename U>
	struct same_remove_cv : boost::is_same<
    	typename boost::remove_cv<T>::type,
        typename boost::remove_cv<U>::type
    >{};

}// type_traits
}// v2
}// assign
}// boost

#endif

