// -*- c++ -*-
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

#ifndef BOOST_GRAPH_DETAIL_ADJACENCY_LIST_HPP
#define BOOST_GRAPH_DETAIL_ADJACENCY_LIST_HPP

#include <map> // for vertex_map in copy_impl
#include <boost/config.hpp>
#include <boost/operators.hpp>
#include <boost/property_map.hpp>
#include <boost/pending/integer_range.hpp>
#include <boost/graph/graph_traits.hpp>
#include <memory>
#include <algorithm>

#include <boost/iterator_adaptors.hpp>

#include <boost/pending/ct_if.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/detail/adj_list_edge_iterator.hpp>
#include <boost/graph/properties.hpp>
#include <boost/pending/property.hpp>
#include <boost/graph/adjacency_iterator.hpp>

// Symbol truncation problems with MSVC, trying to shorten names.
#define out_edge_iter_policies oeip_
#define in_edge_iter_policies ieip_
#define stored_edge se_
#define stored_edge_property sep_
#define stored_edge_iter sei_

/*
  Outline for this file:

  out_edge_iterator and in_edge_iterator implementation
  edge_iterator for undirected graph
  stored edge types (these object live in the out-edge/in-edge lists)
  directed edges helper class
  directed graph helper class
  undirected graph helper class
  bidirectional graph helper class
  bidirectional graph helper class (without edge properties)
  bidirectional graph helper class (with edge properties)
  adjacency_list helper class
  adj_list_impl class
  vec_adj_list_impl class
  adj_list_gen class
  vertex property map
  edge property map


  Note: it would be nice to merge some of the undirected and
  bidirectional code... it is aweful similar.
 */



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

    template <class Vertex>
    struct target_is {
      target_is(const Vertex& v) : m_target(v) { }
      template <class StoredEdge>
      bool operator()(const StoredEdge& e) const {
        return e.get_target() == m_target;
      }
      Vertex m_target;
    };

    // O(E/V)
    template <class EdgeList, class vertex_descriptor>
    void erase_from_incidence_list(EdgeList& el, vertex_descriptor v,
                                   allow_parallel_edge_tag)
    {
      boost::erase_if(el, detail::target_is<vertex_descriptor>(v));
    }
    // O(log(E/V))
    template <class EdgeList, class vertex_descriptor>
    void erase_from_incidence_list(EdgeList& el, vertex_descriptor v,
                                   disallow_parallel_edge_tag)
    {
      typedef typename EdgeList::value_type StoredEdge;
      el.erase(StoredEdge(v));
    }

    //=========================================================================
    // Out-Edge and In-Edge Iterator Implementation

    template <class VertexDescriptor>
    struct out_edge_iter_policies : public boost::default_iterator_policies
    {
      inline out_edge_iter_policies() { }
      inline out_edge_iter_policies(const VertexDescriptor& src)
        : m_src(src) { }

      template <class EdgeDescriptor, class OutEdgeIter>
      inline EdgeDescriptor
      dereference(boost::type<EdgeDescriptor>, const OutEdgeIter& i) const {
        return EdgeDescriptor(m_src, (*i).get_target(), &(*i).get_property());
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
        return EdgeDescriptor((*i).get_target(), m_src, &i->get_property());
      }
      VertexDescriptor m_src;
    };

    //=========================================================================
    // Undirected Edge Iterator Implementation

    struct undirected_edge_iter_policies
      : public boost::default_iterator_policies
    {
      template <class EdgeDescriptor, class InEdgeIter>
      inline EdgeDescriptor
      dereference(boost::type<EdgeDescriptor>, const InEdgeIter& i) const {
        return EdgeDescriptor((*i).m_source, (*i).m_target,
                              &i->get_property());
      }
    };

    //=========================================================================
    // Edge Storage Types (stored in the out-edge/in-edge lists)

    template <class Vertex>
    class stored_edge
      : public boost::equality_comparable1< stored_edge<Vertex>,
        boost::less_than_comparable1< stored_edge<Vertex> > >
    {
    public:
      typedef no_property property_type;
      inline stored_edge() { }
      inline stored_edge(Vertex target, const no_property& = no_property())
        : m_target(target) { }
      // Need to write this explicitly so stored_edge_property can
      // invoke Base::operator= (at least, for SGI MIPSPro compiler)
      inline stored_edge& operator=(const stored_edge& x) {
        m_target = x.m_target;
        return *this;
      }
      inline Vertex& get_target() const { return m_target; }
      inline const no_property& get_property() const { return s_prop; }
      inline bool operator==(const stored_edge& x) const
        { return m_target == x.get_target(); }
      inline bool operator<(const stored_edge& x) const
        { return m_target < x.get_target(); }
    protected:
      static no_property s_prop;
      // Sometimes target not used as key in the set, and in that case
      // it is ok to change the target.
      mutable Vertex m_target;
    };
    template <class Vertex>
    no_property stored_edge<Vertex>::s_prop;

    template <class Vertex, class Property>
    class stored_edge_property : public stored_edge<Vertex> {
      typedef stored_edge_property self;
      typedef stored_edge<Vertex> Base;
    public:
      typedef Property property_type;
      inline stored_edge_property() { }
      inline stored_edge_property(Vertex target,
                                  const Property& p = Property())
        : stored_edge<Vertex>(target), m_property(new Property(p)) { }
      stored_edge_property(const self& x) 
        : Base(x), m_property(const_cast<self&>(x).m_property) { }
      self& operator=(const self& x) {
        Base::operator=(x);
        m_property = const_cast<self&>(x).m_property; 
        return *this;
      }
      inline Property& get_property() { return *m_property; }
      inline const Property& get_property() const { return *m_property; }
    protected:
      // Holding the property by-value causes edge-descriptor
      // invalidation for add_edge() with EdgeList=vecS. Instead we
      // hold a pointer to the property. std::auto_ptr is not
      // a perfect fit for the job, but it is darn close.
      std::auto_ptr<Property> m_property;
    };


    template <class Vertex, class Iter, class Property>
    class stored_edge_iter
      : public stored_edge<Vertex>
    {
    public:
      typedef Property property_type;
      inline stored_edge_iter() { }
      inline stored_edge_iter(Vertex v, Iter i = Iter())
        : stored_edge<Vertex>(v), m_iter(i) { }
      inline Property& get_property() { return m_iter->get_property(); }
      inline const Property& get_property() const { 
        return m_iter->get_property();
      }
      inline Iter get_iter() const { return m_iter; }
    protected:
      Iter m_iter;
    };
  } // namespace detail
    
  template <class Tag, class Vertex, class Property>
  const typename property_value<Property,Tag>::type&
  get(Tag property_tag,
      const detail::stored_edge_property<Vertex, Property>& e)
  {
    typedef typename property_value<Property,Tag>::type value_type;
    return get_property_value(e.get_property(), value_type(), property_tag);
  }

  template <class Tag, class Vertex, class Iter, class Property>
  const typename property_value<Property,Tag>::type&
  get(Tag property_tag,
      const detail::stored_edge_iter<Vertex, Iter, Property>& e)
  {
    typedef typename property_value<Property,Tag>::type value_type;
    return get_property_value(e.get_property(), value_type(), property_tag);
  }
    
    //=========================================================================
    // Directed Edges Helper Class

    namespace detail {

      template <class incidence_iterator, class EdgeList, class Predicate>
      inline void
      remove_directed_edge_if_dispatch(incidence_iterator first,
                                       incidence_iterator last, 
                                       EdgeList& el, Predicate pred,
                                       boost::allow_parallel_edge_tag)
      {
        // remove_if
        while (first != last && !pred(*first))
          ++first;
        incidence_iterator i = first;
        if (first != last)
          for (; i != last; ++i)
            if (!pred(*i)) {
              *first.iter() = *i.iter();
              ++first;
            }
        el.erase(first.iter(), el.end());
      }
      template <class incidence_iterator, class EdgeList, class Predicate>
      inline void
      remove_directed_edge_if_dispatch(incidence_iterator first,
                                       incidence_iterator last, 
                                       EdgeList& el, 
                                       Predicate pred,
                                       boost::disallow_parallel_edge_tag)
      {
        for (incidence_iterator next = first;
             first != last; first = next) {
          ++next;
          if (pred(*first))
            el.erase( first.iter() );
        }
      }

      // O(E/V)
      template <class edge_descriptor, class EdgeList, class StoredProperty>
      inline void
      remove_directed_edge_dispatch(edge_descriptor, EdgeList& el,
                                    StoredProperty& p)
      {
        typename EdgeList::iterator i = el.begin();
        for (; i != el.end(); ++i)
          if (&(*i).get_property() == &p) {
            el.erase(i);
            return;
          }
      }

      // O(E/V)
      template <class edge_descriptor, class EdgeList, class StoredProperty>
      inline void
      remove_directed_edge_dispatch(edge_descriptor e, EdgeList& el,
                                    no_property&)
      {
        typename EdgeList::iterator i = el.begin();
        for (; i != el.end(); ++i)
          if ((*i).get_target() == e.m_target) {
            el.erase(i);
            return;
          }
      }

    } // namespace detail

    template <class Config>
    struct directed_edges_helper {

      // Placement of these overloaded remove_edge() functions
      // inside the class avoids a VC++ bug.
      
      // O(E/V)
      inline void
      remove_edge(typename Config::edge_descriptor e)
      {
        typedef typename Config::graph_type graph_type;
        graph_type& g = static_cast<graph_type&>(*this);
        typename Config::OutEdgeList& el = g.out_edge_list(source(e, g));
        typedef typename Config::OutEdgeList::value_type::property_type PType;
        detail::remove_directed_edge_dispatch(e, el,
                                              *(PType*)e.get_property());
      }

      // O(1)
      inline void
      remove_edge(typename Config::out_edge_iterator iter)
      {
        typedef typename Config::graph_type graph_type;
        graph_type& g = static_cast<graph_type&>(*this);
        typename Config::edge_descriptor e = *iter;
        typename Config::OutEdgeList& el = g.out_edge_list(source(e, g));
        el.erase(iter.iter());
      }

    };

    // O(1)
    template <class Config>
    inline std::pair<typename Config::edge_iterator, 
                     typename Config::edge_iterator>
    edges(const directed_edges_helper<Config>& g_)
    {
      typedef typename Config::graph_type graph_type;
      typedef typename Config::edge_iterator edge_iterator;
      const graph_type& cg = static_cast<const graph_type&>(g_);
      graph_type& g = const_cast<graph_type&>(cg);
      return std::make_pair( edge_iterator(g.vertex_set().begin(), 
                                           g.vertex_set().begin(), 
                                           g.vertex_set().end(), g),
                             edge_iterator(g.vertex_set().begin(),
                                           g.vertex_set().end(),
                                           g.vertex_set().end(), g) );
    }

    //=========================================================================
    // Directed Graph Helper Class

    template <class Config>
    struct directed_graph_helper
      : public directed_edges_helper<Config> { 
      typedef vertex_and_edge_list_graph_tag traversal_category;
    };

    // O(E/V)
    template <class Config>
    inline void
    remove_edge(typename Config::vertex_descriptor u,
                typename Config::vertex_descriptor v,
                directed_graph_helper<Config>& g_)
    {
      typedef typename Config::graph_type graph_type;
      typedef typename Config::edge_parallel_category Cat;
      graph_type& g = static_cast<graph_type&>(g_);
      detail::erase_from_incidence_list(g.out_edge_list(u), v, Cat());
    }

    template <class Config, class Predicate>
    inline void
    remove_out_edge_if(typename Config::vertex_descriptor u, Predicate pred,
                       directed_graph_helper<Config>& g_)
    {
      typedef typename Config::graph_type graph_type;
      graph_type& g = static_cast<graph_type&>(g_);
      typename Config::out_edge_iterator first, last;
      tie(first, last) = out_edges(u, g);
      typedef typename Config::edge_parallel_category edge_parallel_category;
      detail::remove_directed_edge_if_dispatch
        (first, last, g.out_edge_list(u), pred, edge_parallel_category());
    }

    template <class Config, class Predicate>
    inline void
    remove_edge_if(Predicate pred, directed_graph_helper<Config>& g_)
    {
      typedef typename Config::graph_type graph_type;
      graph_type& g = static_cast<graph_type&>(g_);

      typename Config::vertex_iterator vi, vi_end;
      for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
        remove_out_edge_if(*vi, pred, g);
    }    

    template <class EdgeOrIter, class Config>
    inline void
    remove_edge(EdgeOrIter e_or_iter, directed_graph_helper<Config>& g_)
    {
      g_.remove_edge(e_or_iter);
    }

    // O(V + E) for allow_parallel_edges
    // O(V * log(E/V)) for disallow_parallel_edges
    template <class Config>
    inline void 
    clear_vertex(typename Config::vertex_descriptor u,
                 directed_graph_helper<Config>& g_)
    {
      typedef typename Config::graph_type graph_type;
      typedef typename Config::edge_parallel_category Cat;
      graph_type& g = static_cast<graph_type&>(g_);
      typename Config::vertex_iterator vi, viend;
      for (boost::tie(vi, viend) = vertices(g); vi != viend; ++vi)
        detail::erase_from_incidence_list(g.out_edge_list(*vi), u, Cat());
      g.out_edge_list(u).clear();
      // clear() should be a req of Sequence and AssociativeContainer,
      // or maybe just Container
    }

    // O(V), could do better...
    template <class Config>
    inline typename Config::edges_size_type
    num_edges(const directed_graph_helper<Config>& g_)
    {
      typedef typename Config::graph_type graph_type;
      const graph_type& g = static_cast<const graph_type&>(g_);
      typename Config::edges_size_type num_e = 0;
      typename Config::vertex_iterator vi, vi_end;
      for (boost::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
        num_e += out_degree(*vi, g);
      return num_e;
    }
    // O(1) for allow_parallel_edge_tag
    // O(log(E/V)) for disallow_parallel_edge_tag
    template <class Config>
    inline std::pair<typename Config::edge_descriptor, bool>
    add_edge(typename Config::vertex_descriptor u, 
             typename Config::vertex_descriptor v,
             const typename Config::edge_property_type& p, 
             directed_graph_helper<Config>& g_)
    {
      typedef typename Config::edge_descriptor edge_descriptor;
      typedef typename Config::graph_type graph_type;
      typedef typename Config::StoredEdge StoredEdge;
      graph_type& g = static_cast<graph_type&>(g_);
      typename Config::OutEdgeList::iterator i; 
      bool inserted;
      boost::tie(i, inserted) = boost::push(g.out_edge_list(u), 
                                            StoredEdge(v, p));
      return std::make_pair(edge_descriptor(u, v, &(*i).get_property()), 
                            inserted);
    }
    // Did not use default argument here because that
    // causes Visual C++ to get confused.
    template <class Config>
    inline std::pair<typename Config::edge_descriptor, bool>
    add_edge(typename Config::vertex_descriptor u, 
             typename Config::vertex_descriptor v,
             directed_graph_helper<Config>& g_)
    {
      typename Config::edge_property_type p;
      return add_edge(u, v, p, g_);
    }
    //=========================================================================
    // Undirected Graph Helper Class

    template <class Config>
    struct undirected_graph_helper;

    struct undir_adj_list_traversal_tag : 
      public virtual vertex_and_edge_list_graph_tag,
      public virtual bidirectional_graph_tag { };

    namespace detail {
      // O(E/V)
      template <class edge_descriptor, class Config, class StoredProperty>
      inline void
      remove_undirected_edge_dispatch(edge_descriptor e, 
                                      undirected_graph_helper<Config>& g_, 
                                      StoredProperty& p)
      {
        typedef typename Config::graph_type graph_type;
        graph_type& g = static_cast<graph_type&>(g_);

        typename Config::OutEdgeList& out_el = g.out_edge_list(source(e, g));
        typename Config::OutEdgeList::iterator out_i = out_el.begin();
        for (; out_i != out_el.end(); ++out_i)
          if (&(*out_i).get_property() == &p) {
            g.m_edges.erase((*out_i).get_iter());
            out_el.erase(out_i);
            break;
          }
        typename Config::OutEdgeList& in_el = g.out_edge_list(target(e, g));
        typename Config::OutEdgeList::iterator in_i = in_el.begin();
        for (; in_i != in_el.end(); ++in_i)
          if (&(*in_i).get_property() == &p) {
            in_el.erase(in_i);
            return;
          }
      }
      // O(E/V)
      template <class edge_descriptor, class Config>
      inline void
      remove_undirected_edge_dispatch(edge_descriptor e,
                                      undirected_graph_helper<Config>& g_,
                                      no_property&)
      {
        typedef typename Config::graph_type graph_type;
        graph_type& g = static_cast<graph_type&>(g_);

        typename Config::OutEdgeList& out_el = g.out_edge_list(source(e, g));
        typename Config::OutEdgeList::iterator out_i = out_el.begin();
        for (; out_i != out_el.end(); ++out_i)
          if (&(*out_i).get_property() == (no_property*)e.get_property()) {
            g.m_edges.erase((*out_i).get_iter());
            out_el.erase(out_i);
            break;
          }
        typename Config::OutEdgeList& in_el = g.out_edge_list(target(e, g));
        typename Config::OutEdgeList::iterator in_i = in_el.begin();
        for (; in_i != in_el.end(); ++in_i)
          if (&(*in_i).get_property() == (no_property*)e.get_property()) {
            in_el.erase(in_i);
            return;
          }
      }

      // O(E/V)
      template <class Graph, class EdgeList, class Vertex>
      inline void
      remove_edge_and_property(Graph& g, EdgeList& el, Vertex v, 
                               boost::allow_parallel_edge_tag cat)
      {
        typedef typename EdgeList::value_type StoredEdge;
        typename EdgeList::iterator i = el.begin(), end = el.end();
        for (; i != end; ++i)
          if ((*i).get_target() == v)
            g.m_edges.erase((*i).get_iter());
        detail::erase_from_incidence_list(el, v, cat);
      }
      // O(log(E/V))
      template <class Graph, class EdgeList, class Vertex>
      inline void
      remove_edge_and_property(Graph& g, EdgeList& el, Vertex v, 
                               boost::disallow_parallel_edge_tag)
      {
        typedef typename EdgeList::value_type StoredEdge;
        typename EdgeList::iterator i = el.find(StoredEdge(v)), end = el.end();
        if (i != end) {
          g.m_edges.erase((*i).get_iter());
          el.erase(i);
        }
      }

    } // namespace detail

    template <class Vertex, class EdgeProperty>
    struct list_edge // short name due to VC++ truncation and linker problems
      : public boost::detail::edge_base<boost::undirected_tag, Vertex>
    {
      typedef EdgeProperty property_type;
      typedef boost::detail::edge_base<boost::undirected_tag, Vertex> Base;
      list_edge(Vertex u, Vertex v, const EdgeProperty& p = EdgeProperty())
        : Base(u, v), m_property(p) { }
      EdgeProperty& get_property() { return m_property; }
      const EdgeProperty& get_property() const { return m_property; }
      // the following methods should never be used, but are needed
      // to make SGI MIPSpro C++ happy
      list_edge() { }
      bool operator==(const list_edge&) const { return false; }
      bool operator<(const list_edge&) const { return false; }
      EdgeProperty m_property;
    };

    template <class Config>
    struct undirected_graph_helper {

      typedef undir_adj_list_traversal_tag traversal_category;

      // Placement of these overloaded remove_edge() functions
      // inside the class avoids a VC++ bug.

      // O(E/V)
      inline void
      remove_edge(typename Config::edge_descriptor e)
      {
        typedef typename Config::OutEdgeList::value_type::property_type PType;
        detail::remove_undirected_edge_dispatch
          (e, *this, *(PType*)e.get_property());
      }
      // O(E/V)
      inline void
      remove_edge(typename Config::out_edge_iterator iter)
      {
        this->remove_edge(*iter);
      }
    };

    // O(E/V)
    template <class EdgeOrIter, class Config>
    inline void
    remove_edge(EdgeOrIter e, undirected_graph_helper<Config>& g_)
    {
      g_.remove_edge(e);
    }

    // O(E/V) or O(log(E/V))
    template <class Config>
    void
    remove_edge(typename Config::vertex_descriptor u, 
                typename Config::vertex_descriptor v, 
                undirected_graph_helper<Config>& g_)
    {
      typedef typename Config::graph_type graph_type;
      graph_type& g = static_cast<graph_type&>(g_);
      typedef typename Config::edge_parallel_category Cat;
      detail::remove_edge_and_property(g, g.out_edge_list(u), v, Cat());
      detail::erase_from_incidence_list(g.out_edge_list(v), u, Cat());
    }
  
    template <class Config, class Predicate>
    void
    remove_out_edge_if(typename Config::vertex_descriptor u, Predicate pred,
                       undirected_graph_helper<Config>& g_)
    {
      typedef typename Config::graph_type graph_type;
      typedef typename Config::OutEdgeList::value_type::property_type PropT;
      graph_type& g = static_cast<graph_type&>(g_);

      // First remove the edges from the targets' lists and
      // from the graph's edge set list.
      typename Config::out_edge_iterator out_i, out_end;
      for (tie(out_i, out_end) = out_edges(u, g); out_i != out_end; ++out_i)
        if (pred(*out_i)) {
          typename Config::vertex_descriptor v = target(*out_i, g);
          detail::remove_directed_edge_dispatch
            (*out_i, g.out_edge_list(v), *(PropT*)(*out_i).get_property());
          g.m_edges.erase( (*out_i.iter()).get_iter() );
        }
      // Now remove the edges from this out-edge list.
      typename Config::out_edge_iterator first, last;
      tie(first, last) = out_edges(u, g);
      typedef typename Config::edge_parallel_category Cat;
      detail::remove_directed_edge_if_dispatch
        (first, last, g.out_edge_list(u), pred, Cat());
    }
    template <class Config, class Predicate>
    void
    remove_in_edge_if(typename Config::vertex_descriptor u, Predicate pred,
                      undirected_graph_helper<Config>& g_)
    {
      remove_out_edge_if(u, pred, g_);
    }

    // O(E/V * E) or O(log(E/V) * E)
    template <class Predicate, class Config>
    void
    remove_edge_if(Predicate pred, undirected_graph_helper<Config>& g_)
    {
      typedef typename Config::graph_type graph_type;
      graph_type& g = static_cast<graph_type&>(g_);
      typename Config::edge_iterator ei, ei_end, next;
      tie(ei, ei_end) = edges(g);
      for (next = ei; ei != ei_end; ei = next) {
        ++next;
        if (pred(*ei))
          remove_edge(*ei, g);
      }
    }

    // O(1)
    template <class Config>
    inline std::pair<typename Config::edge_iterator, 
                     typename Config::edge_iterator>
    edges(const undirected_graph_helper<Config>& g_)
    {
      typedef typename Config::graph_type graph_type;
      typedef typename Config::edge_iterator edge_iterator;
      const graph_type& cg = static_cast<const graph_type&>(g_);
      graph_type& g = const_cast<graph_type&>(cg);
      return std::make_pair( edge_iterator(g.m_edges.begin()),
                             edge_iterator(g.m_edges.end()) );
    }
    // O(1)
    template <class Config>
    inline typename Config::edges_size_type
    num_edges(const undirected_graph_helper<Config>& g_) 
    {
      typedef typename Config::graph_type graph_type;
      const graph_type& g = static_cast<const graph_type&>(g_);
      return g.m_edges.size();
    }
    // O(E/V * E/V)
    template <class Config>
    inline void 
    clear_vertex(typename Config::vertex_descriptor u,
                 undirected_graph_helper<Config>& g_)
    {
      typedef typename Config::graph_type graph_type;
      typedef typename Config::edge_parallel_category Cat;
      graph_type& g = static_cast<graph_type&>(g_);
      typename Config::OutEdgeList& el = g.out_edge_list(u);
      typename Config::OutEdgeList::iterator 
        ei = el.begin(), ei_end = el.end();
      for (; ei != ei_end; ++ei) {
        detail::erase_from_incidence_list
          (g.out_edge_list((*ei).get_target()), u, Cat());
        g.m_edges.erase((*ei).get_iter());
      }
      g.out_edge_list(u).clear();
    }
    // O(1) for allow_parallel_edge_tag
    // O(log(E/V)) for disallow_parallel_edge_tag
    template <class Config>
    inline std::pair<typename Config::edge_descriptor, bool>
    add_edge(typename Config::vertex_descriptor u, 
             typename Config::vertex_descriptor v, 
             const typename Config::edge_property_type& p,
             undirected_graph_helper<Config>& g_)
    {
      typedef typename Config::StoredEdge StoredEdge;
      typedef typename Config::edge_descriptor edge_descriptor;
      typedef typename Config::graph_type graph_type;
      graph_type& g = static_cast<graph_type&>(g_);

      assert( u != v ); // don't allow self-loops 

      bool inserted;
      typename Config::EdgeContainer::value_type e(u, v, p);
      g.m_edges.push_back(e);
      typename Config::EdgeContainer::iterator p_iter 
	= boost::prior(g.m_edges.end());
      typename Config::OutEdgeList::iterator i;
      boost::tie(i, inserted) = boost::push(g.out_edge_list(u), 
					    StoredEdge(v, p_iter));
      if (inserted) {
	boost::push(g.out_edge_list(v), StoredEdge(u, p_iter));
	return std::make_pair(edge_descriptor(u, v, &p_iter->get_property()),
			      true);
      } else {
	g.m_edges.erase(p_iter);
	return std::make_pair
	  (edge_descriptor(u, v, &i->get_iter()->get_property()), false);
      }
    }
    template <class Config>
    inline std::pair<typename Config::edge_descriptor, bool>
    add_edge(typename Config::vertex_descriptor u, 
             typename Config::vertex_descriptor v, 
             undirected_graph_helper<Config>& g_)
    {
      typename Config::edge_property_type p;
      return add_edge(u, v, p, g_);
    }

    // O(1)
    template <class Config>
    inline typename Config::degree_size_type
    degree(typename Config::vertex_descriptor u, 
           const undirected_graph_helper<Config>& g_)
    {
      typedef typename Config::graph_type Graph;
      const Graph& g = static_cast<const Graph&>(g_);
      return out_degree(u, g);
    }

    template <class Config>
    inline std::pair<typename Config::in_edge_iterator, 
                     typename Config::in_edge_iterator>
    in_edges(typename Config::vertex_descriptor u, 
             const undirected_graph_helper<Config>& g_)
    {
      typedef typename Config::graph_type Graph;
      const Graph& cg = static_cast<const Graph&>(g_);
      Graph& g = const_cast<Graph&>(cg);
      typedef typename Config::in_edge_iterator in_edge_iterator;
      return
        std::make_pair(in_edge_iterator(g.out_edge_list(u).begin(), u),
                       in_edge_iterator(g.out_edge_list(u).end(), u));
    }

    //=========================================================================
    // Bidirectional Graph Helper Class

    struct bidir_adj_list_traversal_tag : 
      public virtual vertex_and_edge_list_graph_tag,
      public virtual bidirectional_graph_tag { };

    template <class Config>
    struct bidirectional_graph_helper
      : public directed_edges_helper<Config> {
      typedef bidir_adj_list_traversal_tag traversal_category;
    };

    template <class Predicate, class Config>
    inline void
    remove_edge_if(Predicate pred, bidirectional_graph_helper<Config>& g_)
    {
      typedef typename Config::graph_type graph_type;
      graph_type& g = static_cast<graph_type&>(g_);
      typename Config::edge_iterator ei, ei_end, next;
      tie(ei, ei_end) = edges(g);
      for (next = ei; ei != ei_end; ei = next) {
        ++next;
        if (pred(*ei))
          remove_edge(*ei, g);
      }
    }

    template <class Config>
    inline std::pair<typename Config::in_edge_iterator, 
                     typename Config::in_edge_iterator>
    in_edges(typename Config::vertex_descriptor u, 
             const bidirectional_graph_helper<Config>& g_)
    {
      typedef typename Config::graph_type graph_type;
      const graph_type& cg = static_cast<const graph_type&>(g_);
      graph_type& g = const_cast<graph_type&>(cg);
      typedef typename Config::in_edge_iterator in_edge_iterator;
      return
        std::make_pair(in_edge_iterator(in_edge_list(g, u).begin(), u),
                       in_edge_iterator(in_edge_list(g, u).end(), u));
    }

    // O(1)
    template <class Config>
    inline std::pair<typename Config::edge_iterator, 
                     typename Config::edge_iterator>
    edges(const bidirectional_graph_helper<Config>& g_)
    {
      typedef typename Config::graph_type graph_type;
      typedef typename Config::edge_iterator edge_iterator;
      const graph_type& cg = static_cast<const graph_type&>(g_);
      graph_type& g = const_cast<graph_type&>(cg);
      return std::make_pair( edge_iterator(g.m_edges.begin()),
                             edge_iterator(g.m_edges.end()) );
    }

    //=========================================================================
    // Bidirectional Graph Helper Class (with edge properties)


    template <class Config>
    struct bidirectional_graph_helper_with_property
      : public bidirectional_graph_helper<Config>
    {
      // Placement of these overloaded remove_edge() functions
      // inside the class avoids a VC++ bug.
      
      // O(E/V)
      inline void
      remove_edge(typename Config::edge_descriptor e)
      {
        typedef typename Config::graph_type graph_type;
        graph_type& g = static_cast<graph_type&>(*this);

        typedef typename Config::OutEdgeList::value_type::property_type PType;

        typename Config::OutEdgeList& out_el = g.out_edge_list(source(e, g));
        typename Config::OutEdgeList::iterator out_i = out_el.begin();
        for (; out_i != out_el.end(); ++out_i)
          if (&(*out_i).get_property() == (PType*)e.get_property()) {
            g.m_edges.erase((*out_i).get_iter());
            out_el.erase(out_i);
            break;
          }
        typename Config::InEdgeList& in_el = in_edge_list(g, target(e, g));
        typename Config::InEdgeList::iterator in_i = in_el.begin();
        for (; in_i != in_el.end(); ++in_i)
          if (&(*in_i).get_property() == (PType*)e.get_property()) {
            in_el.erase(in_i);
            break;
          }
      }
      inline void
      remove_edge(typename Config::out_edge_iterator iter)
      {
        this->remove_edge(*iter);
      }
    };

    // O(E/V) for allow_parallel_edge_tag
    // O(log(E/V)) for disallow_parallel_edge_tag
    template <class Config>
    inline void
    remove_edge(typename Config::vertex_descriptor u, 
                typename Config::vertex_descriptor v, 
                bidirectional_graph_helper_with_property<Config>& g_)
    {
      typedef typename Config::graph_type graph_type;
      graph_type& g = static_cast<graph_type&>(g_);
      typedef typename Config::edge_parallel_category Cat;
      detail::remove_edge_and_property(g, g.out_edge_list(u), v, Cat());
      detail::erase_from_incidence_list(in_edge_list(g, v), u, Cat());
    }
    // O(E/V)
    template <class EdgeOrIter, class Config>
    inline void
    remove_edge(EdgeOrIter e,
                bidirectional_graph_helper_with_property<Config>& g_)
    {
      g_.remove_edge(e);
    }

    template <class Config, class Predicate>
    inline void
    remove_out_edge_if(typename Config::vertex_descriptor u, Predicate pred,
                       bidirectional_graph_helper_with_property<Config>& g_)
    {
      typedef typename Config::graph_type graph_type;
      typedef typename Config::OutEdgeList::value_type::property_type PropT;
      graph_type& g = static_cast<graph_type&>(g_);

      typedef std::vector<typename Config::EdgeIter> Garbage;
      Garbage garbage;

      // First remove the edges from the targets' in-edge lists and
      // from the graph's edge set list.
      typename Config::out_edge_iterator out_i, out_end;
      for (tie(out_i, out_end) = out_edges(u, g); out_i != out_end; ++out_i)
        if (pred(*out_i)) {
          detail::remove_directed_edge_dispatch
            (*out_i, in_edge_list(g, target(*out_i, g)),
             *(PropT*)(*out_i).get_property());
          // Put in garbage to delete later. Will need the properties
          // for the remove_if of the out-edges.
          garbage.push_back((*out_i.iter()).get_iter());
        }

      // Now remove the edges from this out-edge list.
      typename Config::out_edge_iterator first, last;
      tie(first, last) = out_edges(u, g);
      typedef typename Config::edge_parallel_category Cat;
      detail::remove_directed_edge_if_dispatch
        (first, last, g.out_edge_list(u), pred, Cat());

      // Now delete the edge properties from the g.m_edges list
      for (typename Garbage::iterator i = garbage.begin();
           i != garbage.end(); ++i)
        g.m_edges.erase(*i);
    }
    template <class Config, class Predicate>
    inline void
    remove_in_edge_if(typename Config::vertex_descriptor v, Predicate pred,
                      bidirectional_graph_helper_with_property<Config>& g_)
    {
      typedef typename Config::graph_type graph_type;
      typedef typename Config::OutEdgeList::value_type::property_type PropT;
      graph_type& g = static_cast<graph_type&>(g_);

      typedef std::vector<typename Config::EdgeIter> Garbage;
      Garbage garbage;

      // First remove the edges from the sources' out-edge lists and
      // from the graph's edge set list.
      typename Config::in_edge_iterator in_i, in_end;
      for (tie(in_i, in_end) = in_edges(v, g); in_i != in_end; ++in_i)
        if (pred(*in_i)) {
          typename Config::vertex_descriptor u = source(*in_i, g);
          detail::remove_directed_edge_dispatch
            (*in_i, g.out_edge_list(u), *(PropT*)(*in_i).get_property());
          // Put in garbage to delete later. Will need the properties
          // for the remove_if of the out-edges.
          garbage.push_back((*in_i.iter()).get_iter());
        }
      // Now remove the edges from this in-edge list.
      typename Config::in_edge_iterator first, last;
      tie(first, last) = in_edges(v, g);
      typedef typename Config::edge_parallel_category Cat;
      detail::remove_directed_edge_if_dispatch
        (first, last, in_edge_list(g, v), pred, Cat());

      // Now delete the edge properties from the g.m_edges list
      for (typename Garbage::iterator i = garbage.begin();
           i != garbage.end(); ++i)
        g.m_edges.erase(*i);
    }

    // O(1)
    template <class Config>
    inline typename Config::edges_size_type
    num_edges(const bidirectional_graph_helper_with_property<Config>& g_) 
    {
      typedef typename Config::graph_type graph_type;
      const graph_type& g = static_cast<const graph_type&>(g_);
      return g.m_edges.size();
    }
    // O(E/V * E/V) for allow_parallel_edge_tag
    // O(E/V * log(E/V)) for disallow_parallel_edge_tag
    template <class Config>
    inline void
    clear_vertex(typename Config::vertex_descriptor u,
                 bidirectional_graph_helper_with_property<Config>& g_)
    {
      typedef typename Config::graph_type graph_type;
      typedef typename Config::edge_parallel_category Cat;
      graph_type& g = static_cast<graph_type&>(g_);
      typename Config::OutEdgeList& el = g.out_edge_list(u);
      typename Config::OutEdgeList::iterator 
        ei = el.begin(), ei_end = el.end();
      for (; ei != ei_end; ++ei) {
        detail::erase_from_incidence_list
          (in_edge_list(g, (*ei).get_target()), u, Cat());
        g.m_edges.erase((*ei).get_iter());
      }      
      typename Config::InEdgeList& in_el = in_edge_list(g, u);
      typename Config::InEdgeList::iterator 
        in_ei = in_el.begin(), in_ei_end = in_el.end();
      for (; in_ei != in_ei_end; ++in_ei) {
        detail::erase_from_incidence_list
          (g.out_edge_list((*in_ei).get_target()), u, Cat());
        g.m_edges.erase((*in_ei).get_iter());   
      }
      g.out_edge_list(u).clear();
      in_edge_list(g, u).clear();
    }

    // O(1) for allow_parallel_edge_tag
    // O(log(E/V)) for disallow_parallel_edge_tag
    template <class Config>
    inline std::pair<typename Config::edge_descriptor, bool>
    add_edge(typename Config::vertex_descriptor u,
             typename Config::vertex_descriptor v, 
             const typename Config::edge_property_type& p,
             bidirectional_graph_helper_with_property<Config>& g_)
    {
      typedef typename Config::graph_type graph_type;
      graph_type& g = static_cast<graph_type&>(g_);
      typedef typename Config::edge_descriptor edge_descriptor;
      typedef typename Config::StoredEdge StoredEdge;
      bool inserted;
      typename Config::EdgeContainer::value_type e(u, v, p);
      g.m_edges.push_back(e);
      typename Config::EdgeContainer::iterator p_iter 
        = boost::prior(g.m_edges.end());
      typename Config::OutEdgeList::iterator i;
      boost::tie(i, inserted) = boost::push(g.out_edge_list(u), 
                                            StoredEdge(v, p_iter));
      if (inserted) {
        boost::push(in_edge_list(g, v), StoredEdge(u, p_iter));
        return std::make_pair(edge_descriptor(u, v, &p_iter->m_property), 
                              true);
      } else {
        g.m_edges.erase(p_iter);
        return std::make_pair(edge_descriptor(u, v, 
                                              &i->get_iter()->get_property()), 
                              false);
      }
    }

    template <class Config>
    inline std::pair<typename Config::edge_descriptor, bool>
    add_edge(typename Config::vertex_descriptor u,
             typename Config::vertex_descriptor v,
             bidirectional_graph_helper_with_property<Config>& g_)
    {
      typename Config::edge_property_type p;
      return add_edge(u, v, p, g_);
    }
    // O(1)
    template <class Config>
    inline typename Config::degree_size_type
    degree(typename Config::vertex_descriptor u, 
           const bidirectional_graph_helper_with_property<Config>& g_)
    {
      typedef typename Config::graph_type graph_type;
      const graph_type& g = static_cast<const graph_type&>(g_);
      return in_degree(u, g) + out_degree(u, g);
    }

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
      typedef typename Config::inv_adjacency_iterator inv_adjacency_iterator;
      typedef typename Config::vertex_iterator vertex_iterator;
      typedef typename Config::edge_iterator edge_iterator;
      typedef typename Config::directed_category directed_category;
      typedef typename Config::edge_parallel_category edge_parallel_category;
      typedef typename Config::vertices_size_type vertices_size_type;
      typedef typename Config::edges_size_type edges_size_type;
      typedef typename Config::degree_size_type degree_size_type;
      typedef typename Config::StoredEdge StoredEdge;
      typedef typename Config::edge_property_type edge_property_type;

      //    protected:

      // The edge_dispatch() functions should be static, but
      // Borland gets confused about constness.

      // O(E/V)
      inline std::pair<edge_descriptor,bool>      
      edge_dispatch(const AdjList& g, 
                    vertex_descriptor u, vertex_descriptor v, 
                    boost::allow_parallel_edge_tag) const
      {
        bool found;
        const typename Config::OutEdgeList& el = g.out_edge_list(u);
        typename Config::OutEdgeList::const_iterator 
          i = std::find_if(el.begin(), el.end(), 
                           detail::target_is<vertex_descriptor>(v));
        found = (i != g.out_edge_list(u).end());
        if (found)
          return std::make_pair(edge_descriptor(u, v, &(*i).get_property()),
                                true);
        else
          return std::make_pair(edge_descriptor(u, v, 0), false);
      }
      // O(log(E/V))
      inline std::pair<edge_descriptor,bool>      
      edge_dispatch(const AdjList& g, 
                    vertex_descriptor u, vertex_descriptor v, 
                    boost::disallow_parallel_edge_tag) const
      {
        bool found;
        typename Config::OutEdgeList::const_iterator 
          i = std::find(g.out_edge_list(u).begin(),
			g.out_edge_list(u).end(), StoredEdge(v)),
          end = g.out_edge_list(u).end();
        found = (i != end);
        if (found)
          return std::make_pair(edge_descriptor(u, v, &(*i).get_property()),
                                true);
        else
          return std::make_pair(edge_descriptor(u, v, 0), false);
      }
    };

    template <class Config, class Base>
    inline std::pair<typename Config::adjacency_iterator, 
                     typename Config::adjacency_iterator>
    adjacent_vertices(typename Config::vertex_descriptor u, 
                      const adj_list_helper<Config, Base>& g_)
    {
      typedef typename Config::graph_type AdjList;
      const AdjList& cg = static_cast<const AdjList&>(g_);
      AdjList& g = const_cast<AdjList&>(cg);
      typedef typename Config::adjacency_iterator adjacency_iterator;
      typename Config::out_edge_iterator first, last;
      boost::tie(first, last) = out_edges(u, g);
      return std::make_pair(adjacency_iterator(first, &g),
                            adjacency_iterator(last, &g));
    }
    template <class Config, class Base>
    inline std::pair<typename Config::inv_adjacency_iterator, 
                     typename Config::inv_adjacency_iterator>
    inv_adjacent_vertices(typename Config::vertex_descriptor u, 
			  const adj_list_helper<Config, Base>& g_)
    {
      typedef typename Config::graph_type AdjList;
      const AdjList& cg = static_cast<const AdjList&>(g_);
      AdjList& g = const_cast<AdjList&>(cg);
      typedef typename Config::inv_adjacency_iterator inv_adjacency_iterator;
      typename Config::in_edge_iterator first, last;
      boost::tie(first, last) = in_edges(u, g);
      return std::make_pair(inv_adjacency_iterator(first, &g),
                            inv_adjacency_iterator(last, &g));
    }
    template <class Config, class Base>
    inline std::pair<typename Config::out_edge_iterator, 
                     typename Config::out_edge_iterator>
    out_edges(typename Config::vertex_descriptor u, 
              const adj_list_helper<Config, Base>& g_)
    {
      typedef typename Config::graph_type AdjList;
      typedef typename Config::out_edge_iterator out_edge_iterator;
      const AdjList& cg = static_cast<const AdjList&>(g_);
      AdjList& g = const_cast<AdjList&>(cg);
      return
        std::make_pair(out_edge_iterator(g.out_edge_list(u).begin(), u),
                       out_edge_iterator(g.out_edge_list(u).end(), u));
    }
    template <class Config, class Base>
    inline std::pair<typename Config::vertex_iterator, 
                     typename Config::vertex_iterator>
    vertices(const adj_list_helper<Config, Base>& g_)
    {
      typedef typename Config::graph_type AdjList;
      const AdjList& cg = static_cast<const AdjList&>(g_);
      AdjList& g = const_cast<AdjList&>(cg);
      return std::make_pair( g.vertex_set().begin(), g.vertex_set().end() );
    }
    template <class Config, class Base>
    inline typename Config::vertices_size_type
    num_vertices(const adj_list_helper<Config, Base>& g_)
    {
      typedef typename Config::graph_type AdjList;
      const AdjList& g = static_cast<const AdjList&>(g_);
      return g.vertex_set().size();
    }
    template <class Config, class Base>
    inline typename Config::degree_size_type
    out_degree(typename Config::vertex_descriptor u, 
               const adj_list_helper<Config, Base>& g_)
    {
      typedef typename Config::graph_type AdjList;
      const AdjList& g = static_cast<const AdjList&>(g_);
      return g.out_edge_list(u).size();
    }
    template <class Config, class Base>
    inline std::pair<typename Config::edge_descriptor, bool>
    edge(typename Config::vertex_descriptor u, 
         typename Config::vertex_descriptor v, 
         const adj_list_helper<Config, Base>& g_)
    {
      typedef typename Config::graph_type Graph;
      typedef typename Config::edge_parallel_category Cat;
      const Graph& g = static_cast<const Graph&>(g_);
      return g_.edge_dispatch(g, u, v, Cat());
    }
    template <class Config, class Base>
    inline std::pair<typename Config::out_edge_iterator,
                     typename Config::out_edge_iterator>
    edge_range(typename Config::vertex_descriptor u,
               typename Config::vertex_descriptor v,
               const adj_list_helper<Config, Base>& g_)
    {
      typedef typename Config::graph_type Graph;
      typedef typename Config::StoredEdge StoredEdge;
      const Graph& cg = static_cast<const Graph&>(g_);
      Graph& g = const_cast<Graph&>(cg);
      typedef typename Config::out_edge_iterator out_edge_iterator;
      typename Config::OutEdgeList& el = g.out_edge_list(u);
      typename Config::OutEdgeList::iterator first, last;
      tie(first, last) = std::equal_range(el.begin(), el.end(), StoredEdge(v));
      return std::make_pair(out_edge_iterator(first, u),
                            out_edge_iterator(last, u));
    }

    template <class Config, class Base>
    inline typename Config::degree_size_type
    in_degree(typename Config::vertex_descriptor u, 
              const adj_list_helper<Config,Base>& g_)
    {
      typedef typename Config::graph_type Graph;
      const Graph& cg = static_cast<const Graph&>(g_);
      Graph& g = const_cast<Graph&>(cg);
      return in_edge_list(g, u).size();
    }

    namespace detail {
      template <class Config, class Base, class Property>
      inline
      typename boost::property_map<typename Config::graph_type,
        Property>::type
      get_dispatch(adj_list_helper<Config,Base>&, Property, 
                   boost::edge_property_tag) {
        typedef typename Config::graph_type Graph;
        typedef typename boost::property_map<Graph, Property>::type PA;
        return PA();
      }
      template <class Config, class Base, class Property>
      inline
      typename boost::property_map<typename Config::graph_type, 
        Property>::const_type
      get_dispatch(const adj_list_helper<Config,Base>&, Property, 
                   boost::edge_property_tag) {
        typedef typename Config::graph_type Graph;
        typedef typename boost::property_map<Graph, Property>::const_type PA;
        return PA();
      }

      template <class Config, class Base, class Property>
      inline
      typename boost::property_map<typename Config::graph_type, 
        Property>::type
      get_dispatch(adj_list_helper<Config,Base>& g, Property, 
                   boost::vertex_property_tag) {
        typedef typename Config::graph_type Graph;
        typedef typename boost::property_map<Graph, Property>::type PA;
        return PA(static_cast<Graph&>(g));
      }
      template <class Config, class Base, class Property>
      inline
      typename boost::property_map<typename Config::graph_type,
        Property>::const_type
      get_dispatch(const adj_list_helper<Config, Base>& g, Property, 
                   boost::vertex_property_tag) {
        typedef typename Config::graph_type Graph;
        typedef typename boost::property_map<Graph, Property>::const_type PA;
        const Graph& cg = static_cast<const Graph&>(g);
        return PA(const_cast<Graph&>(cg));
      }

    } // namespace detail

    // Implementation of the PropertyGraph interface
    template <class Config, class Base, class Property>
    inline
    typename boost::property_map<typename Config::graph_type, Property>::type
    get(Property p, adj_list_helper<Config, Base>& g) {
      typedef typename property_kind<Property>::type Kind;
      return detail::get_dispatch(g, p, Kind());
    }
    template <class Config, class Base, class Property>
    inline
    typename boost::property_map<typename Config::graph_type, 
      Property>::const_type
    get(Property p, const adj_list_helper<Config, Base>& g) {
      typedef typename property_kind<Property>::type Kind;
      return detail::get_dispatch(g, p, Kind());
    }

    template <class Config, class Base, class Property, class Key>
    inline
    typename boost::property_traits<
      typename boost::property_map<typename Config::graph_type, 
        Property>::const_type
    >::value_type
    get(Property p, const adj_list_helper<Config, Base>& g, const Key& key) {
      return get(get(p, g), key);
    }

    template <class Config, class Base, class Property, class Key,class Value>
    inline void
    put(Property p, adj_list_helper<Config, Base>& g, 
        const Key& key, const Value& value)
    {
      typedef typename Config::graph_type Graph;
      typedef typename boost::property_map<Graph, Property>::type Map;
      Map pmap = get(p, static_cast<Graph&>(g));
      put(pmap, key, value);
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
      typedef typename Config::edge_descriptor edge_descriptor;
      typedef typename Config::vertex_iterator vertex_iterator;
      typedef typename Config::edge_iterator edge_iterator;
      typedef typename Config::edge_parallel_category edge_parallel_category;
      typedef typename Config::vertices_size_type vertices_size_type;
      typedef typename Config::edges_size_type edges_size_type;
      typedef typename Config::degree_size_type degree_size_type;
      typedef typename Config::edge_property_type edge_property_type;
      typedef adj_list_tag graph_tag;

      inline adj_list_impl() { }

      inline adj_list_impl(const adj_list_impl& x) {
        copy_impl(x);
      }
      inline adj_list_impl& operator=(const adj_list_impl& x) {
        this->clear();
        copy_impl(x);
        return *this;
      }
      inline void clear() {
        m_vertices.clear();
        m_edges.clear();
      }
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
          add_edge(v[(*first).first], v[(*first).second], *this);
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
          add_edge(v[(*first).first], v[(*first).second], *ep_iter, *this);
          ++first;
          ++ep_iter;
        }
        delete [] v;
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
      inline StoredVertexList& vertex_set() { return m_vertices; }
      inline const StoredVertexList& vertex_set() const { return m_vertices; }

      inline void copy_impl(const adj_list_impl& x_) 
      {
        const Derived& x = static_cast<const Derived&>(x_);

        // Would be better to have a constant time way to get from
        // vertices in x to the corresponding vertices in *this.
        std::map<stored_vertex*,stored_vertex*> vertex_map;

        // Copy the stored vertex objects by adding each vertex
        // and copying its property object.
        vertex_iterator vi, vi_end;
        for (tie(vi, vi_end) = vertices(x); vi != vi_end; ++vi) {
          stored_vertex* v = (stored_vertex*)add_vertex(*this);
          v->m_property = ((stored_vertex*)*vi)->m_property;
          vertex_map[(stored_vertex*)*vi] = v;
        }
        // Copy the edges by adding each edge and copying its
        // property object.
        edge_iterator ei, ei_end;
        for (tie(ei, ei_end) = edges(x); ei != ei_end; ++ei) {
          edge_descriptor e;
          bool inserted; 
          vertex_descriptor s = source(*ei,x), t = target(*ei,x);
          tie(e, inserted) = add_edge(vertex_map[(stored_vertex*)s], 
                                      vertex_map[(stored_vertex*)t], *this);
          *((edge_property_type*)e.m_eproperty)
            = *((edge_property_type*)(*ei).m_eproperty);
        }
      }


      typename Config::EdgeContainer m_edges;
      StoredVertexList m_vertices;
    };
    // Had to make these non-members to avoid accidental instantiation
    // on SGI MIPSpro C++
    template <class D, class C, class B>
    inline typename C::InEdgeList& 
    in_edge_list(adj_list_impl<D,C,B>&, 
		 typename C::vertex_descriptor v)
    {
      typename C::stored_vertex* sv = (typename C::stored_vertex*)v;
      return sv->m_in_edges;
    }
    template <class D, class C, class B>
    inline const typename C::InEdgeList& 
    in_edge_list(const adj_list_impl<D,C,B>&,
		 typename C::vertex_descriptor v) {
      typename C::stored_vertex* sv = (typename C::stored_vertex*)v;
      return sv->m_in_edges;
    }


    // O(1)
    template <class Derived, class Config, class Base>
    inline typename Config::vertex_descriptor
    add_vertex(adj_list_impl<Derived, Config, Base>& g_)
    {
      Derived& g = static_cast<Derived&>(g_);
      typedef typename Config::stored_vertex stored_vertex;
      stored_vertex* v = new stored_vertex;
      typename Config::StoredVertexList::iterator pos;
      bool inserted;
      boost::tie(pos,inserted) = boost::push(g.m_vertices, v);
      v->m_position = pos;
      return v;
    }
    // O(1)
    template <class Derived, class Config, class Base>
    inline typename Config::vertex_descriptor
    add_vertex(const typename Config::vertex_property_type& p,
               adj_list_impl<Derived, Config, Base>& g_)
    {
      Derived& g = static_cast<Derived&>(g_);
      typedef typename Config::stored_vertex stored_vertex;
      stored_vertex* v = new stored_vertex(p);
      typename Config::StoredVertexList::iterator pos;
      bool inserted;
      boost::tie(pos,inserted) = boost::push(g.m_vertices, v);
      v->m_position = pos;
      return v;
    }
    // O(1)
    template <class Derived, class Config, class Base>
    inline void remove_vertex(typename Config::vertex_descriptor u,
                              adj_list_impl<Derived, Config, Base>& g_)
    {
      typedef typename Config::stored_vertex stored_vertex;
      Derived& g = static_cast<Derived&>(g_);
      stored_vertex* su = (stored_vertex*)u;
      g.m_vertices.erase(su->m_position);
      delete su;
    }
    // O(V)
    template <class Derived, class Config, class Base>
    inline typename Config::vertex_descriptor
    vertex(typename Config::vertices_size_type n, 
           const adj_list_impl<Derived, Config, Base>& g_)
    {
      const Derived& g = static_cast<const Derived&>(g_);
      typename Config::vertex_iterator i = vertices(g).first;
      while (n--) ++i; // std::advance(i, n); (not VC++ portable)
      return *i;
    }

    //=========================================================================
    // Vector-Backbone Adjacency List Implementation

    namespace detail {

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
        typedef typename Graph::EdgeContainer Container;
        typedef typename Container::iterator Iter;
        Iter ei = g.m_edges.begin(), ei_end = g.m_edges.end();
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
          reindex_edge_list(in_edge_list(g, v), u, 
                            edge_parallel_category());

        typedef typename Graph::EdgeContainer Container;
        typedef typename Container::iterator Iter;
        Iter ei = g.m_edges.begin(), ei_end = g.m_edges.end();
        for (; ei != ei_end; ++ei) {
          if (ei->m_source > u)
            --ei->m_source;
          if (ei->m_target > u)
            --ei->m_target;
        }
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
    } // namespace detail

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
      typedef typename Config::edge_iterator edge_iterator;
      typedef typename Config::directed_category directed_category;
      typedef typename Config::vertices_size_type vertices_size_type;
      typedef typename Config::edges_size_type edges_size_type;
      typedef typename Config::degree_size_type degree_size_type;
      typedef typename Config::StoredEdge StoredEdge;
      typedef typename Config::stored_vertex stored_vertex;
      typedef typename Config::EdgeContainer EdgeContainer;
      typedef typename Config::edge_property_type edge_property_type;
      typedef vec_adj_list_tag graph_tag;

      inline vec_adj_list_impl() { }

      inline vec_adj_list_impl(const vec_adj_list_impl& x) {
        copy_impl(x);
      }
      inline vec_adj_list_impl& operator=(const vec_adj_list_impl& x) {
        this->clear();
        copy_impl(x);
        return *this;
      }
      inline void clear() {
        m_vertices.clear();
	clear_dispatch(edge_property_type());
      }
      template <typename Property>
      inline void clear_dispatch(const Property&) {
        m_edges.clear();
      }
      inline void clear_dispatch(const no_property&) { }

      inline vec_adj_list_impl(vertices_size_type _num_vertices)
        : m_vertices(_num_vertices) { }

      template <class EdgeIterator>
      inline vec_adj_list_impl(vertices_size_type num_vertices,
                               EdgeIterator first, EdgeIterator last)
        : m_vertices(num_vertices)
      {
        while (first != last) {
          add_edge((*first).first, (*first).second, 
                   static_cast<Graph&>(*this));
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
          add_edge((*first).first, (*first).second, *ep_iter, 
                   static_cast<Graph&>(*this));
          ++first;
          ++ep_iter;
        }
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
      inline void copy_impl(const vec_adj_list_impl& x_) 
      {
        const Graph& x = static_cast<const Graph&>(x_);
        // Copy the stored vertex objects by adding each vertex
        // and copying its property object.
        for (vertices_size_type i = 0; i < num_vertices(x); ++i) {
          vertex_descriptor v = add_vertex(*this);
          m_vertices[v].m_property = x.m_vertices[i].m_property;
        }
        // Copy the edges by adding each edge and copying its
        // property object.
        edge_iterator ei, ei_end;
        for (tie(ei, ei_end) = edges(x); ei != ei_end; ++ei) {
          edge_descriptor e;
          bool inserted; 
          tie(e, inserted) = add_edge(source(*ei,x), target(*ei,x) , *this);
          *((edge_property_type*)e.m_eproperty)
            = *((edge_property_type*)(*ei).m_eproperty);
        }
      }
      typename Config::EdgeContainer m_edges;
      StoredVertexList m_vertices;
    };
    // Had to make these non-members to avoid accidental instantiation
    // on SGI MIPSpro C++
    template <class G, class C, class B>
    inline typename C::InEdgeList& 
    in_edge_list(vec_adj_list_impl<G,C,B>& g, 
		 typename C::vertex_descriptor v) {
      return g.m_vertices[v].m_in_edges;
    }
    template <class G, class C, class B>
    inline const typename C::InEdgeList& 
    in_edge_list(const vec_adj_list_impl<G,C,B>& g, 
		 typename C::vertex_descriptor v) {
      return g.m_vertices[v].m_in_edges;
    }

      // O(1)
    template <class Graph, class Config, class Base>
    inline typename Config::vertex_descriptor
    add_vertex(vec_adj_list_impl<Graph, Config, Base>& g_) {
      Graph& g = static_cast<Graph&>(g_);
      g.m_vertices.resize(g.m_vertices.size() + 1);
      return g.m_vertices.size() - 1;
    }

    template <class Graph, class Config, class Base>
    inline typename Config::vertex_descriptor
    add_vertex(const typename Config::vertex_property_type& p,
               vec_adj_list_impl<Graph, Config, Base>& g_) {
      Graph& g = static_cast<Graph&>(g_);
      typedef typename Config::stored_vertex stored_vertex;
      g.m_vertices.push_back(stored_vertex(p));
      return g.m_vertices.size() - 1;
    }

    // Here we override the directed_graph_helper add_edge() function
    // so that the number of vertices is automatically changed if
    // either u or v is greater than the number of vertices.
    template <class Graph, class Config, class Base>
    inline std::pair<typename Config::edge_descriptor, bool>
    add_edge(typename Config::vertex_descriptor u, 
             typename Config::vertex_descriptor v,
             const typename Config::edge_property_type& p,
             vec_adj_list_impl<Graph, Config, Base>& g_)
    {
      typename Config::vertex_descriptor x = std::max(u, v);
      if (x >= num_vertices(g_))
        g_.m_vertices.resize(x + 1);
      adj_list_helper<Config, Base>& g = g_;
      return add_edge(u, v, p, g);
    }
    template <class Graph, class Config, class Base>
    inline std::pair<typename Config::edge_descriptor, bool>
    add_edge(typename Config::vertex_descriptor u, 
             typename Config::vertex_descriptor v,
             vec_adj_list_impl<Graph, Config, Base>& g_)
    {
      typename Config::edge_property_type p;
      return add_edge(u, v, p, g_);
    }


    // O(V + E)
    template <class Graph, class Config, class Base>
    inline void remove_vertex(typename Config::vertex_descriptor v,
                              vec_adj_list_impl<Graph, Config, Base>& g_)
    {
      typedef typename Config::directed_category Cat;
      Graph& g = static_cast<Graph&>(g_);
      detail::remove_vertex_dispatch(g, v, Cat());
    }
    // O(1)
    template <class Graph, class Config, class Base>
    inline typename Config::vertex_descriptor 
    vertex(typename Config::vertices_size_type n, 
           const vec_adj_list_impl<Graph, Config, Base>&)
    {
      return n;
    }


  namespace detail {

    //=========================================================================
    // Adjacency List Generator

    template <class Graph, class VertexListS, class EdgeListS,
              class DirectedS, class VertexProperty, class EdgeProperty, 
              class GraphProperty>
    struct adj_list_gen
    {
      typedef typename detail::is_random_access<VertexListS>::type 
        is_rand_access;
      typedef typename has_property<EdgeProperty>::type has_edge_property; 
      typedef typename DirectedS::is_directed_t DirectedT;
      typedef typename DirectedS::is_bidir_t BidirectionalT;

      struct config
      {
        typedef EdgeListS edgelist_selector;

        typedef Graph graph_type;
        typedef EdgeProperty edge_property_type;
        typedef VertexProperty vertex_property_type;
        typedef GraphProperty graph_property_type;
        typedef std::size_t vertices_size_type;

        typedef adjacency_list_traits<EdgeListS, VertexListS, DirectedS> 
           Traits;

        typedef typename Traits::directed_category directed_category;
        typedef typename Traits::edge_parallel_category edge_parallel_category;
        typedef typename Traits::vertex_descriptor vertex_descriptor;
        typedef typename Traits::edge_descriptor edge_descriptor;

        typedef void* vertex_ptr; 

        // need to reorganize this to avoid instantiating stuff
        // that doesn't get used -JGS

        // VertexList and vertex_iterator
        typedef typename container_gen<VertexListS, 
          vertex_ptr>::type SeqVertexList;
        typedef boost::integer_range<std::size_t> RandVertexList;
        typedef typename boost::ct_if_t<is_rand_access,
          RandVertexList, SeqVertexList>::type VertexList;

        typedef typename VertexList::iterator vertex_iterator;

        // EdgeContainer and StoredEdge

        typedef std::list< list_edge<vertex_descriptor,EdgeProperty> >
          EdgeContainer;

        typedef typename ct_and<DirectedT, 
             typename ct_not<BidirectionalT>::type >::type on_edge_storage;

        typedef typename boost::ct_if_t<on_edge_storage,
	  std::size_t, typename EdgeContainer::size_type
        >::type edges_size_type;

        typedef typename EdgeContainer::iterator EdgeIter;

        typedef typename boost::ct_if_t<on_edge_storage,
          stored_edge_property<vertex_descriptor, EdgeProperty>,
          stored_edge_iter<vertex_descriptor, EdgeIter, EdgeProperty>
        >::type StoredEdge;

        // Adjacency Types

        typedef typename container_gen<EdgeListS, StoredEdge>::type 
          OutEdgeList;
        typedef typename OutEdgeList::size_type degree_size_type;
        typedef typename OutEdgeList::iterator OutEdgeIter;

        typedef boost::detail::iterator_traits<OutEdgeIter> OutEdgeIterTraits;
        typedef typename OutEdgeIterTraits::iterator_category OutEdgeIterCat;
        typedef typename OutEdgeIterTraits::difference_type OutEdgeIterDiff;

        typedef iterator_adaptor<OutEdgeIter, 
          out_edge_iter_policies<vertex_descriptor>,
          edge_descriptor, edge_descriptor, edge_descriptor*, 
          boost::multi_pass_input_iterator_tag,
          OutEdgeIterDiff
        > out_edge_iterator;

        typedef typename adjacency_iterator_generator<graph_type,
           vertex_descriptor, out_edge_iterator>::type adjacency_iterator;

        typedef OutEdgeList InEdgeList;
        typedef OutEdgeIter InEdgeIter;
        typedef OutEdgeIterCat InEdgeIterCat;
        typedef OutEdgeIterDiff InEdgeIterDiff;

        typedef boost::iterator_adaptor<InEdgeIter, 
          in_edge_iter_policies<vertex_descriptor>,
          edge_descriptor, edge_descriptor, edge_descriptor*, 
          boost::multi_pass_input_iterator_tag,
          InEdgeIterDiff
        > in_edge_iterator;

        typedef typename inv_adjacency_iterator_generator<graph_type,
           vertex_descriptor, in_edge_iterator>::type inv_adjacency_iterator;

        // Edge Iterator

        typedef boost::detail::iterator_traits<EdgeIter> EdgeIterTraits;
        typedef typename EdgeIterTraits::iterator_category EdgeIterCat;
        typedef typename EdgeIterTraits::difference_type EdgeIterDiff;

        typedef boost::iterator_adaptor<EdgeIter,
          undirected_edge_iter_policies,
          edge_descriptor, edge_descriptor, edge_descriptor*, 
          boost::multi_pass_input_iterator_tag,
          EdgeIterDiff          
        > UndirectedEdgeIter; // also used for bidirectional

        typedef adj_list_edge_iterator<vertex_iterator, out_edge_iterator, 
           graph_type> DirectedEdgeIter;

        typedef typename boost::ct_if_t<on_edge_storage,
          DirectedEdgeIter, UndirectedEdgeIter>::type edge_iterator;

        // stored_vertex and StoredVertexList
        typedef typename container_gen<VertexListS, vertex_ptr>::type
          SeqStoredVertexList;
        struct seq_stored_vertex {
          seq_stored_vertex() { }
          seq_stored_vertex(const VertexProperty& p) : m_property(p) { }
          OutEdgeList m_out_edges;
          VertexProperty m_property;
          typename SeqStoredVertexList::iterator m_position;
        };
        struct bidir_seq_stored_vertex {
          bidir_seq_stored_vertex() { }
          bidir_seq_stored_vertex(const VertexProperty& p) : m_property(p) { }
          OutEdgeList m_out_edges;
          InEdgeList m_in_edges;
          VertexProperty m_property;
          typename SeqStoredVertexList::iterator m_position;
        };
        struct rand_stored_vertex {
          rand_stored_vertex() { }
          rand_stored_vertex(const VertexProperty& p) : m_property(p) { }
          OutEdgeList m_out_edges;
          VertexProperty m_property;
        };
        struct bidir_rand_stored_vertex {
          bidir_rand_stored_vertex() { }
          bidir_rand_stored_vertex(const VertexProperty& p) : m_property(p) { }
          OutEdgeList m_out_edges;
          InEdgeList m_in_edges;
          VertexProperty m_property;
        };
        typedef typename boost::ct_if_t<is_rand_access,
          typename boost::ct_if_t<BidirectionalT,
            bidir_rand_stored_vertex, rand_stored_vertex>::type,
          typename boost::ct_if_t<BidirectionalT,
            bidir_seq_stored_vertex, seq_stored_vertex>::type
        >::type StoredVertex;
        struct stored_vertex : public StoredVertex {
          stored_vertex() { }
          stored_vertex(const VertexProperty& p) : StoredVertex(p) { }
        };

        typedef typename container_gen<VertexListS, stored_vertex>::type
          RandStoredVertexList;
        typedef typename boost::ct_if_t< is_rand_access,
          RandStoredVertexList, SeqStoredVertexList>::type StoredVertexList;
      }; // end of config


      typedef typename boost::ct_if_t<BidirectionalT,
        bidirectional_graph_helper_with_property<config>,
        typename boost::ct_if_t<DirectedT,
          directed_graph_helper<config>,
          undirected_graph_helper<config>
        >::type
      >::type DirectedHelper;

      typedef typename boost::ct_if_t<is_rand_access,
        vec_adj_list_impl<Graph, config, DirectedHelper>,
        adj_list_impl<Graph, config, DirectedHelper>
      >::type type;

    };

  } // namespace detail

    //=========================================================================
    // Vertex Property Maps

    template <class Graph, class Property, class Tag>
    struct adj_list_vertex_property_map
      : public boost::put_get_at_helper<
          typename property_value<Property,Tag>::type,
          adj_list_vertex_property_map<Graph, Property, Tag>
        >
    {
      typedef typename Graph::stored_vertex StoredVertex;
      typedef typename property_value<Property, Tag>::type value_type;
      typedef typename Graph::vertex_descriptor key_type;
      typedef boost::lvalue_property_map_tag category;
      inline adj_list_vertex_property_map(Graph&) { }
      inline value_type& operator[](key_type v) {
        StoredVertex* sv = (StoredVertex*)v;
        return get_property_value(sv->m_property, value_type(), Tag());
      }
      inline const value_type& operator[](key_type v) const {
        StoredVertex* sv = (StoredVertex*)v;
        return get_property_value(sv->m_property, value_type(), Tag());
      }
    };

    template <class Graph, class Property>
    struct adj_list_vertex_all_properties_map
      : public boost::put_get_at_helper<Property,
          adj_list_vertex_all_properties_map<Graph, Property>
        >
    {
      typedef typename Graph::stored_vertex StoredVertex;
      typedef Property value_type;
      typedef typename Graph::vertex_descriptor key_type;
      typedef boost::lvalue_property_map_tag category;
      inline adj_list_vertex_all_properties_map(Graph&) { }
      inline value_type& operator[](key_type v) {
        StoredVertex* sv = (StoredVertex*)v;
        return sv->m_property;
      }
      inline const value_type& operator[](key_type v) const {
        StoredVertex* sv = (StoredVertex*)v;
        return sv->m_property;
      }
    };

    template <class Graph, class GraphRef, class Property, class Tag>
    struct vec_adj_list_vertex_property_map
      : public boost::put_get_at_helper<
          typename property_value<Property,Tag>::type,
          vec_adj_list_vertex_property_map<Graph,GraphRef,Property,Tag>
        >
    {
      typedef typename property_value<Property,Tag>::type value_type;
      typedef typename boost::graph_traits<Graph>::vertex_descriptor key_type;
      typedef boost::lvalue_property_map_tag category;
      vec_adj_list_vertex_property_map(GraphRef g) : m_g(g) { }
      inline value_type& operator[](key_type v) {
        return get_property_value(m_g.m_vertices[v].m_property, 
                                value_type(), Tag());
      }
      inline const value_type& operator[](key_type v) const {
        return get_property_value(m_g.m_vertices[v].m_property, 
                                value_type(), Tag());
      }
      GraphRef m_g;
    };

    template <class Graph, class GraphRef, class Property>
    struct vec_adj_list_vertex_all_properties_map
      : public boost::put_get_at_helper<Property,
          vec_adj_list_vertex_all_properties_map<Graph,GraphRef,Property>
        >
    {
      typedef Property value_type;
      typedef typename boost::graph_traits<Graph>::vertex_descriptor key_type;
      typedef boost::lvalue_property_map_tag category;
      vec_adj_list_vertex_all_properties_map(GraphRef g) : m_g(g) { }
      inline value_type& operator[](key_type v) {
        return m_g.m_vertices[v].m_property;
      }
      inline const value_type& operator[](key_type v) const {
        return m_g.m_vertices[v].m_property;
      }
      GraphRef m_g;
    };

    struct adj_list_any_vertex_pa {
      template <class Tag, class Graph, class Property>
      struct bind {
        typedef adj_list_vertex_property_map
          <Graph, Property, Tag> type;
        typedef adj_list_vertex_property_map
          <Graph, Property, Tag> const_type;
      };
    };
    struct adj_list_all_vertex_pa {
      template <class Tag, class Graph, class Property>
      struct bind {
        typedef typename Graph::vertex_descriptor Vertex;
        typedef adj_list_vertex_all_properties_map<Graph,Property> type;
        typedef adj_list_vertex_all_properties_map<Graph,Property> const_type;
      };
    };

    template <class Property, class Vertex>
    struct vec_adj_list_vertex_id_map
      : public boost::put_get_at_helper<
          Vertex, vec_adj_list_vertex_id_map<Property, Vertex>
        >
    {
      typedef Vertex value_type;
      typedef Vertex key_type;
      typedef boost::read_write_property_map_tag category;
      template <class Graph>
      inline vec_adj_list_vertex_id_map(const Graph&) { }
      inline value_type operator[](key_type v) const { return v; }
    };

    struct vec_adj_list_any_vertex_pa {
      template <class Tag, class Graph, class Property>
      struct bind {
        typedef vec_adj_list_vertex_property_map
          <Graph, Graph&, Property, Tag> type;
        typedef vec_adj_list_vertex_property_map
          <Graph, const Graph&, Property, Tag> const_type;
      };
    };
    struct vec_adj_list_id_vertex_pa {
      template <class Tag, class Graph, class Property>
      struct bind {
        typedef typename Graph::vertex_descriptor Vertex;
        typedef vec_adj_list_vertex_id_map<Property, Vertex> type;
        typedef vec_adj_list_vertex_id_map<Property, Vertex> const_type;
      };
    };
    struct vec_adj_list_all_vertex_pa {
      template <class Tag, class Graph, class Property>
      struct bind {
        typedef typename Graph::vertex_descriptor Vertex;
        typedef vec_adj_list_vertex_all_properties_map
	  <Graph, Graph&, Property> type;
        typedef vec_adj_list_vertex_all_properties_map
	  <Graph, const Graph&, Property> const_type;
      };
    };
  namespace detail {
    template <class Tag>
    struct adj_list_choose_vertex_pa_helper {
      typedef adj_list_any_vertex_pa type;
    };
    template <>
    struct adj_list_choose_vertex_pa_helper<vertex_all_t> {
      typedef adj_list_all_vertex_pa type;
    };
    template <class Tag, class Graph, class Property>
    struct adj_list_choose_vertex_pa {
      typedef typename adj_list_choose_vertex_pa_helper<Tag>::type Helper;
      typedef typename Helper::template bind<Tag,Graph,Property> Bind;
      typedef typename Bind::type type;
      typedef typename Bind::const_type const_type;
    };


    template <class Tag>
    struct vec_adj_list_choose_vertex_pa_helper {
      typedef vec_adj_list_any_vertex_pa type;
    };
    template <>
    struct vec_adj_list_choose_vertex_pa_helper<vertex_index_t> {
      typedef vec_adj_list_id_vertex_pa type;
    };
    template <>
    struct vec_adj_list_choose_vertex_pa_helper<vertex_all_t> {
      typedef vec_adj_list_all_vertex_pa type;
    };
    template <class Tag, class Graph, class Property>
    struct vec_adj_list_choose_vertex_pa {
      typedef typename vec_adj_list_choose_vertex_pa_helper<Tag>::type Helper;
      typedef typename Helper::template bind<Tag,Graph,Property> Bind;
      typedef typename Bind::type type;
      typedef typename Bind::const_type const_type;
    };
  } // namespace detail
    
    //=========================================================================
    // Edge Property Map

    template <class Directed, class Property, class Vertex, class Tag>
    struct adj_list_edge_property_map
      : public put_get_at_helper< 
          typename property_value<Property,Tag>::type,
          adj_list_edge_property_map<Directed, Property, Vertex, Tag>
        >
    {
      typedef typename property_value<Property,Tag>::type value_type;
      typedef detail::edge_desc_impl<Directed, Vertex> key_type;
      typedef boost::lvalue_property_map_tag category;
      inline value_type& operator[](key_type e) {
        Property& p = *(Property*)e.get_property();
        return get_property_value(p, value_type(), Tag());
      }
      inline const value_type& operator[](key_type e) const {
        const Property& p = *(const Property*)e.get_property();
        return get_property_value(p, value_type(), Tag());
      }
    };

    template <class Directed, class Property, class Vertex>
    struct adj_list_edge_all_properties_map
      : public put_get_at_helper<Property,
          adj_list_edge_all_properties_map<Directed, Property, Vertex>
        >
    {
      typedef Property value_type;
      typedef detail::edge_desc_impl<Directed, Vertex> key_type;
      typedef boost::lvalue_property_map_tag category;
      inline value_type& operator[](key_type e) {
        return *(Property*)e.get_property();
      }
      inline const value_type& operator[](key_type e) const {
        return *(const Property*)e.get_property();
      }
    };

  // Edge Property Maps

  namespace detail {
    struct adj_list_any_edge_pmap {
      template <class Graph, class Property, class Tag>
      struct bind {
	typedef adj_list_edge_property_map
	   <typename Graph::directed_category, Property, 
	    typename Graph::vertex_descriptor,Tag> type;
	typedef type const_type;
      };
    };
    struct adj_list_all_edge_pmap {
      template <class Graph, class Property, class Tag>
      struct bind {
	typedef adj_list_edge_all_properties_map
	   <typename Graph::directed_category, Property, 
	    typename Graph::vertex_descriptor> type;
	typedef type const_type;
      };
    };

    template <class Tag>
    struct adj_list_choose_edge_pmap_helper {
      typedef adj_list_any_edge_pmap type;
    };
    template <>
    struct adj_list_choose_edge_pmap_helper<edge_all_t> {
      typedef adj_list_all_edge_pmap type;
    };
    template <class Tag, class Graph, class Property>
    struct adj_list_choose_edge_pmap {
      typedef typename adj_list_choose_edge_pmap_helper<Tag>::type Helper;
      typedef typename Helper::template bind<Graph,Property,Tag> Bind;
      typedef typename Bind::type type;
      typedef typename Bind::const_type const_type;
    };
    struct adj_list_edge_property_selector {
      template <class Graph, class Property, class Tag>
      struct bind {
	typedef adj_list_choose_edge_pmap<Tag,Graph,Property> Choice;
	typedef typename Choice::type type;
	typedef typename Choice::const_type const_type;
      };
    };
  } // namespace detail

  template <>  
  struct edge_property_selector<adj_list_tag> {
    typedef detail::adj_list_edge_property_selector type;
  };
  template <>  
  struct edge_property_selector<vec_adj_list_tag> {
    typedef detail::adj_list_edge_property_selector type;
  };

  // Vertex Property Maps

  struct adj_list_vertex_property_selector {
    template <class Graph, class Property, class Tag>
    struct bind {
      typedef detail::adj_list_choose_vertex_pa<Tag,Graph,Property> Choice;
      typedef typename Choice::type type;
      typedef typename Choice::const_type const_type;
    };
  };
  template <>  
  struct vertex_property_selector<adj_list_tag> {
    typedef adj_list_vertex_property_selector type;
  };

  struct vec_adj_list_vertex_property_selector {
    template <class Graph, class Property, class Tag>
    struct bind {
      typedef detail::vec_adj_list_choose_vertex_pa<Tag,Graph,Property> Choice;
      typedef typename Choice::type type;
      typedef typename Choice::const_type const_type;
    };
  };
  template <>  
  struct vertex_property_selector<vec_adj_list_tag> {
    typedef vec_adj_list_vertex_property_selector type;
  };

} // namespace boost

#undef stored_edge
#undef stored_edge_property
#undef stored_edge_iter
#undef out_edge_iter_policies
#undef in_edge_iter_policies

#endif // BOOST_GRAPH_DETAIL_DETAIL_ADJACENCY_LIST_CCT

/*
  Implementation Notes:
  
  Many of the public interface functions in this file would have been
  more conveniently implemented as inline friend functions.
  However there are a few compiler bugs that make that approach
  non-portable.
 
  1. g++ inline friend in namespace bug
  2. g++ using clause doesn't work with inline friends
  3. VC++ doesn't have Koenig lookup

  For these reasons, the functions were all written as non-inline free 
  functions, and static cast was used to convert from the helper
  class to the adjacency_list derived class.

  Looking back, it might have been better to write out all functions
  in terms of the adjacency_list, and then use a tag to dispatch
  to the various helpers instead of using inheritance.

 */
