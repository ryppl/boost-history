/* Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_SEQ_INDEX_OPS_HPP
#define BOOST_INDEXED_SET_SEQ_INDEX_OPS_HPP

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include <boost/indexed_set/auto_space.hpp>
#include <boost/indexed_set/seq_index_node.hpp>
#include <boost/indexed_set/try_catch.hpp>
#include <boost/limits.hpp>
#include <cstddef>

namespace boost{

namespace indexed_sets{

namespace detail{

/* Common code for sequenced_index memfuns having templatized and
 * non-templatized code.
 */

template <typename SequencedIndex,typename Predicate>
void sequenced_index_remove(SequencedIndex& x,Predicate pred)
{
  typedef typename SequencedIndex::iterator iterator;
  iterator first=x.begin(),last=x.end();
  while(first!=last){
    if(pred(*first))x.erase(first++);
    else ++first;
  }
}

template <typename SequencedIndex,class BinaryPredicate>
void sequenced_index_unique(SequencedIndex& x,BinaryPredicate binary_pred)
{
  typedef typename SequencedIndex::iterator iterator;
  iterator first=x.begin();
  iterator last=x.end();
  if(first!=last){
    for(iterator middle=first;++middle!=last;middle=first){
      if(binary_pred(*middle,*first))x.erase(middle);
      else first=middle;
    }
  }
}

template <typename SequencedIndex,typename Compare>
void sequenced_index_merge(SequencedIndex& x,SequencedIndex& y,Compare comp)
{
  typedef typename SequencedIndex::iterator iterator;
  if(x!=y){
    iterator first0=x.begin(),last0=x.end();
    iterator first1=y.begin(),last1=y.end();
    while(first0!=last0&&first1!=last1){
      if(comp(*first1,*first0))x.splice(first0,y,first1++);
      else ++first0;
    }
    x.splice(last0,y,first1,last1);
  }
}

/* sorting  */

/* auxiliary stuff */

template<typename Node,typename Compare>
void sequenced_index_collate(Node* x,Node* y,Compare comp);

template<typename Node,typename Compare,typename Allocator>
void sequenced_index_sort(Node* header,Compare comp,const Allocator& al)
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

  auto_space<Node,Allocator> carry_spc(al);
  Node&                      carry=*carry_spc.data();
  auto_space<Node,Allocator> counter_spc(al,max_fill);
  Node*                      counter=counter_spc.data();
  std::size_t                fill=0;

  carry.prior()=carry.next()=carry.impl();
  counter[0].prior()=counter[0].next()=counter[0].impl();

  BOOST_INDEXED_SET_TRY{
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
  }
  BOOST_INDEXED_SET_CATCH(...)
  {
    sequenced_index_node_impl::relink(header->impl(),carry.next(),carry.impl());
    for(std::size_t i=0;i<=fill;++i){
      sequenced_index_node_impl::relink(
        header->impl(),counter[i].next(),counter[i].impl());
    }
    BOOST_INDEXED_SET_RETHROW;
  }
}

template<typename Node,typename Compare>
void sequenced_index_collate(Node* x,Node* y,Compare comp)
{
  Node* first0=Node::from_impl(x->next());
  Node* last0=x;
  Node* first1=Node::from_impl(y->next());
  Node* last1=y;
  while(first0!=last0&&first1!=last1){
    if(comp(first1->value,first0->value)){
      Node* tmp=Node::from_impl(first1->next());
      sequenced_index_node_impl::relink(first0->impl(),first1->impl());
      first1=tmp;
    }
    else first0=Node::from_impl(first0->next());
  }
  sequenced_index_node_impl::relink(last0->impl(),first1->impl(),last1->impl());
}

} /* namespace indexed_sets::detail */

} /* namespace indexed_sets */

} /* namespace boost */

#endif
