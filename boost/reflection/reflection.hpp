/*
 * Boost.Reflection / main header
 *
 * (C) Copyright Mariano G. Consoni and Jeremy Pack 2008
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#ifndef BOOST_REFLECTION_REFLECTION_HPP
#define BOOST_REFLECTION_REFLECTION_HPP

#include <map>
#include <vector>

#include <boost/extension/impl/typeinfo.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/reflection/constructor.hpp>
#include <boost/reflection/data.hpp>
#include <boost/reflection/data_info.hpp>
#include <boost/reflection/factory.hpp>
#include <boost/reflection/function.hpp>

namespace boost {
namespace reflections {
using extensions::type_info_handler;
namespace impl {
template <class T>
void destruct(void * val) {
  delete static_cast<T*>(val);
}

#define BOOST_PP_ITERATION_LIMITS (0, \
    BOOST_PP_INC(BOOST_REFLECTION_MAX_FUNCTOR_PARAMS) - 1)
#define BOOST_PP_FILENAME_1 \
  <boost/reflection/impl/reflector_free_functions.hpp>
#include BOOST_PP_ITERATE()

// This is used to store, indexed by type information,
// any normal function pointers.
typedef void (*FunctionPtr)();

}  // namespace impl

// Since there are two specializations, with and without parameter
// information - but which are otherwise mostly the same - the
// implementation file is included twice.
#define BOOST_REFLECTION_WITH_PARAMETER_INFO
#include <boost/reflection/function_info.hpp>
#include <boost/reflection/constructor_info.hpp>
#include <boost/reflection/impl/reflection.hpp>
#undef BOOST_REFLECTION_WITH_PARAMETER_INFO
#include <boost/reflection/function_info.hpp>
#include <boost/reflection/constructor_info.hpp>
#include <boost/reflection/impl/reflection.hpp>

typedef basic_reflection<> reflection;
}}
#endif  // BOOST_REFLECTION_REFLECTION_HPP
