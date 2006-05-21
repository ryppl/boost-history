/* Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_NODE_TYPE_HPP
#define BOOST_INDEXED_SET_NODE_TYPE_HPP

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include <boost/detail/workaround.hpp>
#include <boost/mpl/bind.hpp>
#include <boost/mpl/iter_fold_backward.hpp>
#include <boost/indexed_set_fwd.hpp>
#include <boost/indexed_set/header_holder.hpp>
#include <boost/indexed_set/index_node_base.hpp>
#include <boost/indexed_set/msvc_index_specifier.hpp>
#include <boost/indexed_set/prevent_eti.hpp>
#include <boost/static_assert.hpp>

namespace boost{

namespace indexed_sets{

namespace detail{

/* MPL machinery to construct the internal node type associated to an index list.
 */

#if BOOST_WORKAROUND(BOOST_MSVC,<1300)
struct index_node_applier
{
  template<typename IndexSpecifierIterator,typename Super>
  struct apply:
    msvc_index_specifier<IndexSpecifierIterator::type>::
      template result_node_type<Super>
  {
  }; 
};
#else
struct index_node_applier
{
  template<typename IndexSpecifierIterator,typename Super>
  struct apply:IndexSpecifierIterator::type::BOOST_NESTED_TEMPLATE node_type<Super>
  {
  }; 
};
#endif

template<typename Value,typename IndexSpecifierList,typename Allocator>
struct indexed_set_node_type
{
  BOOST_STATIC_ASSERT(detail::is_index_list<IndexSpecifierList>::value);

  typedef typename mpl::iter_fold_backward<
    typename IndexSpecifierList::type,
    index_node_base<Value>,
    mpl::bind2<index_node_applier,mpl::_2,mpl::_1>
  >::type type;
};

} /* namespace indexed_sets::detail */

} /* namespace indexed_sets */

} /* namespace boost */

#endif
