//  boost/rank_list/detail/node_with_data.hpp

//  Copyright (c) Universidad de Alcala 2006
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/rank_list

/*
  detail/node_with_data.hpp
  -------------------------

  The class rank_list_node is where the payload value_type is
  aggregated. This class inherits all the tree stuff from
  rank_list_node_tree_fields (see detail/node.hpp). A special
  "copy" constructor is provided for copying from value_type.
  Therefore, construction from scratch and normal copy
  construction need to be defined too (they work just like the
  default constructors would).
  
  See accompanying documentation for more details.

                                    November, 2006
                                    Martin Knoblauch Revuelta
                                    Universidad de Alcala (Spain)
                                    martin at aut.uah.es
*/

#ifndef _BOOST_RANK_LIST_NODE_WITH_DATA_HPP_
#define _BOOST_RANK_LIST_NODE_WITH_DATA_HPP_

#ifndef _BOOST_RANK_LIST_HPP_
#error "Don't include this file. Include rank_list.hpp instead."
#endif

namespace boost
{
  namespace rank_lists
  {
    namespace detail
    {

//////////////////////////////////////////////////////////////////

template<class T, class A,
         class W, class P>       // Tree node (with payload T)
class rank_list_node
  : private rank_list_node_tree_fields<T,A,W,P>
{
  friend class boost::rank_lists::rank_list<T,A,W,P>;

  typedef rank_list_node_tree_fields<T,A,W,P>    node_t;
  typedef rank_list_node<T,A,W,P>                payload_node_t;

  typedef typename
          rank_list<T,A,W,P>::value_type         value_type;
  typedef typename
          rank_list<T,A,W,P>::const_reference    const_reference;

  private:  // Only rank_list<T,A,W,P> has access to this class

    // [ Node links and counters: inherited from base class ]

    value_type m_data;   // Payload

    rank_list_node () {}                      // Default ctor.

    rank_list_node (const payload_node_t & n) // Copy ctor.
      : node_t(n),
        m_data(n.m_data) {}

    rank_list_node (const_reference t)        // "Copy" ctor.
      : m_data(t) {}                          // (from T)
};

//////////////////////////////////////////////////////////////////

    }  // namespace detail
  }  // namespace rank_lists
}  // namespace boost

#endif

