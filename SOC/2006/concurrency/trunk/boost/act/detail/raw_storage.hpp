/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_DETAIL_RAW_STORAGE_HPP
#define BOOST_ACT_DETAIL_RAW_STORAGE_HPP

#include <boost/type_traits/aligned_storage.hpp>
#include <boost/type_traits/alignment_of.hpp>

namespace boost
{
namespace act
{
namespace detail
{

template< typename Type >
struct raw_storage
  : aligned_storage< sizeof( Type ), alignment_of< Type >::value >
{
};

}
}
}

#endif
