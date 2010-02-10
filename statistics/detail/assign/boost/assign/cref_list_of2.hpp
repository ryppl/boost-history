//////////////////////////////////////////////////////////////////////////////
// assign::ref_list_of_copy.hpp                               				//
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_CREF_LIST_OF_COPY_ER_2010_HPP
#define BOOST_ASSIGN_CREF_LIST_OF_COPY_ER_2010_HPP
#include <boost/assign/detail/ref_list_of_auto_size_copy_rebind.hpp>

// This is a variation on Boost.Assign's ref_list_of<> that deduces the number 
// of items and has copy semantics.
// Usage 1: 
// 	std::vector<T> vec = cref_list_of(a)(b)(c);
// Usage 2: 
// 	boost::array<T,3> vec = cref_list_of(a)(b)(c);
// Usage 3: 
// 	boost::fill( cref_list_of(a)(b)(c), 0);
//    
// See detail/ref_list_of_auto_size_copy_rebind.hpp for detail

namespace boost{
namespace assign{

    template<typename T>
    typename cref_list_of_impl::first<const T>::type
    cref_list_of(const T& t){
        typedef typename cref_list_of_impl::first<const T>::type expr_;
        return expr_(t);
    }

    template<typename T>
    typename cref_list_of_impl::first<T>::type
    ref_list_of(T& t){
        typedef typename cref_list_of_impl::first<T>::type expr_;
        return expr_(t);
    }

}// assign
}// boost

#endif
