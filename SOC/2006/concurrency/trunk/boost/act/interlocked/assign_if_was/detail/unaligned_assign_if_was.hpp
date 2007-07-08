/*=============================================================================
    Copyright (c) 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_ASSIGN_IF_WAS_DETAIL_UNALIGNED_AIW_HPP
#define BOOST_ACT_INTERLOCKED_ASSIGN_IF_WAS_DETAIL_UNALIGNED_AIW_HPP

#include <boost/act/interlocked/assign_if_was_fwd.hpp>
#include <boost/act/interlocked/integer/selection.hpp>
#include <boost/act/interlocked/load/load_unordered.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/alignment_of.hpp>
#include <boost/integer/static_log2.hpp>
#include <boost/mpl/if.hpp>
#include <cstddef>
#include <utility>

namespace boost { namespace act { namespace interlocked { namespace detail {

template< typename Type >
struct aligned_relative
{
  typedef typename uint_t< ( sizeof( Type ) * CHAR_BIT ) >::fast type;
};

template< std::size_t TargetAlignment >
struct pow_2_crop
{
  static std::size_t execute( std::size_t unaligned_address )
  {
    std::size_t const mask
      = static_cast< std::size_t >
        ( ~( static_log2< TargetAlignment >::value - 1u ) );

    return static_cast< std::size_t >( unaligned_address & mask );
  }
};

template< std::size_t TargetAlignment >
struct generic_crop
{
  static std::size_t execute( std::size_t unaligned_address )
  {
    return static_cast< std::size_t >
           ( ( unaligned_address / TargetAlignment ) * TargetAlignment );
  }
};

// First is aligned address value, second is offset to unaligned
template< typename Type >
std::pair< std::size_t, std::ptrdiff_t > aligned_address_info( Type& object )
{
  std::size_t const address_value = reinterpret_cast< std::size_t >( &object );

  typedef typename aligned_relative< Type >::type aligned_relative_type;

  std::size_t const target_alignment
    = alignment_of< aligned_relative_type >::value,

                    aligned_address
    = mpl::if_c< ( ( 1u << static_log2< target_alignment >::value )
                   == target_alignment
                 )
               , pow_2_crop< target_alignment >
               , generic_crop< target_alignment >
               >
               ::type::execute( address_value );

  return std::make_pair
         ( aligned_address
         , static_cast< std::ptrdiff_t >( address_value - aligned_address )
         );
}

template< typename Semantics >
struct unaligned_assign_if_was
{
  template< typename LeftType >
  static typename remove_cv< LeftType >::type
  execute( LeftType& left, typename remove_cv< LeftType >::type right
         , typename remove_cv< LeftType >::type expected
         )
  {
    std::pair< std::size_t, std::ptrdiff_t > const object_info
      = aligned_address_info( left );

    typedef typename aligned_relative< LeftType >::type aligned_type;

    aligned_type& aligned_left
      = *reinterpret_cast< aligned_type* >( object_info.first );

    // ToDo: Possibly change semantics here to acquire
    aligned_type aligned_expected = load< unordered >( aligned_left ),
                 aligned_right = aligned_expected;

    typedef typename remove_cv< LeftType >::type unaligned_type;

    unaligned_type& unaligned_right
      = *reinterpret_cast< unaligned_type* >
        ( reinterpret_cast< char* >( &aligned_right ) + object_info.second ),

                  & unaligned_old
      = *reinterpret_cast< unaligned_type* >
        ( reinterpret_cast< char* >( &aligned_expected ) + object_info.second );

    unaligned_old = expected;
    unaligned_right = right;

    aligned_type const aligned_result
      = assign_if_was< Semantics >( aligned_left
                                  , aligned_right
                                  , aligned_expected
                                  );

    return *reinterpret_cast< unaligned_type const* >
           (   reinterpret_cast< char const* >( &aligned_result )
             + object_info.second
           );
  }
};

} } } }

#endif
