/* Copyright Joaquín M López Muñoz 2003. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_INDEX_PROXY_HPP
#define BOOST_INDEXED_SET_INDEX_PROXY_HPP

#if defined(BOOST_INDEXED_SET_ENABLE_SAFE_MODE)
#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include <boost/detail/workaround.hpp>

#if BOOST_WORKAROUND(BOOST_MSVC,<1300)
#include <algorithm>
#include <boost/indexed_set/index_iterator_fwd.hpp>
#include <boost/indexed_set/safe_mode.hpp>

namespace boost{

namespace indexed_sets{

namespace detail{

/* In safe mode, index iterators are derived from safe_iterator<Index>,
 * where Index is the type of the index where the iterator belongs. Due
 * to the long symbol names of indices, MSVC++ 6.0 often issues a
 * LNK1179 (duplicate comdat) error. To workaround this problem,
 * index_proxy is used instead. index_proxy<Node> acts as an index
 * over nodes of type Node in all aspects relevant to safe_iterator, and
 * its shorter symbol name makes life easier for MSVC++ 6.0.
 * index_proxy provides its own index_proxy_iterator that interoperates
 * with real index iterators with respect to safe mode checks.
 */

template<typename Node>
class index_proxy_iterator
{
public:
  index_proxy_iterator(Node* node_):node(node_){}

  Node* get_node()const{return node;}
private:
  Node* node;
};

template<typename Node>
class index_proxy:public safe_container<index_proxy<Node> >
{
protected:
  index_proxy(Node* header_):header(header_){}

  void swap(index_proxy<Node>& x)
  {
    std::swap(header,x.header);
    safe_container<index_proxy<Node> >::swap(x);
  }

public:
  index_proxy_iterator<Node> begin()const
  {
    return index_proxy_iterator<Node>(Node::from_impl(header->left()));
  }

  index_proxy_iterator<Node> end()const
  {
    return index_proxy_iterator<Node>(header);
  }

private:
  Node* header;
};

template<typename Node>
bool operator==(const index_iterator<Node>& x,const index_proxy_iterator<Node>& y)
{
  return x.get_node()==y.get_node();
}

template<typename Node>
bool operator==(const index_proxy_iterator<Node>& x,const index_iterator<Node>& y)
{
  return y==x;
}

template<typename Node>
bool operator!=(const index_iterator<Node>& x,const index_proxy_iterator<Node>& y)
{
  return !(x==y);
}

template<typename Node>
bool operator!=(const index_proxy_iterator<Node>& x,const index_iterator<Node>& y)
{
  return !(y==x);
}

} /* namespace indexed_sets::detail */

} /* namespace indexed_sets */

} /* namespace boost */

#endif /* workaround */

#endif /* BOOST_INDEXED_SET_ENABLE_SAFE_MODE */

#endif
