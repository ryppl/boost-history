// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_DETAIL_RETURN_VOID_HPP_INCLUDED
#define BOOST_IDL_DETAIL_RETURN_VOID_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/type_traits/remove_reference.hpp>

namespace boost { namespace interfaces { namespace detail {

// Used when a function is canceled by the cancel member function of an advice 
// instance. Only occurs when the return type is void.

template<typename Result>
struct return_void_impl { 
    static Result execute() // Never executed.
    {   
        typedef typename remove_reference<Result>::type type;
        static type* t = 0;
        return *t;
    }
};

template<>
struct return_void_impl<void> {
    static void execute() { }
};

template<typename Result>
Result return_void() { return return_void_impl<Result>::execute(); }

} } } // End namespace detail, interfaces, boost.

#endif // #ifndef BOOST_IDL_DETAIL_RETURN_VOID_HPP_INCLUDED
