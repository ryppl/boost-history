/* Copyright Joaquín M López Muñoz 2003. Use, modification, and distribution
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

/* converter upcasts a given indexed_set to one of its indices.
 * This class is friend to indexed_set, so it provides a localized
 * point for get() functions.
 */

template<typename IndexedSet,typename Index>
struct converter
{
  const Index& operator()(const IndexedSet&x)const{return x;}
  Index& operator()(IndexedSet &x)const{return x;}
};

} /* namespace indexed_sets::detail */

} /* namespace indexed_sets */

} /* namespace boost */

#endif
