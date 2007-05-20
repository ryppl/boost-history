/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONSUMER_POLICY_DEFAULT_NORMALIZATION_INFO_FWD_HPP
#define BOOST_ACT_CONSUMER_POLICY_DEFAULT_NORMALIZATION_INFO_FWD_HPP

namespace boost { namespace act {

// Note: Defined here (in fwd header) to potential errors when working with an
//       incomple type (avoids false "has default normalization info" checks)
template< typename Type >
struct default_normalization_info
{
  typedef void has_no_default_normalization_info;
};

template< typename Type >
struct default_normalizer;

template< typename Type >
struct default_denormalizer;

} }

#endif
