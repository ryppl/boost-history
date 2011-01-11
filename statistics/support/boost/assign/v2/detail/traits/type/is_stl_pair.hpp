//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_TRAITS_TYPE_IS_STL_PAIR_ER_2010_HPP
#define BOOST_ASSIGN_V2_TRAITS_TYPE_IS_STL_PAIR_ER_2010_HPP
#include <utility>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/detail/yes_no_type.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace type_traits{

//    template< class T, class U >
//    boost::type_traits::yes_type is_pair( const std::pair<T,U>* );
//    boost::type_traits::no_type is_pair( ... ); 

    template<typename T>
    struct is_stl_pair : boost::mpl::false_{};
    
    template<typename T,typename U>
    struct is_stl_pair< std::pair<T, U> > : boost::mpl::true_{};

}// type_traits
}// v2
}// assign
}// boost 

#endif
