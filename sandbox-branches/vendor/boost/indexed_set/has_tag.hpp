/* Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_HAS_TAG_HPP
#define BOOST_INDEXED_SET_HAS_TAG_HPP

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include <boost/mpl/contains.hpp>

namespace boost{

namespace indexed_sets{

namespace detail{

/* determines whether an index type has a given tag in its tag list */

template<typename Tag>
struct has_tag
{
  template<typename Index>
  struct apply:mpl::contains<BOOST_DEDUCED_TYPENAME Index::tag_list,Tag>
  {
  }; 
};

} /* namespace indexed_sets::detail */

} /* namespace indexed_sets */

} /* namespace boost */

#endif
