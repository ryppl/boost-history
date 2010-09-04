//  common_type.hpp  ---------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_TYPE_TRAITS_EXT_ADD_RVALUE_REFERENCE__HPP
#define BOOST_TYPE_TRAITS_EXT_ADD_RVALUE_REFERENCE__HPP

#include <boost/config.hpp>

//----------------------------------------------------------------------------//

#include <boost/type_traits/is_void.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/is_pointer.hpp>

//----------------------------------------------------------------------------//
//                                                                            //
//                           C++03 implementation of                          //
//             20.7.6.2 Reference modifications [meta.trans.ref]              //
//                          Written by Vicente J. Botet Escriba               //
//                                                                            //
// If T names an object or function type then the member typedef type
// shall name T&&; otherwise, type shall name T. [ Note: This rule reflects
// the semantics of reference collapsing. For example, when a type T names
// a type T1&, the type add_rvalue_reference<T>::type is not an rvalue
// reference. —end note ]
//----------------------------------------------------------------------------//

namespace boost {

namespace type_traits_detail {

    template <typename T,
        bool = !is_reference<T>::value 
                && ! is_pointer<T>::value 
                &&  !is_void<T>::value>
    struct add_rvalue_reference_helper
    { typedef T   type; };

    template <typename T>
    struct add_rvalue_reference_helper<T, true>
    {
#if !defined(BOOST_NO_RVALUE_REFERENCES)
        typedef T&&   type;
#else
        typedef T   type;
#endif
    };
}
    /// add_rvalue_reference
    template <typename T>
    struct add_rvalue_reference
    : public type_traits_detail::add_rvalue_reference_helper<T>
    { };

    template <typename T>
    struct add_rvalue_reference<T&>
    { 
        typedef T& type;
    };


}  // namespace boost

#endif  // BOOST_TYPE_TRAITS_EXT_ADD_RVALUE_REFERENCE__HPP
