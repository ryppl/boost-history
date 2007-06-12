/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_INFO
#error Improper incusion of interlocked implementation header. Contact Boost.
#else

#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/control/if.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_SHORT_NAME                                \
BOOST_PP_TUPLE_ELEM( 2, 0, BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_INFO )

#define BOOST_ACT_INTERLOCKED_DETAIL_SEMANTICS                                 \
BOOST_PP_TUPLE_ELEM( 2, 1, BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_INFO )

#define BOOST_ACT_INTERLOCKED_DETAIL_AFFIX_full_barrier 0
#define BOOST_ACT_INTERLOCKED_DETAIL_AFFIX_acquire      1
#define BOOST_ACT_INTERLOCKED_DETAIL_AFFIX_release      2

#define BOOST_ACT_INTERLOCKED_DETAIL_FULL_NAME                                 \
BOOST_PP_IF( BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_AFFIX_                 \
                         , BOOST_ACT_INTERLOCKED_DETAIL_SEMANTICS              \
                         )                                                     \
           , BOOST_PP_CAT                                                      \
             (                                                                 \
               BOOST_ACT_INTERLOCKED_DETAIL_SHORT_NAME                         \
             , BOOST_PP_CAT( _                                                 \
                           , BOOST_ACT_INTERLOCKED_DETAIL_SEMANTICS            \
                           )                                                   \
             )                                                                 \
           , BOOST_ACT_INTERLOCKED_DETAIL_SHORT_NAME                           \
           )

#define BOOST_ACT_INTERLOCKED_DETAIL_OP_HEADER_OPENER()                        \
BOOST_PP_CAT                                                                   \
(                                                                              \
  BOOST_PP_CAT( <boost/act/interlocked/                                        \
              , BOOST_ACT_INTERLOCKED_DETAIL_SHORT_NAME                        \
              )                                                                \
, /                                                                            \
)

#define BOOST_ACT_INTERLOCKED_DETAIL_OP_RESULT_HEADER()                        \
BOOST_PP_CAT                                                                   \
(                                                                              \
  BOOST_ACT_INTERLOCKED_DETAIL_OP_HEADER_OPENER()                              \
, BOOST_PP_CAT                                                                 \
  (                                                                            \
   BOOST_ACT_INTERLOCKED_DETAIL_SHORT_NAME                                     \
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
  , BOOST_ACT_INTERLOCKED_DETAIL_FULL_NAME                                     \
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

#define BOOST_ACT_INTERLOCKED_DETAIL_RESULT_INFO                               \
BOOST_ACT_INTERLOCKED_DETAIL_SHORT_NAME

#include BOOST_ACT_INTERLOCKED_DETAIL_OP_RESULT_HEADER()
#include BOOST_ACT_INTERLOCKED_DETAIL_OP_IMPL_HEADER()
#include BOOST_ACT_INTERLOCKED_DETAIL_OP_DEFAULT_IMPL_HEADER()

#undef BOOST_ACT_INTERLOCKED_DETAIL_OP_DEFAULT_IMPL_HEADER
#undef BOOST_ACT_INTERLOCKED_DETAIL_OP_IMPL_HEADER
#undef BOOST_ACT_INTERLOCKED_DETAIL_OP_DETAIL_HEADER_OPENER
#undef BOOST_ACT_INTERLOCKED_DETAIL_OP_RESULT_HEADER
#undef BOOST_ACT_INTERLOCKED_DETAIL_OP_HEADER_OPENER

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/remove_volatile.hpp>

#include <boost/act/interlocked/detail/cas_support.hpp>
#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <boost/act/interlocked/detail/impl_decl.hpp>
#include <boost/act/interlocked/detail/has_nested_type.hpp>
#include <boost/type_traits/remove_volatile.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename TargetType >
typename lazy_enable_if
<
  mpl::and_
  <
    detail::are_valid_assign_style_params< TargetType >
  , mpl::not_< detail::is_interlocked_bool< TargetType > >
  >
, BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_SHORT_NAME
              , _result
              )
              < TargetType >
>
::type
BOOST_ACT_INTERLOCKED_DETAIL_FULL_NAME( TargetType& target )
{
  typedef typename remove_volatile< TargetType >::type type;
  typedef typename BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_SHORT_NAME
                               , _result
                               )
                               < TargetType >
                               ::type result_type;

  typedef detail::BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_FULL_NAME
                               , _impl
                               )
          < result_type
          , type
          >
          impl_type;

  return mpl::if_< detail::has_nested_type< impl_type >
                 , impl_type
                 , detail::BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_FULL_NAME
                                       , _default_impl
                                       )
                                       < result_type, type >
                 >
                 ::type
                 ::execute( target );
}

} } }

#undef BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_INFO

#endif
