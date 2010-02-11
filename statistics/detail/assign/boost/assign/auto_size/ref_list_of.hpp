//////////////////////////////////////////////////////////////////////////////
// assign::cref_list_of_rebind.hpp                               			//
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_REF_LIST_OF_REBIND_ER_2010_HPP
#define BOOST_ASSIGN_REF_LIST_OF_REBIND_ER_2010_HPP
#include <boost/assign/auto_size/detail/auto_size.hpp>

// Usage 1: 
// 	std::vector<T> vec = cref_list_of(a)(b)(c);
// Usage 2: 
// 	boost::array<T,3> vec = cref_list_of(a)(b)(c);
// Usage 2: 
// BOOST_AUTO(tmp,ref_list_of(a)(b)(c)); boost::fill(tmp,d);
//    
// Note that this function supersedes and has the same side effect as 
// ref_list_of<int>(), which still exists only for backward-compatibility.
//
// Unlike ref_copy_list_of, it has rebind semantics so that Usage 3 binds d to
// a, b, c. If in doubt, ref_copy_list_of is the safer option.

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
