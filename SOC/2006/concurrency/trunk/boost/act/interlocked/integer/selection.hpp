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
#include <boost/type_traits/alignment_of.hpp>
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
#include <boost/mpl/modulus.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/filter_view.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/sizeof.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>

namespace boost { namespace act { namespace interlocked {

namespace detail
{
#ifdef BOOST_NO_INT64_T
typedef mpl::vector< int_least8_t, int_least16_t, int_least32_t >
        int_least_types_all;

typedef mpl::vector< uint_least8_t, uint_least16_t, uint_least32_t >
        uint_least_types_all;

typedef mpl::vector< int_fast8_t, int_fast16_t, int_fast32_t >
        int_fast_types_all;

typedef mpl::vector< uint_fast8_t, uint_fast16_t, uint_fast32_t >
        uint_fast_types_all;
#else
typedef mpl::vector< int_least8_t, int_least16_t, int_least32_t, int_least64_t >
        int_least_types_all;

typedef mpl::vector< uint_least8_t, uint_least16_t
                   , uint_least32_t, uint_least64_t
                   > uint_least_types_all;

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

template< typename Sequence, std::size_t Bytes >
struct int_finder_t_impl
{
private:
  typedef typename mpl::find_if
          < Sequence
          , int_type_is_at_least_size< mpl::_1, mpl::size_t< Bytes > >
          >
          ::type it_to_type;

  typedef typename mpl::end< Sequence >::type end;

  typedef is_same< it_to_type, end > no_match;
private:
  BOOST_MPL_ASSERT_MSG( ( mpl::not_< no_match >::value )
                      , TYPE_HAS_NO_INTERLOCKED_SUPPORT
                      , ()
                      );
public:
  typedef typename mpl::eval_if
          < no_match
          , mpl::identity< mpl::void_ >
          , mpl::deref< it_to_type >
          >
          ::type type;
};

template< bool Signed, std::size_t Bytes >
struct int_fast_t_impl
  : int_finder_t_impl< typename mpl::if_c< Signed
                                         , int_fast_types
                                         , uint_fast_types
                                         >
                                         ::type
                     , Bytes
                     > {};

template< typename LeftType, typename RightType >
struct alignment_is_multiple_of_alignment_of
  : mpl::equal_to< mpl::modulus< alignment_of< LeftType >
                               , alignment_of< RightType >
                               >
                 , mpl::size_t< 0 >
                 > {};

template< typename Type >
struct has_aligned_relative_raw
  : alignment_is_multiple_of_alignment_of
    < typename int_fast_t_impl< is_signed< Type >::value, sizeof( Type ) >::type
    , Type
    > {};

#define BOOST_ACT_INTERLOCKED_DETAIL_HAS_LEAST_TYPE_MACRO( r, type, elem )     \
( ( sizeof( type ) * CHAR_BIT ) <= elem ) ||

template< typename Type >
struct has_aligned_relative
  : mpl::and_
    <
      mpl::bool_< ( BOOST_PP_SEQ_FOR_EACH
                    ( BOOST_ACT_INTERLOCKED_DETAIL_HAS_LEAST_TYPE_MACRO
                    , Type
                    , BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_SEQ
                    )
                    false
                  )
                >
    , has_aligned_relative_raw< Type >
    > {};

#undef BOOST_ACT_INTERLOCKED_DETAIL_HAS_LEAST_TYPE_MACRO

typedef mpl::filter_view< int_least_types_all
                        , has_aligned_relative< mpl::_1 >
                        >
                        int_least_types;

typedef mpl::filter_view< uint_least_types_all
                        , has_aligned_relative< mpl::_1 >
                        >
                        uint_least_types;

template< bool Signed, std::size_t Bytes >
struct int_least_t_impl
  : int_finder_t_impl< typename mpl::if_c< Signed
                                         , int_least_types
                                         , uint_least_types
                                         >
                                         ::type
                     , Bytes
                     > {};

template< typename Type, typename Sequence >
struct is_unaligned_interlocked_raw
{
private:
  typedef typename mpl::find_if
          < Sequence
          , mpl::and_< mpl::equal_to< mpl::sizeof_< mpl::_1 >
                                    , typename mpl::sizeof_< Type >::type
                                    >
                     , mpl::equal_to< alignment_of< mpl::_1 >
                                    , alignment_of< Type >
                                    >
                     >
          >
          ::type it_to_type;
public:
  typedef is_same< it_to_type, typename mpl::end< Sequence >::type > type;
};


template< typename Type >
struct is_unaligned_interlocked
  : is_unaligned_interlocked_raw< Type, typename mpl::if_< is_signed< Type >
                                                         , int_fast_types
                                                         , uint_fast_types
                                                         >
                                                         ::type
                                >
                                ::type {};

}

template< typename LeastInt >
struct int_fast_t
{
private:
  BOOST_MPL_ASSERT_MSG( is_integral< LeastInt >::value
                      , ARGUMENTS_TO_INT_FAST_T_MUST_BE_INTEGRAL
                      , ( LeastInt )
                      );
public:
  typedef typename detail::int_fast_t_impl
          < is_signed< LeastInt >::value
          , sizeof( LeastInt )
          >
          ::type fast;
};

template< std::size_t Bits >
struct int_t
{
  typedef typename detail::int_least_t_impl
          < true // true == Signed
          , ( Bits / CHAR_BIT )
          >
          ::type least;

  typedef typename int_fast_t< least >::fast fast;
};

template< std::size_t Bits >
struct uint_t
{
  typedef typename detail::int_least_t_impl
          < false // false == Unsigned
          , ( Bits / CHAR_BIT )
          >
          ::type least;

  typedef typename int_fast_t< least >::fast fast;
};

/*
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
