#ifndef BOOST_MULTI_INDEX_ORDERED_INDEX_PARTITION_POINT_HPP
#define BOOST_MULTI_INDEX_ORDERED_INDEX_PARTITION_POINT_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include <boost/multi_index/ordered_index.hpp>

namespace boost {

/* Template functions lower_bound, upper_bound, equal_range and binary_search
expressed in terms of partition_point, special version for iterators ranging in
multi_index_container's index

NodeBase is a base type of the multi_index_container's node (internal data
structure used to store elements). */

#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
template< typename NodeBase, typename OrderedIndex, typename UnaryPred >
boost::multi_index::safe_mode::safe_iterator<
  boost::multi_index::detail::bidir_node_iterator<boost::multi_index::detail::ordered_index_node<NodeBase> >,
  OrderedIndex
>
partition_point( 
  boost::multi_index::safe_mode::safe_iterator<
    boost::multi_index::detail::bidir_node_iterator<boost::multi_index::detail::ordered_index_node<NodeBase> >,
    OrderedIndex
  > first,
  boost::multi_index::safe_mode::safe_iterator<
    boost::multi_index::detail::bidir_node_iterator<boost::multi_index::detail::ordered_index_node<NodeBase> >,
    OrderedIndex
  > last,
  UnaryPred pred
)
#else
template< typename NodeBase, typename UnaryPred >
boost::multi_index::detail::bidir_node_iterator<boost::multi_index::detail::ordered_index_node<NodeBase> >
partition_point( 
  boost::multi_index::detail::bidir_node_iterator<boost::multi_index::detail::ordered_index_node<NodeBase> > first,
  boost::multi_index::detail::bidir_node_iterator<boost::multi_index::detail::ordered_index_node<NodeBase> > last,
  UnaryPred pred
)
#endif
{
  typedef boost::multi_index::detail::ordered_index_node<NodeBase> node_type;

  if(first==last) return last;

  /* find special nodes of the underlying internal tree: root and header;
  header points to the end of the index and is used here instead of end() */
  node_type* root=first.get_node();
  node_type* header=
    node_type::from_impl(root->parent());
  node_type* next=
    node_type::from_impl(header->parent());
  while(root!=next) {
    root=header;
    header=next;
    next=node_type::from_impl(next->parent());
  }

  /* find the node of the partition point, using binary search */
  node_type *top = root;
  node_type *i = header;
  while(top) {
    if(pred(top->value())){
	  top=node_type::from_impl(top->right());
	} else {
      i=top;
      top=node_type::from_impl(top->left());
	}
  }

  /* find its projection on the interval */
  if(i==header)
    return last;
  if((last.get_node()!=header) && (pred(*last) && !pred(i->value())))
      return last;
  if(!pred(*first) || pred(i->value()))
    return first;
#if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
  return boost::multi_index::safe_mode::safe_iterator<boost::multi_index::detail::bidir_node_iterator<boost::multi_index::detail::ordered_index_node<NodeBase> >, OrderedIndex>(i, const_cast<OrderedIndex*>(first.owner()));
#else
  return boost::multi_index::detail::bidir_node_iterator<boost::multi_index::detail::ordered_index_node<NodeBase> >(i);
#endif
}

}; /* namespace */


#endif // BOOST_MULTI_INDEX_ORDERED_INDEX_PARTITION_POINT_HPP
