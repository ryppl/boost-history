/* Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_CONVERTER_HPP
#define BOOST_INDEXED_SET_CONVERTER_HPP

namespace boost{

namespace indexed_sets{

namespace detail{

/* converter offers means to access indices of a given indexed_set
 * and for convertibilty between index iterators, so providing a
 * localized access point for get() and project() functions.
 */

template<typename IndexedSet,typename Index>
struct converter
{
  static const Index& index(const IndexedSet& x){return x;}
  static Index&       index(IndexedSet& x){return x;}

  static typename Index::const_iterator const_iterator(
    const IndexedSet& x,typename IndexedSet::node_type* node)
  {
    return x.Index::make_iterator(node);
  }

  static typename Index::iterator iterator(
    IndexedSet& x,typename IndexedSet::node_type* node)
  {
    return x.Index::make_iterator(node);
  }
};

} /* namespace indexed_sets::detail */

} /* namespace indexed_sets */

} /* namespace boost */

#endif
