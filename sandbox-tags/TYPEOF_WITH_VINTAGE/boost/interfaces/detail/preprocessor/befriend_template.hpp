// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_DETAIL_BEFRIEND_TEMPLATE_HPP_INCLUDED
#define BOOST_IDL_DETAIL_BEFRIEND_TEMPLATE_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/interfaces/access.hpp>
#ifndef BOOST_IDL_NO_ACCESS_CONTROL
# include <boost/interfaces/detail/preprocessor/template_decl.hpp>
#endif

#ifndef BOOST_IDL_NO_ACCESS_CONTROL
# define BOOST_IDL_BEFRIEND_TEMPLATE_CLASS(name, arity, param) \
    BOOST_IDL_TEMPLATE_DECL(arity, param) friend class name; \
    /**/
# define BOOST_IDL_BEFRIEND_TEMPLATE_STRUCT(name, arity, param) \
    BOOST_IDL_TEMPLATE_DECL(arity, param) friend struct name; \
    /**/
#else
# define BOOST_IDL_BEFRIEND_TEMPLATE_CLASS(name, arity, param)
# define BOOST_IDL_BEFRIEND_TEMPLATE_STRUCT(name, arity, param)
#endif

#endif // #ifndef BOOST_IDL_DETAIL_BEFRIEND_TEMPLATE_HPP_INCLUDED
