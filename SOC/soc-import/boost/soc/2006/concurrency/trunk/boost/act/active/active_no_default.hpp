//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

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
