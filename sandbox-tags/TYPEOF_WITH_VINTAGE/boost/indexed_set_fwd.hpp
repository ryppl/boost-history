/* Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_FWD_HPP
#define BOOST_INDEXED_SET_FWD_HPP

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include <boost/indexed_set/identity.hpp>
#include <boost/indexed_set/index_fwd.hpp>
#include <boost/indexed_set/index_list.hpp>
#include <memory>

namespace boost{

namespace indexed_sets{

template<
  typename Value,
  typename IndexSpecifierList=index_list<unique<identity<Value> > >,
  typename Allocator=std::allocator<Value> >
class indexed_set;

template<typename Value,typename IndexSpecifierList,typename Allocator>
void swap(
  indexed_set<Value,IndexSpecifierList,Allocator>& x,
  indexed_set<Value,IndexSpecifierList,Allocator>& y);

template<typename IndexedSet,int N>
struct nth_index_type;

template<typename IndexedSet,typename Tag>
struct index_type;

template<typename IndexedSet,int N>
struct nth_iterator_type;

template<typename IndexedSet,int N>
struct nth_const_iterator_type;

template<typename IndexedSet,typename Tag>
struct iterator_type;

template<typename IndexedSet,typename Tag>
struct const_iterator_type;

/* get and project functions not fwd declared due to problems
 * with dependent typenames
 */

template<typename Value,typename IndexSpecifierList,typename Allocator>
bool operator==(
  const indexed_set<Value,IndexSpecifierList,Allocator>& x,
  const indexed_set<Value,IndexSpecifierList,Allocator>& y);

template<typename Value,typename IndexSpecifierList,typename Allocator>
bool operator<(
  const indexed_set<Value,IndexSpecifierList,Allocator>& x,
  const indexed_set<Value,IndexSpecifierList,Allocator>& y);

template<typename Value,typename IndexSpecifierList,typename Allocator>
bool operator!=(
  const indexed_set<Value,IndexSpecifierList,Allocator>& x,
  const indexed_set<Value,IndexSpecifierList,Allocator>& y);

template<typename Value,typename IndexSpecifierList,typename Allocator>
bool operator>(
  const indexed_set<Value,IndexSpecifierList,Allocator>& x,
  const indexed_set<Value,IndexSpecifierList,Allocator>& y);

template<typename Value,typename IndexSpecifierList,typename Allocator>
bool operator>=(
  const indexed_set<Value,IndexSpecifierList,Allocator>& x,
  const indexed_set<Value,IndexSpecifierList,Allocator>& y);

template<typename Value,typename IndexSpecifierList,typename Allocator>
bool operator<=(
  const indexed_set<Value,IndexSpecifierList,Allocator>& x,
  const indexed_set<Value,IndexSpecifierList,Allocator>& y);

} /* namespace indexed_sets */

/* indexed_set, being the main type of this library, is promoted to
 * namespace boost.
 */

using indexed_sets::indexed_set;

} /* namespace boost */

#endif
