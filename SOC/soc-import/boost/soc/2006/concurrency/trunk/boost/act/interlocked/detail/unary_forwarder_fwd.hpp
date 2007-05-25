/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_OP_NAME
#error Improper incusion of interlocked implementation header. Contact Boost.
#else

#include <boost/preprocessor/cat.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_OP_HEADER_OPENER()                        \
BOOST_PP_CAT                                                                   \
(                                                                              \
  BOOST_PP_CAT( <boost/act/interlocked/                                        \
              , BOOST_ACT_INTERLOCKED_DETAIL_OP_NAME                           \
              )                                                                \
, /                                                                            \
)

#define BOOST_ACT_INTERLOCKED_DETAIL_OP_RESULT_HEADER()                        \
BOOST_PP_CAT                                                                   \
(                                                                              \
  BOOST_ACT_INTERLOCKED_DETAIL_OP_HEADER_OPENER()                              \
, BOOST_PP_CAT                                                                 \
  (                                                                            \
   BOOST_ACT_INTERLOCKED_DETAIL_OP_NAME                                        \
  , _result.hpp>                                                               \
  )                                                                            \
)

#define BOOST_ACT_INTERLOCKED_DETAIL_OP_DETAIL_HEADER_OPENER()                 \
BOOST_PP_CAT                                                                   \
(                                                                              \
  BOOST_ACT_INTERLOCKED_DETAIL_OP_HEADER_OPENER()                              \
, BOOST_PP_CAT                                                                 \
  (                                                                            \
    detail/                                                                    \
  , BOOST_ACT_INTERLOCKED_DETAIL_OP_NAME                                       \
  )                                                                            \
)

#define BOOST_ACT_INTERLOCKED_DETAIL_OP_IMPL_HEADER()                          \
BOOST_PP_CAT                                                                   \
(                                                                              \
  BOOST_ACT_INTERLOCKED_DETAIL_OP_DETAIL_HEADER_OPENER()                       \
, _impl.hpp>                                                                   \
)

#define BOOST_ACT_INTERLOCKED_DETAIL_OP_DEFAULT_IMPL_HEADER()                  \
BOOST_PP_CAT                                                                   \
(                                                                              \
  BOOST_ACT_INTERLOCKED_DETAIL_OP_DETAIL_HEADER_OPENER()                       \
, _default_impl.hpp>                                                           \
)


#include BOOST_ACT_INTERLOCKED_DETAIL_OP_RESULT_HEADER()
#include BOOST_ACT_INTERLOCKED_DETAIL_OP_IMPL_HEADER()
#include BOOST_ACT_INTERLOCKED_DETAIL_OP_DEFAULT_IMPL_HEADER()

#undef BOOST_ACT_INTERLOCKED_DETAIL_OP_DEFAULT_IMPL_HEADER
#undef BOOST_ACT_INTERLOCKED_DETAIL_OP_IMPL_HEADER
#undef BOOST_ACT_INTERLOCKED_DETAIL_OP_DETAIL_HEADER_OPENER
#undef BOOST_ACT_INTERLOCKED_DETAIL_OP_RESULT_HEADER
#undef BOOST_ACT_INTERLOCKED_DETAIL_OP_HEADER_OPENER

#include <boost/utility/enable_if.hpp>

#include <boost/act/interlocked/detail/cas_support.hpp>
#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <boost/act/interlocked/detail/impl_decl.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename TargetType >
typename lazy_enable_if
<
  mpl::and_
  <
    detail::are_valid_assign_style_params< TargetType >
  , mpl::not_< detail::is_interlocked_bool< TargetType > >
  >
, BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_OP_NAME
              , _result
              )
              < TargetType >
>
::type
BOOST_ACT_INTERLOCKED_DETAIL_OP_NAME( TargetType& target );

} } }

#undef BOOST_ACT_INTERLOCKED_DETAIL_OP_NAME

#endif
