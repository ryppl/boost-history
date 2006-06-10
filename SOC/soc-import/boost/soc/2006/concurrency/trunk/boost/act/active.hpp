//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ACTIVE_HPP
#define BOOST_ACT_ACTIVE_HPP

#include "type_traits/add_active.hpp"
#include "detail/type_from_function_params.hpp"

// ToDo: Rewrite to internally call BOOST_ACTIVE_H
// ToDo: Remove act qualifiers before adding them
#define BOOST_ACTIVE( source_type )                                            \
::boost::act::add_active                                                       \
<                                                                              \
  ::boost::act::detail::type_from_function_params< void source_type >::type    \
>                                                                              \
::type 

#define BOOST_ACTIVE_T( source_type )                                          \
typename ::boost::act::add_active                                              \
<                                                                              \
, typename ::boost::act::detail::detail::type_from_function_params             \
    < void source_type >::type                                                 \
>                                                                              \
::type 

#define BOOST_ACTIVE_M( source_type, act_model )                               \
::boost::act::add_active                                                       \
<                                                                              \
  ::boost::act::detail::type_from_function_params< void source_type >          \
  ::type                                                                       \
, ::boost::act::detail::type_from_function_params< void act_model >            \
  ::type                                                                       \
>                                                                              \
::type 

#define BOOST_ACTIVE_M_T( source_type, act_model )                             \
typename ::boost::act::add_active                                              \
<                                                                              \
  ::boost::act::detail::type_from_function_params< void source_type >::type    \
, typename ::boost::act::detail::detail::type_from_function_params             \
    < void act_model >::type                                                   \
>                                                                              \
::type 

#define BOOST_ACTIVE_T_M_T( source_type, act_model )                           \
::boost::act::add_active                                                       \
<                                                                              \
, typename ::boost::act::detail::detail::type_from_function_params             \
    < void source_type >::type                                                 \
, typename ::boost::act::detail::detail::type_from_function_params             \
    < void act_model >::type                                                   \
>                                                                              \
::type 

#endif
