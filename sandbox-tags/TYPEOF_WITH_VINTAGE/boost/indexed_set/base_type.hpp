/* Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_BASE_TYPE_HPP
#define BOOST_INDEXED_SET_BASE_TYPE_HPP

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include <boost/detail/workaround.hpp>
#include <boost/mpl/bind.hpp>
#include <boost/mpl/iter_fold_backward.hpp>
#include <boost/indexed_set_fwd.hpp>
#include <boost/indexed_set/header_holder.hpp>
#include <boost/indexed_set/index_fwd.hpp>
#include <boost/indexed_set/msvc_index_specifier.hpp>
#include <boost/indexed_set/index_base.hpp>
#include <boost/indexed_set/prevent_eti.hpp>
#include <boost/static_assert.hpp>

namespace boost{

namespace indexed_sets{

namespace detail{

/* MPL machinery to construct a linear hierarchy of indices out of
 * a index list.
 */

#if BOOST_WORKAROUND(BOOST_MSVC,<1300)
struct index_applier
{
  template<typename IndexSpecifierIterator,typename Super>
  struct apply:
    msvc_index_specifier<IndexSpecifierIterator::type>::
      template result_index_type<Super>
  {
  }; 
};
#else
struct index_applier
{
  template<typename IndexSpecifierIterator,typename Super>
  struct apply:IndexSpecifierIterator::type::
    BOOST_NESTED_TEMPLATE index_class_type<Super>
  {
  }; 
};
#endif

template<typename Value,typename IndexSpecifierList,typename Allocator>
struct indexed_set_base_type
{
  BOOST_STATIC_ASSERT(detail::is_index_list<IndexSpecifierList>::value);

  typedef typename prevent_eti<
    indexed_set<Value,IndexSpecifierList,Allocator>,
    typename mpl::iter_fold_backward<
      typename IndexSpecifierList::type,
      index_base<Value,IndexSpecifierList,Allocator>,
      mpl::bind2<
        index_applier,
        mpl::_2,
        mpl::_1
      >
    >::type
  >::type type;
};

} /* namespace indexed_sets::detail */

} /* namespace indexed_sets */

} /* namespace boost */

#endif
