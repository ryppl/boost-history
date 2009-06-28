/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_LUID_DSL_ON_OVERFLOW_HPP_
#define BOOST_LUID_DSL_ON_OVERFLOW_HPP_

#include "boost/luid/dsl/error_handling.hpp"
#include <boost/detail/is_xxx.hpp>
#include <boost/parameter/aux_/void.hpp>
#include <boost/parameter/parameters.hpp>

namespace boost {
namespace luid {
namespace dsl {

struct on_overflow_marker{};

template<typename T>
struct is_on_overflow:is_base_and_derived<on_overflow_marker,T> {};

template<typename T=parameter::void_>
struct on_overflow: parameter::template_keyword<on_overflow<>,T> {};

//using ::boost::luid::dsl::errno_on_error;
//using ::boost::luid::dsl::is_errno_on_error;
//using ::boost::luid::dsl::ignore_error;
//using ::boost::luid::dsl::is_ignore_error;
//using ::boost::luid::dsl::throw_on_error;
//using ::boost::luid::dsl::is_throw_on_error;

template<typename T>
struct is_on_overflow<errno_on_error<T> >: mpl::true_ {};

template<typename T>
struct is_on_overflow<ignore_error<T> >: mpl::true_ {};

template<typename T>
struct is_on_overflow<throw_on_error<T> >: mpl::true_ {};

 
} // dsl
} // luid
} // boost
#endif // BOOST_LUID_DSL_ON_OVERFLOW_HPP_
