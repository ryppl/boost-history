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
#include <boost/assign/detail/ref_list_of_auto_size_copy_rebind.hpp>

// Same as ref_list_of() but has rebind rather than copy semantics
// Usage:
// BOOST_AUTO(tmp,ref_rebind_list_of(a)(b)(c)); boost::fill(tmp,d);
//
// See detail/ref_list_of_auto_size_copy_rebind.hpp for detail

namespace boost{
namespace assign{

    template<typename T>
    typename cref_list_of_impl::rebind_first<const T>::type
    cref_rebind_list_of(const T& t){
        typedef typename cref_list_of_impl::rebind_first<const T>::type expr_;
        return expr_(t);
    }

    template<typename T>
    typename cref_list_of_impl::rebind_first<T>::type
    ref_rebind_list_of(T& t){
        typedef typename cref_list_of_impl::rebind_first<T>::type expr_;
        return expr_(t);
    }

}// assign
}// boost

#endif
