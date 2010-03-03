//////////////////////////////////////////////////////////////////////////////
// assign::ref_list_of.hpp                                                  //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_AUTO_SIZE_REF_LIST_OF_ER_2010_HPP
#define BOOST_ASSIGN_AUTO_SIZE_REF_LIST_OF_ER_2010_HPP
#include <boost/assign/auto_size/detail/auto_size.hpp>

// Creates a collection of references whose functionality is that of 
// auto_size::array_interface<>. It can be used either as the rhs or lhs 
// of an assignment such as:
// 	boost::fill(ref_list_of(a)(b)(c),0)


namespace boost{
namespace assign{

    template<typename T>
    typename detail::auto_size::first_copy<const T>::type
    cref_list_of(const T& t){
        return detail::auto_size::first_copy<const T>::call(t);
    }

    template<typename T>
    typename detail::auto_size::first_copy<T>::type
    ref_list_of(T& t){
        return detail::auto_size::first_copy<T>::call(t);
    }

}// assign
}// boost

#endif
