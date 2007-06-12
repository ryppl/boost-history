/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_INTEGER_HAS_INTERLOCKED_OPERATIONS_HPP
#define BOOST_ACT_INTERLOCKED_INTEGER_HAS_INTERLOCKED_OPERATIONS_HPP

#include <boost/act/config/interlocked/has_interlocked_operations.hpp>
#include <boost/mpl/bool.hpp>

#ifndef BOOST_ACT_HAS_INTERLOCKED_OPERATIONS

namespace boost { namespace act { namespace interlocked {

template< typename Type >
struct is_interlocked_type : mpl::false_ {};

template< typename Type >
struct has_interlocked_operations : is_interlocked_type< Type > {};

} } }

#else

#include <boost/config.hpp>

#ifdef BOOST_WINDOWS

#include <boost/act/interlocked/integer/types.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/act/detail/is_nonfunction_pointer.hpp>

namespace boost { namespace act { namespace interlocked {

#if WINVER < 0x0600

template< typename Type >
struct is_interlocked_type
  : mpl::or_< mpl::and_< is_integral< Type >
                       , mpl::bool_< ( sizeof( Type ) == 4 ) >
                       >
            , act::detail::is_nonfunction_pointer< Type >
            , is_same< Type, bool_t >
            , is_same< Type, bool_fast_t >
            > {};

template< typename Type >
struct has_interlocked_operations
  : is_interlocked_type< Type > {};

#else

template< typename Type >
struct is_interlocked_type
  : mpl::or_< mpl::and_< is_integral< Type >
                       , mpl::or_< mpl::bool_< ( sizeof( Type ) == 8 ) >
                                 , mpl::bool_< ( sizeof( Type ) == 4 ) >
                                 >
                       >
            , act::detail::is_nonfunction_pointer< Type >
            , is_same< Type, bool_t >
            , is_same< Type, bool_fast_t >
            >

template< typename Type >
struct has_interlocked_operations
  : is_interlocked_type< Type > {};
{
};

#endif

} } }  // namespace boost

#endif

#endif

#endif
