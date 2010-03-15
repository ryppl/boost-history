//////////////////////////////////////////////////////////////////////////////
// assign::detail::convert_iterator.hpp                                     //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_CONVERT_ITERATOR_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_CONVERT_ITERATOR_ER_2010_HPP
#include <boost/type_traits/remove_reference.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/iterator/iterator_categories.hpp>
#include <boost/iterator/iterator_traits.hpp>

namespace boost{
namespace assign{
namespace detail{

template<typename I,typename T>
struct iterator_converter : boost::iterator_adaptor<
    detail::iterator_converter<I,T>             // Derived
    ,I                                          // Base
    ,typename boost::remove_reference<T>::type  // Value
    ,use_default                                // CategoryOrTraversal
    ,T                                          // Reference
    ,use_default                                // Difference
>{

    typedef boost::iterator_adaptor<
        detail::iterator_converter<I,T> 				
        ,I                         						
        ,typename boost::remove_reference<T>::type 		
        ,use_default 
        ,T 												
        ,use_default 
    > super_;
    
    iterator_converter(){}
    iterator_converter(const I& base):super_(base){}
}; 
namespace result_of{

    template<typename T,typename I>
    struct convert_iterator{
        typedef detail::iterator_converter<I,T> type;
    };

}

template<typename T,typename I>
typename result_of::convert_iterator<T,I>::type
convert_iterator(const I& i){
    typedef typename result_of::convert_iterator<T,I>::type result_;
    return result_(i);
}
} //detail
}// assign
}// boost

#endif
