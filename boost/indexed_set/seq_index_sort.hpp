/* Copyright Joaquín M López Muñoz 2003. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_SEQ_INDEX_SORT_HPP
#define BOOST_INDEXED_SET_SEQ_INDEX_SORT_HPP

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include <boost/indexed_set/auto_space.hpp>
#include <boost/indexed_set/seq_index_node.hpp>
#include <boost/indexed_set/scope_guard.hpp>
#include <boost/limits.hpp>
#include <cstddef>

namespace boost{

namespace indexed_sets{

namespace detail{

/* Sorting routine for use by sequenced_index. Due to the existence of
 * several sort versions (non-templatized and templatized) and some ugly
 * interactions with BOOST_NO_MEMBER_TEMPLATES, it is convenient to
 * implement this stuff separately from sequenced_index proper.
 */

template<typename NodeType,typename Compare,typename Allocator>
inline void sequenced_index_sort(NodeType* header,Compare comp,const Allocator& al)
{
  /* Musser's mergesort, see http://www.cs.rpi.edu/~musser/gp/List/lists1.html.
   * The implementation is a little convoluted: in the original code
   * counter elements and carry are std::lists: here we do not want
   * to use indexed_sets instead, so we do things at a lower level, managing
   * directly the internal node representation.
   * Incidentally, the implementations I've seen of this algorithm (SGI,
   * Dinkumware, STLPort) are not exception-safe: this is.
   */

  if(header->next()==header->impl()||
     header->next()->next()==header->impl())return;

  BOOST_STATIC_CONSTANT(
    std::size_t,max_fill=(std::size_t)std::numeric_limits<std::size_t>::digits+1);

  auto_space<NodeType,Allocator>          carry_spc(al);
  NodeType&                               carry=*carry_spc.data();
  auto_space<NodeType,Allocator,max_fill> counter_spc(al);
  NodeType*                               counter=counter_spc.data();
  std::size_t                             fill=0;

  carry.prior()=carry.next()=carry.impl();
  counter[0].prior()=counter[0].next()=counter[0].impl();

  /* Explicit instantiation of sequenced_index_sort_clean_up helps
   * some compilers.
   */

  void (* clean_up_fun)(NodeType*,NodeType*,NodeType*,const std::size_t*)=
    sequenced_index_sort_clean_up<NodeType>;

  scope_guard exception_cleaner=make_guard(clean_up_fun,header,&carry,counter,&fill); 

  while(header->next()!=header->impl()){
    sequenced_index_node_impl::relink(carry.next(),header->next());
    std::size_t i=0;
    while(i<fill&&counter[i].next()!=counter[i].impl()){
      sequenced_index_collate(&counter[i],&carry,comp);
      sequenced_index_node_impl::swap(carry.impl(),counter[i++].impl());
    }
    sequenced_index_node_impl::swap(carry.impl(),counter[i].impl());
    if(i==fill){
      ++fill;
      counter[fill].prior()=counter[fill].next()=counter[fill].impl();
    }
  }

  for(std::size_t i=1;i<fill;++i){
    sequenced_index_collate(&counter[i],&counter[i-1],comp);
  }
  sequenced_index_node_impl::swap(header->impl(),counter[fill-1].impl());
  exception_cleaner.dismiss();
}

/* auxiliary stuff */

template<typename NodeType,typename Compare>
inline void sequenced_index_collate(NodeType* x,NodeType* y,Compare comp)
{
  NodeType* first0=NodeType::from_impl(x->next());
  NodeType* last0=x;
  NodeType* first1=NodeType::from_impl(y->next());
  NodeType* last1=y;
  while(first0!=last0&&first1!=last1){
    if(comp(first1->value,first0->value)){
      NodeType* tmp=NodeType::from_impl(first1->next());
      sequenced_index_node_impl::relink(first0->impl(),first1->impl());
      first1=tmp;
    }
    else first0=NodeType::from_impl(first0->next());
  }
  sequenced_index_node_impl::relink(last0->impl(),first1->impl(),last1->impl());
}

template<typename NodeType>
inline void sequenced_index_sort_clean_up(
  NodeType* header,NodeType* carry,NodeType* counter,const std::size_t* fill)
{
  sequenced_index_node_impl::relink(header->impl(),carry->next(),carry->impl());
  for(std::size_t i=0;i<=*fill;++i){
    sequenced_index_node_impl::relink(
      header->impl(),counter[i].next(),counter[i].impl());
  }
}

} /* namespace indexed_sets::detail */

} /* namespace indexed_sets */

} /* namespace boost */

#endif
