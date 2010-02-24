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

// Usage 1: 
// 	std::vector<T> elems = cref_list_of(a)(b)(c);
// Usage 2: 
// 	boost::array<T,3> elems = cref_list_of(a)(b)(c);
// Usage 3: 
// BOOST_AUTO(tmp,ref_list_of(a)(b)(c)); boost::fill(tmp,d);
//    
// This function has the same side effect as ref_list_of<int>(), but supersedes
// it as it deduces the number of elements, here 3, from the arguments. 
//
// Usage 3 binds d to a, b, c. Unless this specific feature is needed, function 
// ref_copy_list_of() is more straightforward.

namespace boost{
namespace assign{

    template<typename T>
    typename detail::auto_size::rebind_first<const T>::type
    cref_list_of(const T& t){
        typedef typename detail::auto_size::rebind_first<const T>::type expr_;
        return expr_(t);
    }

    template<typename T>
    typename detail::auto_size::rebind_first<T>::type
    ref_list_of(T& t){
        typedef typename detail::auto_size::rebind_first<T>::type expr_;
        return expr_(t);
    }

}// assign
}// boost

#endif
