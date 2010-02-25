//////////////////////////////////////////////////////////////////////////////
// assign::ref_copy_list_of.hpp                                             //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_AUTO_SIZE_REF_COPY_LIST_OF_ER_2010_HPP
#define BOOST_ASSIGN_AUTO_SIZE_REF_COPY_LIST_OF_ER_2010_HPP
#include <boost/assign/auto_size/detail/auto_size.hpp>

// Usage 1: 
// 	std::vector<T> elems = cref_copy_list_of(a)(b)(c);
// Usage 2: 
// 	boost::array<T,3> elems = cref_copy_list_of(a)(b)(c);
// Usage 3: 
// 	boost::fill( ref_list_of(a)(b)(c), 0);
//    
// The infix 'copy' emphasizes that copy semantics are used, which is the 
// preferable choice in most situations, in constrast to ref_list_of() which 
// has rebind semantics. 

namespace boost{
namespace assign{

    template<typename T>
    typename detail::auto_size::copy_first<const T>::type
    cref_copy_list_of(const T& t){
        typedef typename detail::auto_size::copy_first<const T>::type expr_;
        return expr_(t);
    }

    template<typename T>
    typename detail::auto_size::copy_first<T>::type
    ref_copy_list_of(T& t){
        typedef typename detail::auto_size::copy_first<T>::type expr_;
        return expr_(t);
    }

}// assign
}// boost

#endif
