/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONSUMER_POLICY_DEFAULT_NORMALIZATION_INFO_HPP
#define BOOST_ACT_CONSUMER_POLICY_DEFAULT_NORMALIZATION_INFO_HPP

#include <boost/act/consumer_policy/default_normalization_info_fwd.hpp>

#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/mpl/assert.hpp>

namespace boost { namespace act {

// ToDo: Move to separate header

namespace detail
{

BOOST_MPL_HAS_XXX_TRAIT_DEF( has_no_default_normalization_info )
BOOST_MPL_HAS_XXX_TRAIT_DEF( normalizer )
BOOST_MPL_HAS_XXX_TRAIT_DEF( denormalizer )

template< typename Type >
struct get_normalizer
{
  typedef typename Type::normalizer type;
};

template< typename Type >
struct get_denormalizer
{
  typedef typename Type::denormalizer type;
};


template< typename Type >
struct has_default_normalization_info
  : mpl::not_< has_has_no_default_normalization_info< Type > >
{
};

}

template< typename Type >
struct default_normalizer
  : mpl::if_< detail::has_default_normalization_info< Type >
            , detail::get_normalizer< Type >
            , mpl::empty_base
            >
            ::type
{
  BOOST_MPL_ASSERT_MSG( detail::has_default_normalization_info< Type >::value
                      , CONSUMER_RESTRAINT_HAS_NO_DEFAULT_NORMALIZATION
                      , (Type)
                      );
};

template< typename Type >
struct default_denormalizer
  : mpl::if_< detail::has_default_normalization_info< Type >
            , detail::get_denormalizer< Type >
            , mpl::empty_base
            >
            ::type
{
  BOOST_MPL_ASSERT_MSG( detail::has_default_normalization_info< Type >::value
                      , CONSUMER_RESTRAINT_HAS_NO_DEFAULT_NORMALIZATION
                      , (Type)
                      );
};

} }

#endif
