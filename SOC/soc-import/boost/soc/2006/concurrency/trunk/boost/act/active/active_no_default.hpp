//  Copyright (c) 2006, Matthew Calabrese
//
//  This code may be used under either of the following two licences:
//
//    Permission is hereby granted, free of charge, to any person
//    obtaining a copy of this software and associated documentation
//    files (the "Software"), to deal in the Software without
//    restriction, including without limitation the rights to use,
//    copy, modify, merge, publish, distribute, sublicense, and/or
//    sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following
//    conditions:
//
//    The above copyright notice and this permission notice shall be
//    included in all copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//    OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACT_ACTIVE_ACTIVE_NO_DEFAULT_HPP
#define BOOST_ACT_ACTIVE_ACTIVE_NO_DEFAULT_HPP

#include "../type_traits/add_active_no_default.hpp"
#include "../detail/type_from_function_params.hpp"

#define BOOST_ACTIVE( source_type )                                            \
BOOST_ACTIVE_BASE( source_type )

#define BOOST_ACTIVE_M( source_type, act_model )                               \
BOOST_ACTIVE_M_BASE( source_type, act_model )

#define BOOST_ACTIVE_T( source_type )                                          \
typename BOOST_ACTIVE_T_BASE( source_type )

#define BOOST_ACTIVE_M_T( source_type, act_model )                             \
typename BOOST_ACTIVE_M_T_BASE( source_type, act_model )

// ToDo: Rewrite to internally call BOOST_ACTIVE_M_BASE
// ToDo: Remove act qualifiers before adding them
#define BOOST_ACTIVE_BASE( source_type )                                       \
::boost::act::add_active                                                       \
<                                                                              \
  ::boost::act::detail::type_from_function_params< void source_type >::type    \
>                                                                              \
::type

#define BOOST_ACTIVE_M_BASE( source_type, act_model )                          \
::boost::act::add_active                                                       \
<                                                                              \
  ::boost::act::detail::type_from_function_params< void source_type >::type    \
, ::boost::act::detail::type_from_function_params< void act_model >::type      \
>                                                                              \
::type

#define BOOST_ACTIVE_T_BASE( source_type )                                     \
::boost::act::add_active                                                       \
<                                                                              \
  typename ::boost::act::detail::type_from_function_params                     \
    < void source_type >::type                                                 \
>                                                                              \
::type

#define BOOST_ACTIVE_M_T_BASE( source_type, act_model )                        \
::boost::act::add_active                                                       \
<                                                                              \
  typename ::boost::act::detail::type_from_function_params                     \
    < void source_type, void act_model >::type                                 \
, typename ::boost::act::detail::type_from_function_params                     \
    < void act_model, void source_type >::type                                 \
>                                                                              \
::type

#endif
