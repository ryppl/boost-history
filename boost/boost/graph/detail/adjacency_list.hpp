// -*- c++ -*-
//===========================================================================
//  CVS Information:                                                         
//                                                                           
//     $RCSfile$  $Revision$  $State$ 
//     $Author$  $Date$ 
//     $Locker$ 
//---------------------------------------------------------------------------
//                                                                           
// DESCRIPTION                                                               
//                                                                           
//---------------------------------------------------------------------------
//                                                                           
// LICENSE AGREEMENT                                                         
//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// This file is part of the Boost Graph Library
//
// You should have received a copy of the License Agreement for the
// Boost Graph Library along with the software; see the file LICENSE.
// If not, contact Office of Research, University of Notre Dame, Notre
// Dame, IN 46556.
//
// Permission to modify the code and to distribute modified code is
// granted, provided the text of this NOTICE is retained, a notice that
// the code was modified is included with the above COPYRIGHT NOTICE and
// with the COPYRIGHT NOTICE in the LICENSE file, and that the LICENSE
// file is distributed with the modified code.
//
// LICENSOR MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.
// By way of example, but not limitation, Licensor MAKES NO
// REPRESENTATIONS OR WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
// PARTICULAR PURPOSE OR THAT THE USE OF THE LICENSED SOFTWARE COMPONENTS
// OR DOCUMENTATION WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS, TRADEMARKS
// OR OTHER RIGHTS.
//=======================================================================
//---------------------------------------------------------------------------
//                                                                           
// REVISION HISTORY:                                                         
//                                                                           
// $Log$
// Revision 1.1  2000/09/18 08:17:13  jsiek
// new files for boost graph library
//
// Revision 1.12  2000/09/13 05:03:53  jsiek
//  work on visitors
//
// Revision 1.11  2000/09/12 01:25:44  jsiek
// size_type -> vertices_size_type, etc.
//
// Revision 1.10  2000/09/06 22:54:45  llee1
// *** empty log message ***
//
// Revision 1.9  2000/09/05 21:05:57  llee1
// *** empty log message ***
//
// Revision 1.8  2000/09/05 20:36:44  llee1
// change library name in the header comment
//
//===========================================================================

#ifndef BOOST_GRAPH_DETAIL_ADJACENCY_LIST_HPP
#define BOOST_GRAPH_DETAIL_ADJACENCY_LIST_HPP

#include <boost/config.hpp>
#include <boost/property_accessor.hpp>
#include <boost/operators.hpp>
#include <boost/pending/iterator_adaptors.hpp>

#ifndef BOOST_NO_ITERATOR_ADAPTORS
#include <boost/pending/iterator_adaptors.hpp>
#else
#include <boost/graph/detail/incidence_iterator.hpp>
#endif
#include <boost/pending/ct_if.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/detail/edge.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/detail/adj_list_edge_iterator.hpp>
#include <boost/graph/properties.hpp>
#include <boost/pending/plugin.hpp>

// The helper classes are defined at global namespace scope to
// avoid a g++ bug in dealing with friend functions in namespaces.

namespace boost {

  namespace detail {

    template <typename DirectedS>
    struct directed_category_traits {
      typedef directed_tag directed_category;
    };

    template <>
    struct directed_category_traits<directedS> {
      typedef directed_tag directed_category;
    };
    template <>
    struct directed_category_traits<undirectedS> {
      typedef undirected_tag directed_category;
    };
    template <>
    struct directed_category_traits<bidirectionalS> {
      typedef bidirectional_tag directed_category;
    };

#ifndef BOOST_NO_ITERATOR_ADAPTORS
    template <class Vertex, class Traits>
    struct adjacency_iterator_traits {
      typedef Vertex value_type;
      typedef value_type reference;
      typedef value_type* pointer;
      typedef typename Traits::iterator_category iterator_category;
      typedef typename Traits::difference_type difference_type;
    };

    template <class Graph>
    struct adjacency_iterator_policies : 
      public boost::default_iterator_policies
    {
      inline adjacency_iterator_policies() { }
      inline adjacency_iterator_policies(Graph* g) : m_g(g) { }

      template <class Reference, class Iterator>
      inline Reference
      dereference(boost::type<Reference>, const Iterator& i) const
	{ return target(*i, *m_g); }

      Graph* m_g;
    };

    template <class Graph, class Vertex, class OutEdgeIter,
#if !defined BOOST_NO_STD_ITERATOR_TRAITS
	      class Traits = std::iterator_traits<OutEdgeIter>
#else
	      class Traits
#endif
	     >
    struct adjacency_iterator {
      typedef boost::iterator_adaptor<OutEdgeIter, 
	adjacency_iterator_policies<Graph>,
	adjacency_iterator_traits<Vertex, Traits> 
      > type;
    };

#else
    template <class Vertex, class OutEdgeIter, class Graph>
    struct bidir_adjacency_iterator {
    private:
      typedef bidir_adjacency_iterator self;
    public:
      typedef ptrdiff_t difference_type;
      typedef std::forward_iterator_tag iterator_category;
      typedef Vertex* pointer;
      typedef Vertex reference;
      typedef Vertex value_type;
      inline bidir_adjacency_iterator() { }
	  inline bidir_adjacency_iterator(OutEdgeIter ii, Graph* _g)
        : i(ii), g(_g) {}

      inline self& operator++() { ++i; return *this; }
      inline self operator++(int) { self tmp = *this; ++(*this); return tmp; }
      inline reference operator*() const { return target(*i, *g); }
      /* Attention: */
      /* Even if two iterators are not equal, they could be the same vertex! */
      /* i.e.  i != j does not mean *i != *j */

      inline bool operator!=(const self& x) const { return i != x.i; }
      inline bool operator==(const self& x) const { return i == x.i; }

      inline self* operator->() { return this; }
      /*    protected: */
      OutEdgeIter i;
      Graph* g;
    protected:
    };
#endif

    template <class P>
    struct has_plugin { enum { value = true }; };
    template <>
    struct has_plugin<no_plugin> { enum { value = false }; };

    template <class Directed> struct is_random_access { enum { value = false}; };
    template <>
    struct is_random_access<vecS> { enum { value = true }; };


    //=========================================================================
    // Out-Edge and In-Edge Iterator Implementation

#if !defined  BOOST_NO_ITERATOR_ADAPTORS
    template <class EdgeDescriptor, class EdgeIterTraits>
    struct edge_iter_traits {
      typedef EdgeDescriptor value_type;
      typedef value_type reference;
      typedef value_type* pointer;
      typedef typename EdgeIterTraits::iterator_category iterator_category;
      typedef typename EdgeIterTraits::difference_type difference_type;
    };

    template <class VertexDescriptor>
    struct out_edge_iter_policies : public boost::default_iterator_policies
    {
      inline out_edge_iter_policies() { }
      inline out_edge_iter_policies(const VertexDescriptor& src)
        : m_src(src) { }

      template <class EdgeDescriptor, class OutEdgeIter>
      inline EdgeDescriptor
      dereference(boost::type<EdgeDescriptor>, const OutEdgeIter& i) const {
        return EdgeDescriptor(m_src, (*i).get_target(), (*i).get_plugin());
      }
      VertexDescriptor m_src;
    };
    template <class VertexDescriptor>
    struct in_edge_iter_policies : public boost::default_iterator_policies
    {
      inline in_edge_iter_policies() { }
      inline in_edge_iter_policies(const VertexDescriptor& src) 
        : m_src(src) { }

      template <class EdgeDescriptor, class InEdgeIter>
      inline EdgeDescriptor
      dereference(boost::type<EdgeDescriptor>, const InEdgeIter& i) const {
        return EdgeDescriptor((*i).get_target(), m_src, i->get_plugin());
      }
      VertexDescriptor m_src;
    };
#endif

    //=========================================================================
    // Undirected Edge Iterator Implementation

#if !defined BOOST_NO_ITERATOR_ADAPTORS
    struct undirected_edge_iter_policies
      : public boost::default_iterator_policies
    {
      template <class EdgeDescriptor, class InEdgeIter>
      inline EdgeDescriptor
      dereference(boost::type<EdgeDescriptor>, const InEdgeIter& i) const {
        return EdgeDescriptor((*i).m_source, (*i).m_target, i->get_plugin());
      }
    };
#else
    template <class EdgeIter, class Edge>
    struct undirected_edge_iter
    {
      typedef undirected_edge_iter Self;
      typedef Edge value_type;
      typedef Edge reference;
      typedef Edge* pointer;
      typedef std::ptrdiff_t difference_type;
      typedef std::bidirectional_iterator_tag iterator_category;
      inline undirected_edge_iter() { }
      inline undirected_edge_iter(const EdgeIter& i) : m_iter(i) { }
      inline Self& operator++() { ++m_iter; return *this; }
      inline Self operator++(int) { Self t=*this; ++m_iter; return t; }
      inline Self& operator--() { --m_iter; return *this; }
      inline Self operator--(int) { Self t=*this; --m_iter; return t; }
      inline Edge operator*() const {
        return Edge(m_iter->m_source, m_iter->m_target, m_iter->get_plugin());
      }
      inline bool operator==(const Self& x) const { return m_iter == x.m_iter;}
      inline bool operator!=(const Self& x) const { return m_iter != x.m_iter;}
      EdgeIter m_iter;
    };
#endif

    //=========================================================================
    // Edge Storage Types (stored in the out-edge/in-edge lists)

    template <class Vertex>
    class stored_edge
      : public boost::equality_comparable1< stored_edge<Vertex>,
        boost::less_than_comparable1< stored_edge<Vertex> > >
    {
    public:
      inline stored_edge(Vertex target, const no_plugin& = no_plugin())
        : m_target(target) { }
      inline Vertex& get_target() { return m_target; }
      inline const Vertex& get_target() const { return m_target; }
      inline const no_plugin* get_plugin() const { return 0; }
      inline bool operator==(const stored_edge& x) const
        { return m_target == x.get_target(); }
      inline bool operator<(const stored_edge& x) const
        { return m_target < x.get_target(); }
    protected:
      Vertex m_target;
    };

    template <class Vertex, class Plugin>
    class stored_edge_plugin : public stored_edge<Vertex> {
    public:
      inline stored_edge_plugin(Vertex target, const Plugin& p = Plugin())
        : stored_edge<Vertex>(target), m_plugin(p) { }
      inline Plugin* get_plugin() { return &m_plugin; }
      inline const Plugin* get_plugin() const { return &m_plugin; }
    protected:
      Plugin m_plugin;
    };

    template <class Vertex, class Iter, class Plugin>
    class stored_edge_iter 
      : public stored_edge<Vertex> // there is some redundant storage
      // of the target vertex, but it is difficult to remove...
    {
    public:
      inline stored_edge_iter(Vertex v, Iter i = Iter())
        : stored_edge<Vertex>(v), m_iter(i) { }
      inline Plugin* get_plugin() { return m_iter->get_plugin(); }
      inline const Plugin* get_plugin() const { return m_iter->get_plugin(); }
      inline Iter get_iter() const { return m_iter; }
    protected:
      Iter m_iter;
    };
    
  } // namespace detail
    
#ifdef __GNUC__
} // namespace boost
#endif

    //=========================================================================
    // Directed Graph Helper Class

    template <class Config>
    struct directed_edges_helper
    {
      typedef typename Config::graph_type graph_type;
      typedef typename Config::edge_iterator edge_iterator;

      // O(1)
      friend inline std::pair<edge_iterator, edge_iterator>
      edges(const graph_type& g_)
      {
        graph_type& g = const_cast<graph_type&>(g_);
        return std::make_pair( edge_iterator(g.vertex_set().begin(), 
                                             g.vertex_set().begin(), 
                                             g.vertex_set().end(), g),
                               edge_iterator(g.vertex_set().begin(),
                                             g.vertex_set().end(),
                                             g.vertex_set().end(), g) );
      }
    };
    template <class Config, class vertex_descriptor, class Predicate>
    inline void
    remove_out_edge_if(directed_edges_helper<Config>& g_, 
		       vertex_descriptor v, Predicate pred)
    {
      typedef typename Config::graph_type graph_type;
      graph_type& g = static_cast<graph_type&>(g_);
      typedef typename Config::edge_parallel_category edge_parallel_category;
      remove_edge_if_dispatch(g.out_edge_list(v), pred,
			      edge_parallel_category());
    }
    template <class Config, class vertex_descriptor, class Predicate>
    inline void
    remove_in_edge_if(directed_edges_helper<Config>& g_, 
		      vertex_descriptor v, Predicate pred)
    {
      typedef typename Config::graph_type graph_type;
      graph_type& g = static_cast<graph_type&>(g_);
      typedef typename Config::edge_parallel_category edge_parallel_category;
      remove_edge_if_dispatch(g.in_edge_list(v), pred,
			      edge_parallel_category());
    }
    template <class EdgeList, class Predicate>
    inline void
    remove_edge_if_dispatch(EdgeList& el, Predicate pred,
			    boost::allow_parallel_edge_tag)
    {
      typename EdgeList::iterator
	pos = std::remove_if(el.begin(), el.end(), pred);
      el.erase(pos, el.end());
    }
    template <class EdgeList, class Predicate>
    inline void
    remove_edge_if_dispatch(EdgeList& el, Predicate pred,
			    boost::disallow_parallel_edge_tag)
    {
      typename EdgeList::iterator pos = el.begin(), next = el.begin(), 
	end = el.end();
      for (; pos != end; pos = next) {
	++next;
	if (pred(*pos))
	  el.erase(pred);
      }
    }
    // need to make sure remove_edge_if works for undirected & bidirectional
    // and add tests for it in test/graph.cpp

    template <class Config>
    struct directed_graph_helper : public directed_edges_helper<Config>
    {
      typedef typename Config::graph_type graph_type;
      typedef typename Config::vertex_descriptor vertex_descriptor;
      typedef typename Config::vertex_iterator vertex_iterator;
      typedef typename Config::edge_descriptor edge_descriptor;
      typedef typename Config::edge_iterator edge_iterator;
      typedef typename Config::edge_plugin_type edge_plugin_type;
      typedef typename Config::StoredEdge StoredEdge;

      // O(E/V) or O(log(E/V)) depending on OutEdgeList type
      friend inline void
      remove_edge(graph_type& g, vertex_descriptor u, vertex_descriptor v)
      {
        typename Config::OutEdgeList& el = g.out_edge_list(u);
        boost::erase(el, StoredEdge(v));
      }
      // O(V + E) for allow_parallel_edges
      // O(V * log(E/V)) for disallow_parallel_edges
      friend inline void 
      clear_vertex(graph_type& g, vertex_descriptor u)
      {
        vertex_iterator vi, viend;
        for (boost::tie(vi, viend) = vertices(g); vi != viend; ++vi)
          boost::erase(g.out_edge_list(*vi), StoredEdge(u));
        g.out_edge_list(u).clear();
        // clear() should be a req of Sequence and AssociativeContainer,
        // or maybe just Container
      }
      // O(V), could do better...
      friend inline typename Config::edges_size_type
      num_edges(const graph_type& g) 
      {
        typename Config::edges_size_type num_e = 0;
        vertex_iterator vi, vi_end;
        for (boost::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
          num_e += out_degree(*vi, g);
        return num_e;
      }
      // O(1) for allow_parallel_edge_tag
      // O(log(E/V)) for disallow_parallel_edge_tag
      friend inline std::pair<edge_descriptor, bool>
      add_edge(graph_type& g, vertex_descriptor u, vertex_descriptor v,
	       const edge_plugin_type& p = edge_plugin_type())
      {
	typename Config::OutEdgeList::iterator i; 
	bool inserted;
	boost::tie(i, inserted) = boost::push(g.out_edge_list(u), 
					     StoredEdge(v, p));
	return std::make_pair(edge_descriptor(u, v, (*i).get_plugin()), 
			      inserted);
      }
    };

    //=========================================================================
    // Undirected Graph Helper Class

    template <class Vertex, class EdgePlugin>
    struct undirected_edge
      : public boost::detail::edge_base<boost::undirected_tag, Vertex>
    {
      typedef boost::detail::edge_base<boost::undirected_tag, Vertex> Base;
      undirected_edge(Vertex u, Vertex v, const EdgePlugin& p = EdgePlugin())
        : Base(u, v), m_plugin(p) { }
      EdgePlugin* get_plugin() { return &m_plugin; }
      const EdgePlugin* get_plugin() const { return &m_plugin; }
      EdgePlugin m_plugin;
    };

    template <class Vertex>
    struct undirected_edge_no_plugin 
      : public boost::detail::edge_base<boost::undirected_tag, Vertex> {
      typedef boost::detail::edge_base<boost::undirected_tag, Vertex> Base;
      template <class EdgePlugin>
      undirected_edge_no_plugin (Vertex u, Vertex v, const EdgePlugin& )
        : Base(u, v) { }
      boost::no_plugin* get_plugin() { return 0; }
      const boost::no_plugin* get_plugin() const { return 0; }
    };

    template <class Config>
    struct undirected_graph_helper
    {
      typedef typename Config::graph_type graph_type;
      typedef typename Config::vertex_descriptor vertex_descriptor;
      typedef typename Config::edge_descriptor edge_descriptor;
      typedef typename Config::edge_iterator edge_iterator;
      typedef typename Config::adjacency_iterator adjacency_iterator;
      typedef typename Config::edge_plugin_type edge_plugin_type;
      typedef typename Config::StoredEdge StoredEdge;

      // O(1)
      friend inline std::pair<edge_iterator, edge_iterator>
      edges(const graph_type& g_)
      {
        graph_type& g = const_cast<graph_type&>(g_);
        return std::make_pair( edge_iterator(g.m_edges.begin()),
                               edge_iterator(g.m_edges.end()) );
      }
      // O(E/V) or O(log(E/V))
      friend inline void
      remove_edge(graph_type& g, vertex_descriptor u, vertex_descriptor v)
      {
        typedef typename Config::edge_parallel_category Cat;
        remove_edge_and_plugin(g, g.out_edge_list(u), v, Cat());
        boost::erase(g.out_edge_list(v), StoredEdge(u));
      }
      // O(1)
      friend inline typename Config::edges_size_type
      num_edges(const graph_type& g) 
      {
        return g.m_edges.size();
      }
      // O(E/V * E/V) or O(E/V * log(E/V))
      friend inline void 
      clear_vertex(graph_type& g, vertex_descriptor u)
      {
        typename Config::OutEdgeList& el = g.out_edge_list(u);
        typename Config::OutEdgeList::iterator 
          ei = el.begin(), ei_end = el.end();
        for (; ei != ei_end; ++ei) {
          boost::erase(g.out_edge_list((*ei).get_target()), StoredEdge(u));
          g.m_edges.erase((*ei).get_iter());
        }
        g.out_edge_list(u).clear();
      }
      // O(1) for allow_parallel_edge_tag
      // O(log(E/V)) for disallow_parallel_edge_tag
      friend inline std::pair<edge_descriptor, bool>
      add_edge(graph_type& g, vertex_descriptor u, vertex_descriptor v, 
	       const edge_plugin_type& p = edge_plugin_type())
      {
	if ( u != v ) { // don't allow self-loops 
	  bool inserted;
	  typename Config::EdgeContainer::value_type e(u,v,p);
	  g.m_edges.push_back(e);
	  typename Config::EdgeContainer::iterator p_iter 
	    = boost::prior(g.m_edges.end());
	  typename Config::OutEdgeList::iterator i;
	  boost::tie(i,inserted) = boost::push(g.out_edge_list(u), 
					      StoredEdge(v, p_iter));
	  if (inserted) {
	    boost::push(g.out_edge_list(v), StoredEdge(u, p_iter));
	    return std::make_pair(edge_descriptor(u, v, 
						  p_iter->get_plugin()),true);
	  } else {
	    g.m_edges.erase(p_iter);
	    return std::make_pair(edge_descriptor(u, v), false);
	  }
	}
	return std::make_pair(edge_descriptor(u,v), false);
      }
      // O(1)
      friend inline typename Config::degree_size_type
      degree(vertex_descriptor u, const graph_type& g)
      {
	return out_degree(u, g);
      }
    };

    // O(E/V)
    template <class Graph, class EdgeList, class Vertex>
    inline void
    remove_edge_and_plugin(Graph& g, EdgeList& el, Vertex v, 
                           boost::allow_parallel_edge_tag)
    {
      typedef typename EdgeList::value_type StoredEdge;
      typename EdgeList::iterator i = el.begin(), end = el.end();
      for (; i != end; ++i)
        if ((*i).get_target() == v)
          g.m_edges.erase((*i).get_iter());
      boost::erase(el, StoredEdge(v));
    }
    // O(log(E/V))
    template <class Graph, class EdgeList, class Vertex>
    inline void
    remove_edge_and_plugin(Graph& g, EdgeList& el, Vertex v, 
                           boost::disallow_parallel_edge_tag)
    {
      typedef typename EdgeList::value_type StoredEdge;
      typename EdgeList::iterator i = el.find(StoredEdge(v)), end = el.end();
      if (i != end) {
        g.m_edges.erase((*i).get_iter());
        el.erase(i);
      }
    }

    //=========================================================================
    // Bidirectional Graph Helper Class (with edge plugins)

    template <class EdgePlugin>
    struct bidirectional_edge {
      bidirectional_edge(const EdgePlugin& p)
        : m_plugin(p) { }
      EdgePlugin* get_plugin() { return &m_plugin; }
      const EdgePlugin* get_plugin() const { return &m_plugin; }
      EdgePlugin m_plugin;
    };

    template <class Config>
    struct bidirectional_graph_helper_with_plugin
      : public directed_edges_helper<Config>
    {
      typedef typename Config::graph_type graph_type;
      typedef typename Config::vertex_descriptor vertex_descriptor;
      typedef typename Config::edge_descriptor edge_descriptor;
      typedef typename Config::in_edge_iterator in_edge_iterator;
      typedef typename Config::StoredEdge StoredEdge;
      typedef typename Config::edge_plugin_type edge_plugin_type;

      // O(E/V) for allow_parallel_edge_tag
      // O(log(E/V)) for disallow_parallel_edge_tag
      friend inline void
      remove_edge(graph_type& g, vertex_descriptor u, vertex_descriptor v)
      {
        typedef typename Config::edge_parallel_category Cat;
        remove_edge_and_plugin(g, g.out_edge_list(u), v, Cat());
        boost::erase(g.in_edge_list(v), StoredEdge(u));
      }
      // O(1)
      friend inline typename Config::edges_size_type
      num_edges(const graph_type& g) 
      {
        return g.m_edges.size();
      }
      // O(E/V * E/V) for allow_parallel_edge_tag
      // O(E/V * log(E/V)) for disallow_parallel_edge_tag
      friend inline void
      clear_vertex(graph_type& g, vertex_descriptor u)
      {
        typename Config::OutEdgeList& el = g.out_edge_list(u);
        typename Config::OutEdgeList::iterator 
          ei = el.begin(), ei_end = el.end();
        for (; ei != ei_end; ++ei) {
          boost::erase(g.in_edge_list((*ei).get_target()), StoredEdge(u));
          g.m_edges.erase((*ei).get_iter());
        }

        typename Config::InEdgeList& in_el = g.in_edge_list(u);
        typename Config::InEdgeList::iterator 
          in_ei = in_el.begin(), in_ei_end = in_el.end();
        for (; in_ei != in_ei_end; ++in_ei) {
          boost::erase(g.out_edge_list((*in_ei).get_target()), StoredEdge(u));
          g.m_edges.erase((*in_ei).get_iter());
        }

        g.out_edge_list(u).clear();
        g.in_edge_list(u).clear();
      }
      // O(1) for allow_parallel_edge_tag
      // O(log(E/V)) for disallow_parallel_edge_tag
      friend inline std::pair<edge_descriptor, bool>
      add_edge(graph_type& g, vertex_descriptor u, vertex_descriptor v, 
	       const edge_plugin_type& p = edge_plugin_type())
      {
	bool inserted;
	g.m_edges.push_back(p);
	typename Config::EdgeContainer::iterator p_iter 
	  = boost::prior(g.m_edges.end());
	typename Config::OutEdgeList::iterator i;
	boost::tie(i,inserted) = boost::push(g.out_edge_list(u), 
					    StoredEdge(v, p_iter));
	if (inserted) {
	  boost::push(g.in_edge_list(v), StoredEdge(u, p_iter));
	  return std::make_pair(edge_descriptor(u, v, &p_iter->m_plugin), 
				true);
	} else {
	  g.m_edges.erase(p_iter);
	  return std::make_pair(edge_descriptor(u,v), false);
	}
      }
      // O(1)
      friend inline typename Config::degree_size_type
      degree(vertex_descriptor u, const graph_type& g)
      {
	return in_degree(u, g) + out_degree(u, g);
      }
    };

    //=========================================================================
    // Bidirectional Graph Helper Class (without edge plugins)

    template <class Config>
    struct bidirectional_graph_helper_without_plugin
      : public directed_edges_helper<Config>
    {
      typedef typename Config::graph_type graph_type;
      typedef typename Config::vertex_descriptor vertex_descriptor;
      typedef typename Config::vertex_iterator vertex_iterator;
      typedef typename Config::out_edge_iterator out_edge_iterator;
      typedef typename Config::in_edge_iterator in_edge_iterator;
      typedef typename Config::edge_descriptor edge_descriptor;
      typedef typename Config::StoredEdge StoredEdge;

      // O(1) for allow_parallel_edge_tag
      // O(log(E/V)) for disallow_parallel_edge_tag
      friend inline std::pair<edge_descriptor, bool>
      add_edge(graph_type& g, vertex_descriptor u, vertex_descriptor v)
      {
        bool inserted;
        typename Config::OutEdgeList::iterator i;
        boost::tie(i,inserted) = boost::push(g.out_edge_list(u), 
                                            StoredEdge(v));
        if (inserted) {
          boost::push(g.in_edge_list(v), StoredEdge(u));
          return std::make_pair(edge_descriptor(u, v), inserted);
        } else
          return std::make_pair(edge_descriptor(u, v), false);
      }
      // O(E/V) for allow_parallel_edge_tag
      // O(log(E/V)) for disallow_parallel_edge_tag
      friend inline void
      remove_edge(graph_type& g, vertex_descriptor u, vertex_descriptor v)
      {
        boost::erase(g.out_edge_list(u), StoredEdge(v));
        boost::erase(g.in_edge_list(v), StoredEdge(u));
      }
      // O(V), could do better...
      friend inline typename Config::edges_size_type
      num_edges(const graph_type& g) 
      {
        typename Config::edges_size_type num_e = 0;
        vertex_iterator vi, vi_end;
        for (boost::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
          num_e += out_degree(*vi, g);
        return num_e;
      }
      // O(E/V * E/V) for allow_parallel_edge_tag
      // O(E/V * log(E/V)) for disallow_parallel_edge_tag
      friend inline void 
      clear_vertex(graph_type& g, vertex_descriptor u)
      {
        out_edge_iterator out_ei, out_ei_end;
        for (boost::tie(out_ei, out_ei_end) = out_edges(u, g); 
             out_ei != out_ei_end; ++out_ei)
          boost::erase(g.in_edge_list(target(*out_ei,g)), StoredEdge(u));

        in_edge_iterator in_ei, in_ei_end;
        for (boost::tie(in_ei, in_ei_end) = in_edges(u, g); 
             in_ei != in_ei_end; ++in_ei)
          boost::erase(g.out_edge_list(source(*in_ei,g)), StoredEdge(u));

        g.out_edge_list(u).clear();
        g.in_edge_list(u).clear();
      }
      // O(1)
      friend inline typename Config::degree_size_type
      degree(vertex_descriptor u, const graph_type& g)
      {
	return in_degree(u, g) + out_degree(u, g);
      }
    };

    //=========================================================================
    // Adjacency List Helper Class

    template <class Config, class Base>
    struct adj_list_helper : public Base
    {
      typedef typename Config::graph_type AdjList;
      typedef typename Config::vertex_descriptor vertex_descriptor;
      typedef typename Config::edge_descriptor edge_descriptor;
      typedef typename Config::out_edge_iterator out_edge_iterator;
      typedef typename Config::in_edge_iterator in_edge_iterator;
      typedef typename Config::adjacency_iterator adjacency_iterator;
      typedef typename Config::vertex_iterator vertex_iterator;
      typedef typename Config::edge_iterator edge_iterator;
      typedef typename Config::directed_category directed_category;
      typedef typename Config::edge_parallel_category edge_parallel_category;
      typedef typename Config::vertices_size_type vertices_size_type;
      typedef typename Config::edges_size_type edges_size_type;
      typedef typename Config::degree_size_type degree_size_type;
      typedef typename Config::StoredEdge StoredEdge;
      typedef typename Config::edge_plugin_type edge_plugin_type;

      friend inline std::pair<adjacency_iterator,adjacency_iterator>
      adjacent_vertices(vertex_descriptor u, const AdjList& g_)
      {
        AdjList& g = const_cast<AdjList&>(g_);
        out_edge_iterator first, last;
        boost::tie(first, last) = out_edges(u, g);
        return std::make_pair(adjacency_iterator(first, &g),
                              adjacency_iterator(last, &g));
      }
      // adj() is deprecated, replaced by adjacent_vertices()
      friend inline std::pair<adjacency_iterator,adjacency_iterator>
      adj(vertex_descriptor u, const AdjList& g_)
      {
	return adjacent_vertices(u, g_);
      }
      friend inline std::pair<out_edge_iterator, out_edge_iterator>
      out_edges(vertex_descriptor u, const AdjList& g_)
      {
        AdjList& g = const_cast<AdjList&>(g_);
        return
          std::make_pair(out_edge_iterator(g.out_edge_list(u).begin(), u),
                         out_edge_iterator(g.out_edge_list(u).end(), u));
      }
      friend inline std::pair<vertex_iterator, vertex_iterator>
      vertices(const AdjList& g_)
      {
        AdjList& g = const_cast<AdjList&>(g_);
        return std::make_pair( g.vertex_set().begin(), g.vertex_set().end() );
      }
      friend inline vertices_size_type
      num_vertices(const AdjList& g) 
      {
        return g.vertex_set().size();
      }
      friend inline degree_size_type
      out_degree(vertex_descriptor u, const AdjList& g)
      {
        return g.out_edge_list(u).size();
      }
      friend inline std::pair<edge_descriptor,bool>
      edge(vertex_descriptor u, vertex_descriptor v, const AdjList& g) {
        return edge_dispatch(g, u, v, edge_parallel_category());
      }

    protected:

      // O(E/V)
      static inline std::pair<edge_descriptor,bool>      
      edge_dispatch(const AdjList& g, 
                    vertex_descriptor u, vertex_descriptor v, 
                    boost::allow_parallel_edge_tag)
      {
        bool found;
        const typename Config::OutEdgeList& el = g.out_edge_list(u);
        typename Config::OutEdgeList::const_iterator 
          i = std::find(el.begin(), el.end(), StoredEdge(v));
        found = (i != g.out_edge_list(u).end());
        if (found)
          return std::make_pair(edge_descriptor(u, v, (*i).get_plugin()),
                                true);
        else
          return std::make_pair(edge_descriptor(u, v), false);
      }
      // O(log(E/V))
      static inline std::pair<edge_descriptor,bool>      
      edge_dispatch(const AdjList& g, 
                    vertex_descriptor u, vertex_descriptor v, 
                    boost::disallow_parallel_edge_tag)
      {
        bool found;
        typename Config::OutEdgeList::const_iterator 
          i = g.out_edge_list(u).find(StoredEdge(v)),
          end = g.out_edge_list(u).end();
        found = (i != end);
        if (found)
          return std::make_pair(edge_descriptor(u, v, (*i).get_plugin()),
                                true);
        else
          return std::make_pair(edge_descriptor(u, v), false);
      }
      
    };


    template <class Config, class Base>
    inline std::pair<typename Config::in_edge_iterator, 
                     typename Config::in_edge_iterator>
    in_edges(typename Config::vertex_descriptor u, 
             const adj_list_helper<Config,Base>& g_)
    {
      typedef typename Config::graph_type Graph;
      const Graph& cg = static_cast<const Graph&>(g_);
      Graph& g = const_cast<Graph&>(cg);
      typedef typename Config::in_edge_iterator in_edge_iterator;
      return std::make_pair( in_edge_iterator(g.in_edge_list(u).begin(),u),
                             in_edge_iterator(g.in_edge_list(u).end(),u) );
    }

    template <class Config, class Base>
    inline typename Config::degree_size_type
    in_degree(typename Config::vertex_descriptor u, 
              const adj_list_helper<Config,Base>& g_)
    {
      typedef typename Config::graph_type Graph;
      const Graph& cg = static_cast<const Graph&>(g_);
      Graph& g = const_cast<Graph&>(cg);
      return g.in_edge_list(u).size();
    }

    // The get_edge_property() and get_vertex_property() functions
    // have to be defined outside the class definition because
    // of a VC++ bug triggered by the need for Tag to be templated
    // independently of the adj_list_helper class.

    template <class Config, class Base, class Tag>
    inline
    typename boost::edge_property_accessor<typename Config::graph_type,
      Tag>::type
    get_edge_property_accessor(const adj_list_helper<Config,Base>&, Tag) {
      typedef typename Config::graph_type Graph;
      typedef typename boost::edge_property_accessor<Graph, Tag>::type PA;
      return PA();
    }

    template <class Config, class Base, class Tag>
    inline
    typename boost::edge_property_accessor<typename Config::graph_type,
      Tag>::type
    get_edge_property(const adj_list_helper<Config,Base>&, Tag) {
      typedef typename Config::graph_type Graph;
      typedef typename boost::edge_property_accessor<Graph, Tag>::type PA;
      return PA();
    }

    template <class Config, class Base, class Tag>
    inline
    typename boost::vertex_property_accessor<typename Config::graph_type,
                                            Tag>::type
    get_vertex_property_accessor(adj_list_helper<Config,Base>& g, Tag) {
      typedef typename Config::graph_type Graph;
      typedef typename boost::vertex_property_accessor<Graph, Tag>::type PA;
      return PA(static_cast<Graph&>(g));
    }
    template <class Config, class Base, class Tag>
    inline
    typename boost::vertex_property_accessor<typename Config::graph_type,
                                            Tag>::const_type
    get_vertex_property_accessor(const adj_list_helper<Config,Base>& g, Tag) {
      typedef typename Config::graph_type Graph;
      typedef typename boost::vertex_property_accessor<Graph, Tag>
        ::const_type PA;
      return PA(static_cast<const Graph&>(g));
    }

    template <class Config, class Base, class Tag>
    inline
    typename boost::vertex_property_accessor<typename Config::graph_type,
                                            Tag>::type
    get_vertex_property(adj_list_helper<Config,Base>& g, Tag) {
      typedef typename Config::graph_type Graph;
      typedef typename boost::vertex_property_accessor<Graph, Tag>::type PA;
      return PA(static_cast<Graph&>(g));
    }
    template <class Config, class Base, class Tag>
    inline
    typename boost::vertex_property_accessor<typename Config::graph_type,
                                            Tag>::const_type
    get_vertex_property(const adj_list_helper<Config,Base>& g, Tag) {
      typedef typename Config::graph_type Graph;
      typedef typename boost::vertex_property_accessor<Graph, Tag>
        ::const_type PA;
      return PA(static_cast<const Graph&>(g));
    }

    //=========================================================================
    // Generalize Adjacency List Implementation

    struct adj_list_tag { };

    template <class Derived, class Config, class Base>
    class adj_list_impl
      : public adj_list_helper<Config, Base>
    {
      typedef typename Config::OutEdgeList OutEdgeList;
      typedef typename Config::InEdgeList InEdgeList;
      typedef typename Config::StoredVertexList StoredVertexList;
    public:
      typedef typename Config::stored_vertex stored_vertex;
      typedef typename Config::EdgeContainer EdgeContainer;
      typedef typename Config::vertex_descriptor vertex_descriptor;
      typedef typename Config::vertex_iterator vertex_iterator;
      typedef typename Config::edge_parallel_category edge_parallel_category;
      typedef typename Config::vertices_size_type vertices_size_type;
      typedef typename Config::edges_size_type edges_size_type;
      typedef typename Config::degree_size_type degree_size_type;
      typedef adj_list_tag graph_tag;

      inline adj_list_impl() { }

      inline adj_list_impl(vertices_size_type num_vertices) {
        for (vertices_size_type i = 0; i < num_vertices; ++i)
          add_vertex(static_cast<Derived&>(*this));
      }
      template <class EdgeIterator>
      inline adj_list_impl(vertices_size_type num_vertices,
			   EdgeIterator first, EdgeIterator last)
      {
	vertex_descriptor* v = new vertex_descriptor[num_vertices];
        for (vertices_size_type i = 0; i < num_vertices; ++i)
          v[i] = add_vertex(static_cast<Derived&>(*this));

	while (first != last) {
	  add_edge(*this, v[(*first).first], v[(*first).second]);
	  ++first;
	}
	delete [] v;
      }
      template <class EdgeIterator, class EdgePropertyIterator>
      inline adj_list_impl(vertices_size_type num_vertices,
			   EdgeIterator first, EdgeIterator last,
			   EdgePropertyIterator ep_iter)
      {
	vertex_descriptor* v = new vertex_descriptor[num_vertices];
        for (vertices_size_type i = 0; i < num_vertices; ++i)
          v[i] = add_vertex(static_cast<Derived&>(*this));

	while (first != last) {
	  add_edge(*this, v[(*first).first], v[(*first).second], *ep_iter);
	  ++first;
	  ++ep_iter;
	}
	delete [] v;
      }

      // O(1)
      friend inline vertex_descriptor add_vertex(Derived& g)
      {
        stored_vertex* v = new stored_vertex;
        typename StoredVertexList::iterator pos;
        bool inserted;
        boost::tie(pos,inserted) = boost::push(g.m_vertices, v);
        v->m_position = pos;
        return v;
      }
      // O(1)
      friend inline void remove_vertex(Derived& g, vertex_descriptor u) {
	stored_vertex* su = (stored_vertex*)u;
        g.m_vertices.erase(su->m_position);
        delete su;
      }
      // O(V)
      friend inline vertex_descriptor
      vertex(vertices_size_type n, const Derived& g)
      {
        vertex_iterator i = vertices(g).first;
        while (n--) ++i; // std::advance(i, n); (not VC++ portable)
        return *i;
      }
      //    protected:
      inline OutEdgeList& out_edge_list(vertex_descriptor v) {
	stored_vertex* sv = (stored_vertex*)v;
        return sv->m_out_edges;
      }
      inline const OutEdgeList& out_edge_list(vertex_descriptor v) const {
	stored_vertex* sv = (stored_vertex*)v;
        return sv->m_out_edges;
      }
      inline InEdgeList& in_edge_list(vertex_descriptor v) {
	stored_vertex* sv = (stored_vertex*)v;
        return sv->m_in_edges;
      }
      inline const InEdgeList& in_edge_list(vertex_descriptor v) const {
	stored_vertex* sv = (stored_vertex*)v;
        return sv->m_in_edges;
      }
      inline StoredVertexList& vertex_set() { return m_vertices; }
      inline const StoredVertexList& vertex_set() const { return m_vertices; }

      typename Config::EdgeContainer m_edges;
      StoredVertexList m_vertices;
    };

    //=========================================================================
    // Vector-Backbone Adjacency List Implementation

    struct vec_adj_list_tag { };
    
    template <class Graph, class Config, class Base>
    class vec_adj_list_impl
      : public adj_list_helper<Config, Base>
    {
      typedef typename Config::OutEdgeList OutEdgeList;
      typedef typename Config::InEdgeList InEdgeList;
      typedef typename Config::StoredVertexList StoredVertexList; 
    public:
      typedef typename Config::vertex_descriptor vertex_descriptor;
      typedef typename Config::edge_descriptor edge_descriptor;
      typedef typename Config::out_edge_iterator out_edge_iterator;
      typedef typename Config::directed_category directed_category;
      typedef typename Config::vertices_size_type vertices_size_type;
      typedef typename Config::edges_size_type edges_size_type;
      typedef typename Config::degree_size_type degree_size_type;
      typedef typename Config::StoredEdge StoredEdge;
      typedef typename Config::stored_vertex stored_vertex;
      typedef typename Config::EdgeContainer EdgeContainer;
      typedef vec_adj_list_tag graph_tag;

      inline vec_adj_list_impl() { }

      inline vec_adj_list_impl(vertices_size_type _num_vertices)
        : m_vertices(_num_vertices) { }

      template <class EdgeIterator>
      inline vec_adj_list_impl(vertices_size_type num_vertices,
			       EdgeIterator first, EdgeIterator last)
        : m_vertices(num_vertices)
      {
	while (first != last) {
	  add_edge(static_cast<Graph&>(*this), 
		   (*first).first, (*first).second);
	  ++first;
	}
      }
      template <class EdgeIterator, class EdgePropertyIterator>
      inline vec_adj_list_impl(vertices_size_type num_vertices,
			       EdgeIterator first, EdgeIterator last,
			       EdgePropertyIterator ep_iter)
        : m_vertices(num_vertices)
      {
	while (first != last) {
	  add_edge(static_cast<Graph&>(*this), 
		   (*first).first, (*first).second, *ep_iter);
	  ++first;
	  ++ep_iter;
	}
      }

      // O(1)
      friend inline vertex_descriptor add_vertex(Graph& g) {
        g.m_vertices.resize(g.m_vertices.size() + 1);
        return g.m_vertices.size() - 1;
      }
      // O(V + E)
      friend inline void remove_vertex(Graph& g, vertex_descriptor v) {
        remove_vertex_dispatch(g, v, directed_category());
      }
      // O(1)
      friend inline vertex_descriptor 
      vertex(vertices_size_type n, const Graph&)
      {
        return n;
      }
      //    protected:
      inline boost::integer_range<vertex_descriptor> vertex_set() const {
        return boost::integer_range<vertex_descriptor>(0, m_vertices.size());
      }
      inline OutEdgeList& out_edge_list(vertex_descriptor v) {
        return m_vertices[v].m_out_edges;
      }
      inline const OutEdgeList& out_edge_list(vertex_descriptor v) const {
        return m_vertices[v].m_out_edges;
      }
      inline InEdgeList& in_edge_list(vertex_descriptor v) {
        return m_vertices[v].m_in_edges;
      }
      inline const InEdgeList& in_edge_list(vertex_descriptor v) const {
        return m_vertices[v].m_in_edges;
      }
      typename Config::EdgeContainer m_edges;
      StoredVertexList m_vertices;
    };

    template <class Graph, class vertex_descriptor>
    inline void 
    remove_vertex_dispatch(Graph& g, vertex_descriptor u, 
                           boost::directed_tag)
    {
      typedef typename Graph::edge_parallel_category edge_parallel_category;
      g.m_vertices.erase(g.m_vertices.begin() + u);
      vertex_descriptor V = num_vertices(g);
      for (vertex_descriptor v = 0; v < V; ++v)
        reindex_edge_list(g.out_edge_list(v), u, edge_parallel_category());
    }
    template <class Graph, class vertex_descriptor>
    inline void 
    remove_vertex_dispatch(Graph& g, vertex_descriptor u, 
                           boost::undirected_tag)
    {
      typedef typename Graph::edge_parallel_category edge_parallel_category;
      g.m_vertices.erase(g.m_vertices.begin() + u);
      vertex_descriptor V = num_vertices(g);
      for (vertex_descriptor v = 0; v < V; ++v)
        reindex_edge_list(g.out_edge_list(v), u, 
                          edge_parallel_category());

      typename Graph::EdgeContainer::iterator 
        ei = g.m_edges.begin(),
        ei_end = g.m_edges.end();
      for (; ei != ei_end; ++ei) {
        if (ei->m_source > u)
          --ei->m_source;
        if (ei->m_target > u)
          --ei->m_target;
      }
    }
    template <class Graph, class vertex_descriptor>
    inline void 
    remove_vertex_dispatch(Graph& g, vertex_descriptor u, 
                           boost::bidirectional_tag)
    {
      typedef typename Graph::edge_parallel_category edge_parallel_category;
      g.m_vertices.erase(g.m_vertices.begin() + u);
      vertex_descriptor V = num_vertices(g);
      vertex_descriptor v;
      for (v = 0; v < V; ++v)
        reindex_edge_list(g.out_edge_list(v), u, 
                          edge_parallel_category());
      for (v = 0; v < V; ++v)
        reindex_edge_list(g.in_edge_list(v), u, 
                          edge_parallel_category());
    }

    template <class EdgeList, class vertex_descriptor>
    inline void
    reindex_edge_list(EdgeList& el, vertex_descriptor u, 
                      boost::allow_parallel_edge_tag)
    {
      typename EdgeList::iterator ei = el.begin(), e_end = el.end();
      for (; ei != e_end; ++ei)
        if ((*ei).get_target() > u)
          --(*ei).get_target();
    }
    template <class EdgeList, class vertex_descriptor>
    inline void
    reindex_edge_list(EdgeList& el, vertex_descriptor u, 
                      boost::disallow_parallel_edge_tag)
    {
      typename EdgeList::iterator ei = el.begin(), e_end = el.end();
      while (ei != e_end) {
        typename EdgeList::value_type ce = *ei;
        ++ei;
        if (ce.get_target() > u) {
          el.erase(ce);
          --ce.get_target();
          el.insert(ce);
        }
      }
    }

#ifdef __GNUC__
namespace boost {
#endif

  template <class EdgeListS = vecS,
            class VertexListS = vecS,
            class DirectedS = directedS>
  struct adjacency_list_traits
  {
    enum { is_rand_access = detail::is_random_access<VertexListS>::value };

    typedef typename boost::ct_if<DirectedS::is_bidir,
      bidirectional_tag,
      typename boost::ct_if<DirectedS::is_directed,
        directed_tag, undirected_tag
      >::type
    >::type directed_category;

    typedef typename parallel_edge_traits<EdgeListS>::type
      edge_parallel_category;

    typedef void* vertex_ptr;
    typedef typename boost::ct_if<is_rand_access,
      std::size_t, vertex_ptr>::type vertex_descriptor;

    typedef detail::bidir_edge<directed_category, vertex_descriptor> edge_descriptor;
  };

  namespace detail {

    //=========================================================================
    // Adjacency List Generator

    template <class Graph, class VertexListS, class EdgeListS,
              class DirectedS, class VertexPlugin, class EdgePlugin, 
              class GraphPlugin>
    struct adjacency_list_generator
    {
      enum { is_rand_access = detail::is_random_access<VertexListS>::value,
             has_edge_plugin = has_plugin<EdgePlugin>::value,
	     Directed = DirectedS::is_directed,
	     Bidirectional = DirectedS::is_bidir };

      struct config
      {
        typedef Graph graph_type;
        typedef EdgePlugin edge_plugin_type;
        typedef VertexPlugin vertex_plugin_type;
	typedef GraphPlugin graph_plugin_type;
        typedef std::size_t vertices_size_type;

	typedef adjacency_list_traits<EdgeListS, VertexListS, DirectedS> 
           Traits;

	typedef typename Traits::directed_category directed_category;
	typedef typename Traits::edge_parallel_category edge_parallel_category;
	typedef typename Traits::vertex_descriptor vertex_descriptor;
	typedef typename Traits::edge_descriptor edge_descriptor;

	typedef void* vertex_ptr; 

        // VertexList and vertex_iterator
        typedef typename container_gen<VertexListS, 
          vertex_ptr>::type SeqVertexList;
        typedef boost::integer_range<std::size_t> RandVertexList;
        typedef typename boost::ct_if<is_rand_access,
          RandVertexList, SeqVertexList>::type VertexList;

        typedef typename VertexList::iterator vertex_iterator;

        // EdgeContainer and StoredEdge
	typedef std::list< bidirectional_edge<EdgePlugin> > BidirEdgeList;
	typedef std::list< undirected_edge<vertex_descriptor,EdgePlugin> >
	  UndirEdgeList;
	typedef std::list< undirected_edge_no_plugin<vertex_descriptor> >
	  UndirEdgeNoPluginList;

	// need to reorganize this to avoid instantiating stuff
	// that doesn't get used -JGS

        typedef typename boost::ct_if< Directed,
          typename boost::ct_if< Bidirectional && has_edge_plugin,
            BidirEdgeList,
            no_plugin
          >::type,
          typename boost::ct_if< has_edge_plugin,
            UndirEdgeList,
            UndirEdgeNoPluginList
          >::type
        >::type EdgeContainer;

        typedef typename boost::ct_if< Directed,
          typename boost::ct_if< Bidirectional && has_edge_plugin,
            typename BidirEdgeList::size_type,
	    std::size_t
          >::type,
          typename boost::ct_if< has_edge_plugin,
            typename UndirEdgeList::size_type,
	    typename UndirEdgeNoPluginList::size_type
          >::type
        >::type edges_size_type;

        typedef typename boost::ct_if< Directed,
          typename boost::ct_if< Bidirectional && has_edge_plugin,
            typename BidirEdgeList::iterator,
	    typename BidirEdgeList::iterator // bogus, not used
          >::type,
          typename boost::ct_if< has_edge_plugin,
            typename UndirEdgeList::iterator,
            typename UndirEdgeNoPluginList::iterator
          >::type
        >::type EdgeIter;

        typedef typename boost::ct_if< Directed && !Bidirectional,
          typename boost::ct_if< has_edge_plugin,
            stored_edge_plugin<vertex_descriptor, EdgePlugin>,  
            stored_edge<vertex_descriptor>
          >::type,
          stored_edge_iter<vertex_descriptor, EdgeIter, EdgePlugin>
        >::type StoredEdge;

        // Adjacency Types

        typedef typename container_gen<EdgeListS, StoredEdge>::type 
          OutEdgeList;
	typedef typename OutEdgeList::size_type degree_size_type;
        typedef typename OutEdgeList::iterator OutEdgeIter;
#if !defined BOOST_NO_STD_ITERATOR_TRAITS
        typedef std::iterator_traits<OutEdgeIter> OutEdgeIterTraits;
#else
        // was going to use boost::iterator here, but that also caused VC++
        // problems! !*&^(*%(*!&^(*@&
        struct OutEdgeIterTraits {
          typedef std::forward_iterator_tag iterator_category;
          typedef StoredEdge value_type;
          typedef value_type* pointer;
          typedef value_type& reference;
          typedef std::ptrdiff_t difference_type;
        };
#endif

#if defined BOOST_NO_ITERATOR_ADAPTORS || defined BOOST_NO_STD_ITERATOR_TRAITS
	typedef detail::bidir_incidence_iterator<vertex_descriptor,
         edge_descriptor, OutEdgeIter, detail::out_edge_tag> out_edge_iterator;
#else
        typedef iterator_adaptor<OutEdgeIter, 
          out_edge_iter_policies<vertex_descriptor>,
          edge_iter_traits<edge_descriptor, OutEdgeIterTraits>
        > out_edge_iterator;
#endif

#if !defined BOOST_NO_ITERATOR_ADAPTORS
        typedef typename adjacency_iterator<graph_type, vertex_descriptor,
          out_edge_iterator, out_edge_iterator>::type adjacency_iterator;
#else
	typedef bidir_adjacency_iterator<vertex_descriptor,out_edge_iterator,
         	graph_type> adjacency_iterator;
#endif

        typedef OutEdgeList InEdgeList;
        typedef OutEdgeIter InEdgeIter;
        typedef OutEdgeIterTraits InEdgeIterTraits;

#if !defined BOOST_NO_STD_ITERATOR_TRAITS
        typedef typename boost::iterator_adaptor<InEdgeIter, 
          in_edge_iter_policies<vertex_descriptor>,
          edge_iter_traits<edge_descriptor, InEdgeIterTraits>
        > in_edge_iterator;
#else
	typedef detail::bidir_incidence_iterator<vertex_descriptor,
           edge_descriptor, InEdgeIter, detail::in_edge_tag> in_edge_iterator;
#endif

        // Edge Iterator
#if !defined BOOST_NO_STD_ITERATOR_TRAITS
        typedef std::iterator_traits<EdgeIter> EdgeIterTraits;
#else
	typedef OutEdgeIterTraits EdgeIterTraits;
#endif

#if !defined BOOST_NO_STD_ITERATOR_TRAITS
	typedef typename boost::iterator_adaptor<EdgeIter,
              undirected_edge_iter_policies,
              edge_iter_traits<edge_descriptor, EdgeIterTraits> > 
          UndirectedEdgeIter;
#else
	typedef undirected_edge_iter<EdgeIter,edge_descriptor> UndirectedEdgeIter;
#endif
	typedef adj_list_edge_iterator<vertex_iterator, out_edge_iterator, 
           graph_type> DirectedEdgeIter;
        typedef typename boost::ct_if< Directed,
          DirectedEdgeIter, UndirectedEdgeIter >::type edge_iterator;

        // stored_vertex and StoredVertexList
        typedef typename container_gen<VertexListS, vertex_ptr>::type
          SeqStoredVertexList;
        struct seq_stored_vertex {
          OutEdgeList m_out_edges;
          VertexPlugin m_plugin;
          typename SeqStoredVertexList::iterator m_position;
        };
        struct bidir_seq_stored_vertex {
          OutEdgeList m_out_edges;
          InEdgeList m_in_edges;
          VertexPlugin m_plugin;
          typename SeqStoredVertexList::iterator m_position;
        };
        struct rand_stored_vertex {
          OutEdgeList m_out_edges;
          VertexPlugin m_plugin;
        };
        struct bidir_rand_stored_vertex {
          OutEdgeList m_out_edges;
          InEdgeList m_in_edges;
          VertexPlugin m_plugin;
        };
        typedef typename boost::ct_if< is_rand_access,
          typename boost::ct_if< Bidirectional,
            bidir_rand_stored_vertex, rand_stored_vertex>::type,
          typename boost::ct_if< Bidirectional,
            bidir_seq_stored_vertex, seq_stored_vertex>::type
        >::type StoredVertex;
        struct stored_vertex : public StoredVertex { };

        typedef typename container_gen<VertexListS, stored_vertex>::type
          RandStoredVertexList;
        typedef typename boost::ct_if< is_rand_access,
          RandStoredVertexList, SeqStoredVertexList>::type StoredVertexList;
      };

      typedef typename boost::ct_if< Bidirectional,
        typename boost::ct_if< has_edge_plugin,
          bidirectional_graph_helper_with_plugin<config>,
          bidirectional_graph_helper_without_plugin<config>
        >::type,
        typename boost::ct_if< Directed,
          directed_graph_helper<config>,
          undirected_graph_helper<config>
        >::type
      >::type DirectedHelper;

      typedef typename boost::ct_if< is_rand_access,
        vec_adj_list_impl<Graph, config, DirectedHelper>,
        adj_list_impl<Graph, config, DirectedHelper>
      >::type type;

    };


    //=========================================================================
    // Vertex Property Accessors

    template <class Graph, class Plugin, class Tag>
    struct adj_list_vertex_property_accessor
      : public boost::detail::put_get_at_helper<
          typename plugin_value<Plugin,Tag>::type,
          adj_list_vertex_property_accessor<Graph, Plugin, Tag>
        >
    {
      typedef typename Graph::stored_vertex StoredVertex;
      typedef typename plugin_value<Plugin, Tag>::type value_type;
      typedef typename Graph::vertex_descriptor key_type;
      typedef boost::lvalue_op_bracket_tag category;
      inline adj_list_vertex_property_accessor(Graph&) { }
      inline value_type& operator[](key_type v) {
	StoredVertex* sv = (StoredVertex*)v;
        return get_plugin_value(sv->m_plugin, value_type(), Tag());
      }
      inline const value_type& operator[](key_type v) const {
	StoredVertex* sv = (StoredVertex*)v;
        return get_plugin_value(sv->m_plugin, value_type(), Tag());
      }
    };

    template <class Graph, class GraphRef, class Plugin, class Tag>
    struct vec_adj_list_vertex_property_accessor
      : public boost::detail::put_get_at_helper<
          typename plugin_value<Plugin,Tag>::type,
          vec_adj_list_vertex_property_accessor<Graph,GraphRef,Plugin,Tag>
        >
    {
      typedef typename plugin_value<Plugin,Tag>::type value_type;
      typedef typename boost::graph_traits<Graph>::vertex_descriptor key_type;
      typedef boost::lvalue_op_bracket_tag category;
      vec_adj_list_vertex_property_accessor(GraphRef g) : m_g(g) { }
      inline value_type& operator[](key_type v) {
        return get_plugin_value(m_g.m_vertices[v].m_plugin, 
                                value_type(), Tag());
      }
      inline const value_type& operator[](key_type v) const {
        return get_plugin_value(m_g.m_vertices[v].m_plugin, 
                                value_type(), Tag());
      }
      GraphRef m_g;
    };

    template <class Plugin, class Vertex>
    struct vec_adj_list_vertex_id_accessor
      : public boost::detail::put_get_at_helper<
          Vertex, vec_adj_list_vertex_id_accessor<Plugin, Vertex>
        >
    {
      typedef Vertex value_type;
      typedef Vertex key_type;
      typedef boost::lvalue_op_bracket_tag category;
      template <class Graph>
      inline vec_adj_list_vertex_id_accessor(const Graph&) { }
      inline value_type operator[](key_type v) const { return v; }
    };

    struct vec_adj_list_any_vertex_pa {
      template <class Tag, class Graph, class Plugin>
      struct bind {
        typedef detail::vec_adj_list_vertex_property_accessor
          <Graph, Graph&, Plugin, Tag> type;
        typedef detail::vec_adj_list_vertex_property_accessor
          <Graph, const Graph&, Plugin, Tag> const_type;
      };
    };
    struct vec_adj_list_id_vertex_pa {
      template <class Tag, class Graph, class Plugin>
      struct bind {
        typedef typename Graph::vertex_descriptor Vertex;
        typedef vec_adj_list_vertex_id_accessor<Plugin, Vertex> type;
        typedef vec_adj_list_vertex_id_accessor<Plugin, Vertex> const_type;
      };
    };
    template <class Tag>
    struct vec_adj_list_choose_vertex_pa_helper {
      typedef vec_adj_list_any_vertex_pa type;
    };
    template <>
    struct vec_adj_list_choose_vertex_pa_helper<id_tag> {
      typedef vec_adj_list_id_vertex_pa type;
    };

    template <class Tag, class Graph, class Plugin>
    struct vec_adj_list_choose_vertex_pa {
      typedef typename vec_adj_list_choose_vertex_pa_helper<Tag>::type Helper;
      typedef typename Helper::BOOST_TEMPLATE bind<Tag,Graph,Plugin> Bind;
      typedef typename Bind::type type;
      typedef typename Bind::const_type const_type;
    };
    

    //=========================================================================
    // Edge Property Accessor

    template <class Directed, class Plugin, class Vertex, class Tag>
    struct adj_list_edge_property_accessor
      : public boost::detail::put_get_at_helper<
          typename plugin_value<Plugin,Tag>::type,
          adj_list_edge_property_accessor<Directed,Plugin,Vertex,Tag>
        >
    {
      typedef typename plugin_value<Plugin,Tag>::type value_type;
      typedef detail::bidir_edge<Directed, Vertex> key_type;
      typedef boost::lvalue_op_bracket_tag category;
      inline value_type& operator[](key_type e) {
	Plugin* p = (Plugin*)e.get_plugin();
        return get_plugin_value(*p, value_type(), Tag());
      }
      inline const value_type& operator[](key_type e) const {
	const Plugin* p = (Plugin*)e.get_plugin();
        return get_plugin_value(*p, value_type(), Tag());
      }
    };


  } // namespace detail

  // Edge Property Accessors

  struct adj_list_edge_property_selector {
    template <class Graph, class Plugin, class Tag>
    struct bind {
      typedef detail::adj_list_edge_property_accessor
         <typename Graph::directed_category, Plugin, 
          typename Graph::vertex_descriptor,Tag> type;
      typedef type const_type;
    };
  };
  template <>  
  struct edge_property_selector<adj_list_tag> {
    typedef adj_list_edge_property_selector type;
  };
  template <>  
  struct edge_property_selector<vec_adj_list_tag> {
    typedef adj_list_edge_property_selector type;
  };

  // Vertex Property Accessors

  struct adj_list_vertex_property_selector {
    template <class Graph, class Plugin, class Tag>
    struct bind {
      typedef detail::adj_list_vertex_property_accessor<Graph,Plugin,Tag> type;
      typedef type const_type;
    };
  };
  template <>  
  struct vertex_property_selector<adj_list_tag> {
    typedef adj_list_vertex_property_selector type;
  };

  struct vec_adj_list_vertex_property_selector {
    template <class Graph, class Plugin, class Tag>
    struct bind {
      typedef detail::vec_adj_list_choose_vertex_pa<Tag,Graph,Plugin> Choice;
      typedef typename Choice::type type;
      typedef typename Choice::const_type const_type;
    };
  };
  template <>  
  struct vertex_property_selector<vec_adj_list_tag> {
    typedef vec_adj_list_vertex_property_selector type;
  };


} // namespace boost

#endif // BOOST_GRAPH_DETAIL_DETAIL_ADJACENCY_LIST_CCT
