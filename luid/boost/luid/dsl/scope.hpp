/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_LUID_DSL_SCOPE_HPP
#define BOOST_LUID_DSL_SCOPE_HPP

#include "boost/luid/dsl/thread_model.hpp"
#include <boost/detail/is_xxx.hpp>
#include <boost/parameter/aux_/void.hpp>
#include <boost/parameter/parameters.hpp>

namespace boost {
namespace luid {
namespace dsl {

// Scope
struct scope_marker{};

template<typename T>
struct is_scope:is_base_and_derived<scope_marker,T> {};

template<typename T=parameter::void_>
struct scope: parameter::template_keyword<scope<>,T> {};

//using ::boost::dsl::mono_threaded;
//using ::boost::dsl::is_mono_threaded;
//using ::boost::dsl::multi_threaded;
//using ::boost::dsl::is_multi_threaded;
//using ::boost::dsl::multi_process;
//using ::boost::dsl::is_multi_process;
//using ::boost::dsl::internally_locked;
//using ::boost::dsl::is_internally_locked;
//using ::boost::dsl::externally_locked;
//using ::boost::dsl::is_externally_locked;

template<typename T>
struct is_scope<mono_threaded<T> >: mpl::true_
{};

template<typename T>
struct is_scope<multi_threaded<T> >: mpl::true_
{};

template<typename T>
struct is_scope<multi_process<T> >: mpl::true_
{};


 
} // dsl
} // luid
} // boost
#endif // BOOST_LUID_DSL_SCOPE_HPP
