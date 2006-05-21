/* Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_INDEX_OPS_HPP
#define BOOST_INDEXED_SET_INDEX_OPS_HPP

namespace boost{

namespace indexed_sets{

namespace detail{

/* Common code for index memfuns having templatized and
 * non-templatized versions.
 */

template<
  typename Node,typename KeyFromValue,
  typename CompatibleKey,typename CompatibleCompare
>
inline Node* index_find(
  Node* header,const KeyFromValue& key,const CompatibleKey& x,const CompatibleCompare& comp)
{
  Node* y=header;
  Node* z=Node::from_impl(header->parent());
    
  while (z){
    if(!comp(key(z->value),x)){
      y=z;
      z=Node::from_impl(z->left());
    }
    else z=Node::from_impl(z->right());
  }
    
  return (y==header||comp(x,key(y->value)))?header:y;
}

template<
  typename Node,typename KeyFromValue,
  typename CompatibleKey,typename CompatibleCompare
>
inline Node* index_lower_bound(
  Node* header,const KeyFromValue& key,const CompatibleKey& x,const CompatibleCompare& comp)
{
  Node* y=header;
  Node* z=Node::from_impl(header->parent());

  while(z){
    if(!comp(key(z->value),x)){
      y=z;
      z=Node::from_impl(z->left());
    }
    else z=Node::from_impl(z->right());
  }

  return y;
}

template<
  typename Node,typename KeyFromValue,
  typename CompatibleKey,typename CompatibleCompare
>
inline Node* index_upper_bound(
  Node* header,const KeyFromValue& key,const CompatibleKey& x,const CompatibleCompare& comp)
{
  Node* y=header;
  Node* z=Node::from_impl(header->parent());

  while(z){
    if(comp(x,key(z->value))){
      y=z;
      z=Node::from_impl(z->left());
    }
    else z=Node::from_impl(z->right());
  }

  return y;
}

} /* namespace indexed_sets::detail */

} /* namespace indexed_sets */

} /* namespace boost */

#endif
