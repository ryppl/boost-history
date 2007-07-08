/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_ASSIGN_ASSIGN_RELEASE_HPP
#define BOOST_ACT_INTERLOCKED_ASSIGN_ASSIGN_RELEASE_HPP

#include <boost/act/config/interlocked/has.hpp>

#if BOOST_ACT_INTERLOCKED_HAS( assign, release )

#include <boost/act/interlocked/semantics/release.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/remove_volatile.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/act/interlocked/detail/interlocked_operand_validators.hpp>
#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>
#include <boost/act/interlocked/integer/selection.hpp>
#include <boost/act/interlocked/modify/modify_release.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <boost/act/interlocked/detail/impl.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_IMPL_INFO                                 \
( assign, release )

#include BOOST_ACT_INTERLOCKED_DETAIL_IMPL_BEGIN()

namespace boost { namespace act { namespace interlocked {

namespace detail
{

template< typename OperandType >
class assign_release_fun
{
public:
  explicit assign_release_fun( OperandType const& operand_init )
    : operand_m( operand_init ) {}

  template< typename Type >
  typename remove_cv< Type >::type operator ()( Type& ) const
  {
    return static_cast< typename remove_cv< Type >::type >( operand_m );
  }
private:
  OperandType const& operand_m;
};

struct assign_release_unaligned
{
  template< typename TargetType, typename SourceType >
  static typename remove_cv< TargetType >::type
  execute( TargetType& destination, SourceType const& source )
  {
    return modify< release >( destination
                            , assign_release_fun< SourceType >( source )
                            );
  }
};

}

template< typename Semantics, typename TargetType, typename SourceType >
typename lazy_enable_if
<
  mpl::and_
  <
    is_same< Semantics, release >
  , detail::are_valid_store_style_params< TargetType, SourceType const >
  , mpl::not_< detail::is_interlocked_bool< TargetType > >
  >
, remove_cv< TargetType >
>
::type
assign( TargetType& destination, SourceType const& new_value )
{
  return mpl::if_< detail::is_unaligned_interlocked< TargetType >
                 , detail::assign_release_unaligned
                 , detail::assign_release_impl
                 >
                 ::type::execute( destination, new_value );
}

template< typename Semantics, typename TargetType, typename SourceType >
typename lazy_enable_if
<
  mpl::and_
  <
    is_same< Semantics, release >
  , detail::are_valid_store_style_params< TargetType, SourceType const >
  , detail::is_interlocked_bool< TargetType >
  >
, remove_cv< TargetType >
>
::type
assign( TargetType& destination, SourceType const& new_value )
{
  typedef typename remove_cv< TargetType >::type result_type;

  return result_type
         (
           interlocked::assign< release >
           ( interlocked_bool_internal_value( destination )
           , static_cast< bool >( new_value )
           )
           != 0
         );
}

} } }

#include BOOST_ACT_INTERLOCKED_DETAIL_IMPL_END()

#endif

#endif
