/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_DSL_ERROR_HANDLING_HPP_
#define BOOST_DSL_ERROR_HANDLING_HPP_

#include <boost/detail/is_xxx.hpp>
#include <boost/parameter/aux_/void.hpp>

namespace boost { 
namespace dsl {

template <typename dummy=parameter::void_> 
struct throw_on_error {}; 
BOOST_DETAIL_IS_XXX_DEF(throw_on_error, throw_on_error, 1)

template <typename dummy=parameter::void_> 
struct errno_on_error {};
BOOST_DETAIL_IS_XXX_DEF(errno_on_error, errno_on_error, 1)

template <typename dummy=parameter::void_> 
struct ignore_error {};
BOOST_DETAIL_IS_XXX_DEF(ignore_error, ignore_error, 1)

template <typename dummy=parameter::void_> 
struct user_error {};
BOOST_DETAIL_IS_XXX_DEF(user_error, user_error, 1)

}
}
#endif /*BOOST_DSL_ERROR_HANDLING_HPP_*/
