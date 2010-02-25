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

// This function supersedes ref_list_of<int>() as it deduces the number of 
// elements from the arguments but also uses copy rather than rebind semantics,
// if used as the lhs of an assignement :
// 	boost::fill(ref_list_of(a)(b)(c),0)
// assigns 0 to a,b,c.

namespace boost{
namespace assign{

    template<typename T>
    typename detail::auto_size::copy_first<const T>::type
    cref_list_of(const T& t){
        typedef typename detail::auto_size::copy_first<const T>::type expr_;
        return expr_(t);
    }

    template<typename T>
    typename detail::auto_size::copy_first<T>::type
    ref_list_of(T& t){
        typedef typename detail::auto_size::copy_first<T>::type expr_;
        return expr_(t);
    }

}// assign
}// boost

#endif
