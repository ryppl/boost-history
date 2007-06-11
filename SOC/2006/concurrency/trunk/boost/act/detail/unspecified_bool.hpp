/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_DETAIL_UNSPECIFIED_BOOL_HPP
#define BOOST_ACT_DETAIL_UNSPECIFIED_BOOL_HPP

#include <boost/act/detail/type_from_function_params.hpp>

#include <boost/config.hpp>

namespace boost
{
namespace act
{
namespace detail
{

template< typename Type >
struct unspecified_bool_helper
{
  void true_() {}
};

template< typename Type >
struct unspecified_bool_meta
{
  typedef void (unspecified_bool_helper< Type >::*type)();
};

#define BOOST_ACT_DETAIL_UNSPECIFIED_BOOL( tag )                               \
BOOST_ACT_DETAIL_UNSPECIFIED_BOOL_IMPL                                         \
( tag                                                                          \
, BOOST_ACT_DETAIL_UNSPECIFIED_BOOL_NO_TYPENAME                                \
)

#define BOOST_ACT_DETAIL_UNSPECIFIED_TRUE( tag )                               \
BOOST_ACT_DETAIL_UNSPECIFIED_TRUE_IMPL                                         \
( tag                                                                          \
, BOOST_ACT_DETAIL_UNSPECIFIED_BOOL_NO_TYPENAME                                \
)

#define BOOST_ACT_DETAIL_UNSPECIFIED_FALSE( tag )                              \
BOOST_ACT_DETAIL_UNSPECIFIED_FALSE_IMPL                                        \
( tag                                                                          \
, BOOST_ACT_DETAIL_UNSPECIFIED_BOOL_NO_TYPENAME                                \
)

#define BOOST_ACT_DETAIL_UNSPECIFIED_BOOL_VALUE( tag, value )                  \
BOOST_ACT_DETAIL_UNSPECIFIED_BOOL_VALUE_IMPL                                   \
( tag                                                                          \
, value                                                                        \
, BOOST_ACT_DETAIL_UNSPECIFIED_BOOL_NO_TYPENAME                                \
)

#define BOOST_ACT_DETAIL_UNSPECIFIED_BOOL_T( tag )                             \
BOOST_ACT_DETAIL_UNSPECIFIED_BOOL_IMPL                                         \
( tag                                                                          \
, typename                                                                     \
)

#define BOOST_ACT_DETAIL_UNSPECIFIED_TRUE_T( tag )                             \
BOOST_ACT_DETAIL_UNSPECIFIED_TRUE_IMPL                                         \
( tag                                                                          \
, typename                                                                     \
)

#define BOOST_ACT_DETAIL_UNSPECIFIED_FALSE_T( tag )                            \
BOOST_ACT_DETAIL_UNSPECIFIED_FALSE_IMPL                                        \
( tag                                                                          \
, typename                                                                     \
)

#define BOOST_ACT_DETAIL_UNSPECIFIED_BOOL_VALUE_T( tag, value )                \
BOOST_ACT_DETAIL_UNSPECIFIED_BOOL_VALUE_IMPL                                   \
( tag                                                                          \
, value                                                                        \
, typename                                                                     \
)

#define BOOST_ACT_DETAIL_UNSPECIFIED_BOOL_NO_TYPENAME

#define BOOST_ACT_DETAIL_UNSPECIFIED_BOOL_IMPL( tag, q_typename )              \
q_typename ::boost::act::detail::unspecified_bool_meta                         \
<                                                                              \
  q_typename ::boost::act::detail::type_from_function_params< void tag >::type \
>                                                                              \
::type

#define BOOST_ACT_DETAIL_UNSPECIFIED_TRUE_IMPL( tag, q_typename )              \
&::boost::act::detail::unspecified_bool_helper                                 \
<                                                                              \
  q_typename ::boost::act::detail::type_from_function_params< void tag >::type \
>                                                                              \
::true_

#define BOOST_ACT_DETAIL_UNSPECIFIED_FALSE_IMPL( tag, q_typename )             \
static_cast                                                                    \
<                                                                              \
  q_typename ::boost::act::detail::unspecified_bool_meta                       \
  <                                                                            \
    q_typename ::boost::act::detail::type_from_function_params< void tag >     \
      ::type                                                                   \
  >                                                                            \
  ::type                                                                       \
>( 0 )

#define BOOST_ACT_DETAIL_UNSPECIFIED_BOOL_VALUE_IMPL( tag, value, q_typename ) \
( ( value ) ? BOOST_ACT_DETAIL_UNSPECIFIED_TRUE_IMPL( tag, q_typename )        \
            : BOOST_ACT_DETAIL_UNSPECIFIED_FALSE_IMPL( tag, q_typename )       \
)

}
}
}

#endif
