//////////////////////////////////////////////////////////////////////////////
// assign::ref_rebind_list_of.hpp                                           //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_AUTO_SIZE_REF_REBIND_LIST_OF_ER_2010_HPP
#define BOOST_ASSIGN_AUTO_SIZE_REF_REBIND_LIST_OF_ER_2010_HPP
#include <boost/assign/auto_size/detail/auto_size.hpp>

// Creates a collection of references exposing the boost::array interface and 
// convertible to a range that is constructible from a pair of iterators. Rebind
// semantics apply if the collection is the lhs of an assignment:
// 	cref_rebind_list_of(a,b,c).assign(d)
// Unless this specific feature is needed, ref_list_of() is preferable.
// Note that this function supersedes ref_list_of<int>().

namespace boost{
namespace assign{

    template<typename T>
    typename detail::auto_size::first_rebind<const T>::type
    cref_rebind_list_of(const T& t){
		return detail::auto_size::first_rebind<const T>::call(t);
    }

    template<typename T>
    typename detail::auto_size::first_rebind<T>::type
    ref_rebind_list_of(T& t){
		return detail::auto_size::first_rebind<T>::call(t);
    }

}// assign
}// boost

#define BOOST_ASSIGN_CSV_ref boost::assign::detail::auto_size::ref_rebind
#include <boost/assign/auto_size/detail/csv.hpp>
BOOST_ASSIGN_CSV(ref_rebind_list_of)
#undef BOOST_ASSIGN_CSV_ref

#endif
