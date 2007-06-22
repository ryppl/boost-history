/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

/*

Note: If you are getting an error in this file, please post to the mailing list

*/

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_BINARY_RESULT_INFO
#error Improper incusion of interlocked implementation header. Contact Boost.
#else

#include <boost/act/interlocked/detail/cas_support.hpp>

#ifdef BOOST_ACT_INTERLOCKED_DETAIL_OPERATION_SUPPORT

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>

#include <boost/act/interlocked/detail/impl.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_IMPL_INFO                                 \
BOOST_ACT_INTERLOCKED_DETAIL_BINARY_RESULT_INFO

#include BOOST_ACT_INTERLOCKED_DETAIL_IMPL_BEGIN()

#include BOOST_ACT_INTERLOCKED_DETAIL_BINARY_RESULT_DECLARATION_HEADER()

#undef BOOST_ACT_INTERLOCKED_DETAIL_BINARY_RESULT_DECLARATION_HEADER

#include <boost/act/interlocked/detail/result_meta.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename TargetType, typename OperandType >
struct BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_FULL_NAME
                   , _result
                   )
  : detail::result_meta
    <
      detail::BOOST_PP_CAT
      ( BOOST_ACT_INTERLOCKED_DETAIL_IMPL_FULL_NAME
      , _impl
      )
    , TargetType, OperandType
    > {};

} } }

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_RESULT_IS_READY_FOR_CLEANUP

#include BOOST_ACT_INTERLOCKED_DETAIL_IMPL_END()

#else

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_RESULT_IS_READY_FOR_CLEANUP

// ToDo: Define a result which produces a compile-time error when instantiated.

#endif

#endif
