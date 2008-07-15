//  boost integer.hpp header file  -------------------------------------------//

//  Copyright Beman Dawes and Daryle Walker 1999.  Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/integer for documentation.

//  Revision History
//   15 Jul 08  Added exact-integer templates; added MPL-compatible variant of
//              processor-optimized integer template. (Daryle Walker)
//   14 Jul 08  Added extended-integer support. (Daryle Walker)
//   13 Jul 08  Redid implmentation. (Daryle Walker)
//   22 Sep 01  Added value-based integer templates. (Daryle Walker)
//   01 Apr 01  Modified to use new <boost/limits.hpp> header. (John Maddock)
//   30 Jul 00  Add typename syntax fix (Jens Maurer)
//   28 Aug 99  Initial version

#ifndef BOOST_INTEGER_HPP
#define BOOST_INTEGER_HPP

#include <boost/integer_fwd.hpp>  // self include

#include <boost/config.hpp>             // for BOOST_STATIC_CONSTANT, etc.
#include <boost/cstdint.hpp>            // for boost::uintmax_t, intmax_t
#include <boost/integer_traits.hpp>     // for boost::integer_traits
#include <boost/limits.hpp>             // for std::numeric_limits
#include <boost/utility/enable_if.hpp>  // for boost::enable_if_c

#include <climits>  // for UCHAR_MAX, USHRT_MAX, UINT_MAX, ULONG_MAX, etc.

namespace boost
{

  //  integer template mapping a type to its processor-optimized analog  -----//

  //  Some types can be handled better by the processor than others.  This
  //  template metafunction should map various built-in integral types to
  //  the processor's perferred type for the given type's value range
  template < typename BaseInt >
  struct fast_integral
  {
      typedef BaseInt  type;
  };

  // Platform-specific specializations should go here.

  //  fast integers from least integers
  //  int_fast_t<> works correctly for unsigned too, in spite of the name.
  template< typename LeastInt >
  struct int_fast_t { typedef typename fast_integral<LeastInt>::type fast; };

namespace detail
{

  //  Helper templates  ------------------------------------------------------//

  //  convert integer category to type ; default is empty
  template< int Rank, typename Signedness > struct  int_least_helper {};

  //  specializatons: 1=(unsigned) __int64/long long, 2=(unsigned) long,
  //     3=unsigned/int, 4=(unsigned) short, 5=(un)signed char
  //  no specializations for 0: requests for a type > (unsigned) (long) long are
  //     in error
#ifdef BOOST_HAS_LONG_LONG
  template<> struct int_least_helper<1, signed>
   { typedef long_long_type least; };
  template<> struct int_least_helper<1, unsigned>
   { typedef ulong_long_type least; };
#elif defined(BOOST_HAS_MS_INT64)
  template<> struct int_least_helper<1, signed> { typedef __int64 least; };
  template<> struct int_least_helper<1, unsigned>
   { typedef unsigned __int64 least; };
#endif
  template<> struct int_least_helper<2, signed> { typedef long least; };
  template<> struct int_least_helper<2, unsigned>
   { typedef unsigned long least; };
  template<> struct int_least_helper<3, signed> { typedef int least; };
  template<> struct int_least_helper<3, unsigned>
   { typedef unsigned int least; };
  template<> struct int_least_helper<4, signed> { typedef short least; };
  template<> struct int_least_helper<4, unsigned>
   { typedef unsigned short least; };
  template<> struct int_least_helper<5, signed> { typedef signed char least; };
  template<> struct int_least_helper<5, unsigned>
   { typedef unsigned char least; };

  //  category bounds
  enum
  {
#if defined(BOOST_HAS_LONG_LONG) || defined(BOOST_HAS_MS_INT64)
      lowest_integral_rank  = 1,
#else
      lowest_integral_rank  = 2,
#endif
      highest_integral_rank = 5
  };

  //  map a bit count to a category
  template < int BitsIncludingSign >
  struct int_rank_helper
  {
      BOOST_STATIC_CONSTANT( int, mantissa = BitsIncludingSign - 1 );
      BOOST_STATIC_CONSTANT( int, rank = (BitsIncludingSign > 0) * (
#ifdef BOOST_HAS_LONG_LONG
       (mantissa <= std::numeric_limits< long_long_type >::digits) +
#elif defined(BOOST_HAS_MS_INT64)
       (mantissa <= std::numeric_limits< __int64 >::digits) +
#else
       1 +
#endif
       (mantissa <= std::numeric_limits< long >::digits) +
       (mantissa <= std::numeric_limits< int >::digits) +
       (mantissa <= std::numeric_limits< short >::digits) +
       (mantissa <= std::numeric_limits< signed char >::digits)) );
  };

  template < int Bits >
  struct uint_rank_helper
  {
      BOOST_STATIC_CONSTANT( int, rank = (Bits >= 0) * (
#ifdef BOOST_HAS_LONG_LONG
       (Bits <= std::numeric_limits< ulong_long_type >::digits) +
#elif defined(BOOST_HAS_MS_INT64)
       (Bits <= std::numeric_limits< unsigned __int64 >::digits) +
#else
       1 +
#endif
       (Bits <= std::numeric_limits< unsigned long >::digits) +
       (Bits <= std::numeric_limits< unsigned int >::digits) +
       (Bits <= std::numeric_limits< unsigned short >::digits) +
       (Bits <= std::numeric_limits< unsigned char >::digits)) );
  };

  template < int BitsIncludingSign >
  struct int_exact_rank_helper   { BOOST_STATIC_CONSTANT( int, rank = 0 ); };
  template < int Bits >
  struct uint_exact_rank_helper  { BOOST_STATIC_CONSTANT( int, rank = 0 ); };

#define BOOST_PRIVATE_INT_EXACT_BUILDER(Type, Rank) \
  template < > \
  struct int_exact_rank_helper<std::numeric_limits< Type >::digits + 1> \
  { BOOST_STATIC_CONSTANT( int, rank = Rank ); }
#define BOOST_PRIVATE_UINT_EXACT_BUILDER(Type, Rank) \
  template < > \
  struct uint_exact_rank_helper<std::numeric_limits< Type >::digits> \
  { BOOST_STATIC_CONSTANT( int, rank = Rank ); }

#if defined(BOOST_HAS_LONG_LONG) && ((ULLONG_MAX > ULONG_MAX) || (ULONGLONG_MAX > ULONG_MAX))
  BOOST_PRIVATE_INT_EXACT_BUILDER( long_long_type, 1 );
  BOOST_PRIVATE_UINT_EXACT_BUILDER( ulong_long_type, 1 );
#elif defined(BOOST_HAS_MS_INT64) && (0xFFFFFFFFFFFFFFFFui64 > ULONG_MAX)
  BOOST_PRIVATE_INT_EXACT_BUILDER( __int64, 1 );
  BOOST_PRIVATE_UINT_EXACT_BUILDER( unsigned __int64, 1 );
#endif
#if ULONG_MAX > UINT_MAX
  BOOST_PRIVATE_INT_EXACT_BUILDER( long, 2 );
  BOOST_PRIVATE_UINT_EXACT_BUILDER( unsigned long, 2 );
#endif
#if UINT_MAX > USHRT_MAX
  BOOST_PRIVATE_INT_EXACT_BUILDER( int, 3 );
  BOOST_PRIVATE_UINT_EXACT_BUILDER( unsigned, 3 );
#endif
#if USHRT_MAX > UCHAR_MAX
  BOOST_PRIVATE_INT_EXACT_BUILDER( short, 4 );
  BOOST_PRIVATE_UINT_EXACT_BUILDER( unsigned short, 4 );
#endif
  BOOST_PRIVATE_INT_EXACT_BUILDER( signed char, 5 );
  BOOST_PRIVATE_UINT_EXACT_BUILDER( unsigned char, 5 );

#undef BOOST_PRIVATE_INT_EXACT_BUILDER
#undef BOOST_PRIVATE_UINT_EXACT_BUILDER

  //  map an extreme value to a category
  template < intmax_t MaxValue >
  struct int_max_rank_helper
  {
      BOOST_STATIC_CONSTANT( int, rank = (MaxValue > 0) * (
#ifdef BOOST_HAS_LONG_LONG
       (MaxValue <= integer_traits< long_long_type >::const_max) +
#elif defined(BOOST_HAS_MS_INT64)
       (MaxValue <= integer_traits< __int64 >::const_max) +
#else
       1 +
#endif
       (MaxValue <= integer_traits< long >::const_max) +
       (MaxValue <= integer_traits< int >::const_max) +
       (MaxValue <= integer_traits< short >::const_max) +
       (MaxValue <= integer_traits< signed char >::const_max)) );
  };

  template < intmax_t MinValue >
  struct int_min_rank_helper
  {
      BOOST_STATIC_CONSTANT( int, rank = (MinValue < 0) * (
#ifdef BOOST_HAS_LONG_LONG
       (MinValue >= integer_traits< long_long_type >::const_min) +
#elif defined(BOOST_HAS_MS_INT64)
       (MinValue >= integer_traits< __int64 >::const_min) +
#else
       1 +
#endif
       (MinValue >= integer_traits< long >::const_min) +
       (MinValue >= integer_traits< int >::const_min) +
       (MinValue >= integer_traits< short >::const_min) +
       (MinValue >= integer_traits< signed char >::const_min)) );
  };

  template < uintmax_t Value >
  struct uint_max_rank_helper
  {
      BOOST_STATIC_CONSTANT( int, rank =
#ifdef BOOST_HAS_LONG_LONG
       (Value <= integer_traits< ulong_long_type >::const_max) +
#elif defined(BOOST_HAS_MS_INT64)
       (Value <= integer_traits< unsigned __int64 >::const_max) +
#else
       1 +
#endif
       (Value <= integer_traits< unsigned long >::const_max) +
       (Value <= integer_traits< unsigned int >::const_max) +
       (Value <= integer_traits< unsigned short >::const_max) +
       (Value <= integer_traits< unsigned char >::const_max) );
  };

  //  convert rank to type, Boost.MPL-style
  template < int Rank, typename Signedness, class Enable = void >
  struct integral_rank_to_type
  {
      BOOST_STATIC_CONSTANT( bool, is_specialized = false );
      // No "signed" nor "type" here
  };

  template < int Rank >
  struct integral_rank_to_type< Rank, signed, typename
   enable_if_c<(lowest_integral_rank <= Rank) && (Rank <=
   highest_integral_rank)>::type >
  {
      BOOST_STATIC_CONSTANT( bool, is_specialized = true );
      BOOST_STATIC_CONSTANT( bool, is_signed = true );
      typedef typename int_least_helper< Rank, signed >::least  type;
  };

  template < int Rank >
  struct integral_rank_to_type< Rank, unsigned, typename
   enable_if_c<(lowest_integral_rank <= Rank) && (Rank <=
   highest_integral_rank)>::type >
  {
      BOOST_STATIC_CONSTANT( bool, is_specialized = true );
      BOOST_STATIC_CONSTANT( bool, is_signed = false );
      typedef typename int_least_helper< Rank, unsigned >::least  type;
  };

} // namespace detail

  //  MPL-compatible integer-mapping class templates  ------------------------//

  //  exact number of bits
  template < int Bits, typename Signedness >
  struct exact_integral
  {
      BOOST_STATIC_CONSTANT( bool, is_specialized = false );
      BOOST_STATIC_CONSTANT( int, bit_count = Bits );
  };

  template < int BitsIncludingSign >
  struct exact_integral< BitsIncludingSign, signed >
      : detail::integral_rank_to_type<
         detail::int_exact_rank_helper<BitsIncludingSign>::rank, signed >
  {
      BOOST_STATIC_CONSTANT( int, bit_count = BitsIncludingSign );
  };

  template < int Bits >
  struct exact_integral< Bits, unsigned >
      : detail::integral_rank_to_type<
         detail::uint_exact_rank_helper<Bits>::rank, unsigned >
  {
      BOOST_STATIC_CONSTANT( int, bit_count = Bits );
  };

  //  integer templates specifying number of bits  ---------------------------//

  //  signed
  template< int Bits >   // minimum bits (including sign) required
  struct int_t 
  {
      typedef typename detail::int_least_helper
        <
          detail::int_rank_helper<Bits>::rank,
        signed>::least  least;
      typedef typename int_fast_t<least>::fast  fast;
  };

  template< int Bits >   // exact bits (including sign) desired
  struct int_exact_t
  {
      typedef typename exact_integral<Bits, signed>::type  exact;
  };

  //  unsigned
  template< int Bits >   // minimum bits required
  struct uint_t 
  {
      typedef typename detail::int_least_helper
        < 
          detail::uint_rank_helper<Bits>::rank,
        unsigned>::least  least;
      typedef typename int_fast_t<least>::fast  fast;
      // int_fast_t<> works correctly for unsigned too, in spite of the name.
  };

  template< int Bits >   // exact bits desired
  struct uint_exact_t
  {
      typedef typename exact_integral<Bits, unsigned>::type  exact;
  };

  //  integer templates specifying extreme value  ----------------------------//

  //  signed
  template< intmax_t MaxValue >   // maximum value to require support
  struct int_max_value_t 
  {
      typedef typename detail::int_least_helper
        <
          detail::int_max_rank_helper<MaxValue>::rank,
        signed>::least  least;
      typedef typename int_fast_t<least>::fast  fast;
  };

  template< intmax_t MinValue >   // minimum value to require support
  struct int_min_value_t 
  {
      typedef typename detail::int_least_helper
        <
          detail::int_min_rank_helper<MinValue>::rank,
        signed>::least  least;
      typedef typename int_fast_t<least>::fast  fast;
  };

  //  unsigned
  template< uintmax_t Value >   // maximum value to require support
  struct uint_value_t 
  {
      typedef typename detail::int_least_helper
        < 
          detail::uint_max_rank_helper<Value>::rank,
        unsigned>::least  least;
      typedef typename int_fast_t<least>::fast  fast;
  };


} // namespace boost

#endif  // BOOST_INTEGER_HPP
