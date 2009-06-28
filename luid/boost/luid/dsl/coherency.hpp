/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_LUID_DSL_COHERENCY_HPP_
#define BOOST_LUID_DSL_COHERENCY_HPP_

#include <boost/detail/is_xxx.hpp>
#include <boost/parameter/aux_/void.hpp>
#include <boost/parameter/parameters.hpp>

namespace boost {
namespace luid {
namespace dsl {

struct coherency_marker{};

template<typename T>
struct is_coherency: is_base_and_derived<coherency_marker,T>
{};

template<typename T=parameter::void_>
struct coherency: parameter::template_keyword<coherency<>,T>
{};

template <typename dummy=parameter::void_> 
struct ensure : coherency_marker {
};
BOOST_DETAIL_IS_XXX_DEF(ensure, ensure, 1);

template <typename dummy=parameter::void_> 
struct ignore : coherency_marker {
};
BOOST_DETAIL_IS_XXX_DEF(ignore, ignore, 1);

} // dsl
} // luid
} // boost
#endif // BOOST_LUID_DSL_COHERENCY_HPP_
