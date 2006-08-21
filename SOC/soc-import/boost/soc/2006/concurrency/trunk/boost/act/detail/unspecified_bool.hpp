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

#ifndef BOOST_ACT_DETAIL_UNSPECIFIED_BOOL_HPP
#define BOOST_ACT_DETAIL_UNSPECIFIED_BOOL_HPP

#include "type_from_function_params.hpp"

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

}
}
}

#endif
