/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_INTEGER_SELECTION_HPP
#define BOOST_ACT_INTERLOCKED_INTEGER_SELECTION_HPP

#include <boost/act/config/interlocked/has_interlocked_operations.hpp>

#ifdef BOOST_ACT_HAS_INTERLOCKED_OPERATIONS

#include <boost/config.hpp>

#if defined( BOOST_WINDOWS ) && defined( BOOST_MSVC )

#include <cstddef>
#include <boost/integer.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>

namespace boost { namespace act { namespace interlocked {

namespace detail
{
struct type_too_large;
}

#if WINVER < 0x0600

//  fast integers from least integers
template< typename LeastInt >
struct int_fast_t
{
private:
  BOOST_MPL_ASSERT_MSG( is_integral< LeastInt >::value
                      , ARGUMENTS_TO_INT_FAST_T_MUST_BE_INTEGRAL
                      , ( LeastInt )
                      );
public:
  typedef typename mpl::eval_if_c
          <
            ( sizeof( LeastInt ) <= 4 )
          , mpl::if_< is_signed< LeastInt >
                    , long
                    , unsigned long
                    >
          , mpl::identity< detail::type_too_large >
          >
          ::type fast;
private:
  BOOST_MPL_ASSERT_MSG( (mpl::not<is_same<fast,detail::type_too_large> >::value)
                      , TYPE_TOO_LARGE_FOR_INTERLOCKED_SUPPORT
                      , ( LeastInt )
                      );
};

#else

//  fast integers from least integers
template< typename LeastInt >
struct int_fast_t
{
private:
  BOOST_MPL_ASSERT_MSG( is_integral< LeastInt >::value
                      , ARGUMENTS_TO_INT_FAST_T_MUST_BE_INTEGRAL
                      , ( LeastInt )
                      );
public:
  typedef typename mpl::eval_if_c
          <
            ( sizeof( LeastInt ) <= 4 )
          , mpl::if_< is_signed< LeastInt >
                    , long
                    , unsigned long
                    >
          , mpl::eval_if_c< ( sizeof( LeastInt ) <= 8 )
                          , mpl::if_< is_signed< LeastInt >
                                    , long long
                                    , unsigned long long
                                    >
                          , mpl::identity< detail::type_too_large >
                          >
          >
          ::type fast;
private:
  BOOST_MPL_ASSERT_MSG( (mpl::not<is_same<fast,detail::type_too_large> >::value)
                      , TYPE_TOO_LARGE_FOR_INTERLOCKED_SUPPORT
                      , ( LeastInt )
                      );
};

#endif

//  signed
template< int Bits >
struct int_t 
{
  typedef typename boost::int_t< Bits >::least least;
  typedef typename int_fast_t< least >::fast fast;
};

//  unsigned
template< int Bits >
struct uint_t 
{
  typedef typename boost::uint_t< Bits >::least least;
  typedef typename int_fast_t< least >::fast fast;
};

//  signed
template< long MaxValue >
struct int_max_value_t 
{
  typedef typename boost::int_max_value_t< Bits >::least least;
  typedef typename int_fast_t< least >::fast fast;
};

template< long MinValue >
struct int_min_value_t 
{
  typedef typename boost::int_min_value_t< Bits >::least least;
  typedef typename int_fast_t< least >::fast fast;
};

//  unsigned
template< unsigned long Value >
struct uint_value_t 
{
  typedef typename boost::uint_value_t< Bits >::least least;
  typedef typename int_fast_t< least >::fast fast;
};

namespace detail
{

template< typename Type >
struct logical_type_of_impl
  : mpl::if_< is_interlocked_bool< Type >
            , bool
            , Type
            >
{
};

}

template< typename Type >
struct logical_type_of
  : mpl::if_< has_interlocked_operations< Type >
            , detail::logical_type_of_impl< Type >
            , mpl::empty_base
            >
            ::type
{
};

} } }  // namespace boost

#endif

#endif

#endif
