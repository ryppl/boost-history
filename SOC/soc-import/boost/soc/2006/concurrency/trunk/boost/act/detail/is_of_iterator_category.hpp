//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_IS_OF_ITERATOR_CATEGORY_HPP
#define BOOST_ACT_DETAIL_IS_OF_ITERATOR_CATEGORY_HPP

#include <boost/type_traits/is_same.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_traits.hpp>

namespace boost
{
namespace act
{
namespace detail
{

template< typename IteratorType, typename IteratorCategory >
struct is_of_iterator_category
  : ::boost::is_same< typename ::boost::iterator_category< IteratorType >::type
                    , IteratorCategory
                    >
{
};

}
}
}

#endif
