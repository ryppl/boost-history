/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_INTEGER_SELECTION_HPP
#define BOOST_ACT_INTERLOCKED_INTEGER_SELECTION_HPP

#include <boost/act/interlocked/detail/cas_support.hpp>

#ifdef BOOST_ACT_INTERLOCKED_DETAIL_OPERATION_SUPPORT

#include <climits>
#include <cstddef>
#include <boost/cstdint.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/mpl/filter_view.hpp>
#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>
//#include <boost/act/interlocked/type_traits/is_interlocked.hpp>
#include <boost/mpl/size_t.hpp>

namespace boost { namespace act { namespace interlocked {

namespace detail
{
#ifdef BOOST_NO_INT64_T
typedef mpl::vector< signed char, short, int, long >
        int_types_all;

typedef mpl::vector< unsigned char, unsigned short, unsigned int
                   , unsigned long > uint_types_all;

typedef mpl::vector< int_fast8_t, int_fast16_t, int_fast32_t >
        int_fast_types_all;

typedef mpl::vector< uint_fast8_t, uint_fast16_t, uint_fast32_t >
        uint_fast_types_all;
#else
typedef mpl::vector< signed char, short, int, long, int64_t >
        int_types_all;

typedef mpl::vector< unsigned char, unsigned short, unsigned int
                   , unsigned long, uint64_t > uint_types_all;

typedef mpl::vector< int_fast8_t, int_fast16_t, int_fast32_t, int_fast64_t >
        int_fast_types_all;

typedef mpl::vector< uint_fast8_t, uint_fast16_t, uint_fast32_t, uint_fast64_t >
        uint_fast_types_all;
#endif

template< typename Type, typename DesiredSizeInBits >
struct int_type_is_size_in_bits
  : mpl::bool_< ( Type::value == DesiredSizeInBits::value ) > {};

template< typename Type >
struct type_is_native_interlocked_size
{
private:
  typedef mpl::vector_c
          < std::size_t
          , BOOST_PP_SEQ_ENUM( BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_SEQ )
          >
          supported_sizes;

  typedef typename mpl::find_if
          < supported_sizes
          , int_type_is_size_in_bits
            < mpl::_1
            , mpl::size_t< ( sizeof( Type ) * CHAR_BIT ) >
            >
          >
          ::type it;

  typedef typename mpl::end< supported_sizes >::type end_it;
public:
  typedef mpl::not_< is_same< it, end_it > > type;
};

typedef mpl::filter_view< int_types_all
                        , type_is_native_interlocked_size< mpl::_1 >
                        >
                        int_types;

typedef mpl::filter_view< uint_types_all
                        , type_is_native_interlocked_size< mpl::_1 >
                        >
                        uint_types;

typedef mpl::filter_view< int_fast_types_all
                        , type_is_native_interlocked_size< mpl::_1 >
                        >
                        int_fast_types;

typedef mpl::filter_view< uint_fast_types_all
                        , type_is_native_interlocked_size< mpl::_1 >
                        >
                        uint_fast_types;

template< typename Type, typename DesiredSize >
struct int_type_is_at_least_size
  : mpl::bool_< ( sizeof( Type ) >= DesiredSize::value ) > {};

template< typename TypeSequence, std::size_t LeastSize >
struct int_type_selector
{
private:
  typedef typename mpl::find_if
          < TypeSequence
          , int_type_is_at_least_size< mpl::_1, mpl::size_t< LeastSize > >
          >
          ::type it_to_type;

  typedef typename mpl::end< TypeSequence >::type end;
public:
  typedef mpl::not_< is_same< it_to_type, end > > match;

  typedef typename mpl::eval_if
          < match
          , mpl::deref< it_to_type >
          , mpl::identity< mpl::void_ >
          >
          ::type type;
};

template< bool Signed >
struct int_type_sequence
{
  typedef typename mpl::if_c< Signed, int_types, uint_types >
          ::type sequence;

  typedef typename mpl::if_c< Signed
                            , int_fast_types
                            , uint_fast_types
                            >
                            ::type fast_sequence;
};

template< bool Signed, std::size_t Bytes, bool PreferFast >
struct int_t_impl
{
private:
  typedef typename int_type_sequence< Signed >::sequence      sequence;
  typedef typename int_type_sequence< Signed >::fast_sequence fast_sequence;

  typedef detail::int_type_selector< fast_sequence, Bytes >
          fast_selector;

  typedef detail::int_type_selector< sequence, Bytes >
          small_selector;

  typedef typename mpl::if_c< PreferFast, fast_selector, small_selector >
          ::type pref_selector;

  typedef typename mpl::if_c< PreferFast, small_selector, fast_selector >
          ::type fall_selector;

  typedef typename pref_selector::match pref_match;
  typedef typename fall_selector::match fall_match;
public:
  typedef typename mpl::eval_if
          < pref_match
          , pref_selector
          , mpl::eval_if< fall_match
                        , fall_selector
                        , mpl::identity< mpl::void_ >
                        >
          >
          ::type type;
private:
  BOOST_MPL_ASSERT_MSG( ( mpl::not_< is_same< type, mpl::void_ > >::value )
                      , TYPE_HAS_NO_INTERLOCKED_SUPPORT
                      , ()
                      );
};

}

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
  typedef typename detail::int_t_impl
          <
            is_signed< LeastInt >::value
          , sizeof( LeastInt )
          , true // Prefer fast over least
          >
          ::type fast;
};

template< std::size_t Bits >
struct int_t
{
  typedef typename detail::int_t_impl
          <
            true // Signed
          , ( Bits / CHAR_BIT )
          , false // Prefer least over fast
          >
          ::type least;

  typedef typename int_fast_t< least >::fast fast;
};

template< std::size_t Bits >
struct uint_t
{
  typedef typename detail::int_t_impl
          <
            false // Unsigned
          , ( Bits / CHAR_BIT )
          , false // Prefer least over fast
          >
          ::type least;

  typedef typename int_fast_t< least >::fast fast;
};

/*
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
  typedef typename boost::int_max_value_t< MaxValue >::least least;
  typedef typename int_fast_t< least >::fast fast;
};

template< long MinValue >
struct int_min_value_t 
{
  typedef typename boost::int_min_value_t< MinValue >::least least;
  typedef typename int_fast_t< least >::fast fast;
};

//  unsigned
template< unsigned long Value >
struct uint_value_t 
{
  typedef typename boost::uint_value_t< Value >::least least;
  typedef typename int_fast_t< least >::fast fast;
};
*/
/*
namespace detail
{

template< typename Type >
struct logical_type_of_impl
  : mpl::if_< is_interlocked< Type >
            , bool
            , Type
            >
{
};

}

template< typename Type >
struct logical_type_of
  : mpl::if_< is_interlocked< Type >
            , detail::logical_type_of_impl< Type >
            , mpl::empty_base
            >
            ::type
{
};
*/
} } }  // namespace boost

#endif

#endif
