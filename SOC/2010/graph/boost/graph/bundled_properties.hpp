//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef BOOST_GRAPH_BUNDLED_PROPERTIES_HPP
#define BOOST_GRAPH_BUNDLED_PROPERTIES_HPP

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_pod.hpp>
#include <boost/type_traits/is_const.hpp>

// This file contains utility code for integrating bundled and named properties.
// Each named property corresponds to a get/put pair of functions and an
// associtaed proerty map. The get/put functions for getting the property
// map as well as the property map support are all defined herein. There are
// three components to every property:
//  1. A declaration of the specific get/put interface.
//     get_prop(d, g) and put_prop(d, x, g)
//  2. A property map abstracting the get/put operations defined in 1.
//      get(pm, d) and put(pm, d, x)
//  3. A specialization of property_map and the property graph accessor.
//      property_map<G, P> and get(p, g)

// FIXME: Go through the exceedingly tedious process of ensuring that constant
// property maps do not support writeability. There's probably going to be
// a fair amount of metaprogramming involved. Currently, we just avoid the
// issue altogether by simply allowing all property maps to be writeable.

namespace boost {
  ///////// vertex_name /////////
  // Interface
  template<typename G>
  typename vertex_name_traits<G>::type
  get_vertex_name(typename graph_traits<G>::vertex_descriptor, G const&);

  template<typename G>
  void
  put_vertex_name(typename graph_traits<G>::vertex_descriptor,
                  typename vertex_name_traits<G>::type const&,
                  G&);

  // Property Map
  template<typename G>
  struct vertex_name_property_map
  {
    typedef typename graph_traits<G>::vertex_descriptor key_type;
    typedef typename vertex_name_traits<G>::type value_type;
    typedef value_type& reference;
    typedef read_write_property_map_tag category;

    vertex_name_property_map(G const& g)
      : graph(const_cast<G&>(g))
    { }

    G& graph;
  };

  template<typename G>
  inline typename vertex_name_property_map<G>::value_type const&
  get(vertex_name_property_map<G> const& pm,
      typename vertex_name_property_map<G>::key_type const& k)
  { return get_vertex_name(k, pm.graph); }

  template<typename G>
  inline void
  put(vertex_name_property_map<G>& pm,
      typename vertex_name_property_map<G>::key_type const& k,
      typename vertex_name_property_map<G>::value_type const& x)
  { put_vertex_name(k, x, pm.graph); }

  // Name binding
  template<typename G>
  struct property_map<G, vertex_name_t> {
    typedef vertex_name_property_map<G> type;
    typedef vertex_name_property_map<G const> const_type;
  };

  template<typename G>
  inline typename property_map<G, vertex_name_t>::type
  get(vertex_name_t, G& g)
  { return typename property_map<G, vertex_name_t>::type(g); }

  template<typename G>
  inline typename property_map<G, vertex_name_t>::const_type
  get(vertex_name_t, G const& g)
  { return typename property_map<G, vertex_name_t>::const_type(g); }

  ///////// edge_name /////////
  // Interface
  template<typename G>
  typename edge_name_traits<G>::type
  get_edge_name(typename graph_traits<G>::edge_descriptor, G const&);

  template<typename G>
  void
  put_edge_name(typename graph_traits<G>::edge_descriptor,
                typename edge_name_traits<G>::type const&,
                  G&);

  // Property Map
  template<typename G>
  struct edge_name_property_map
  {
    typedef typename graph_traits<G>::edge_descriptor key_type;
    typedef typename edge_name_traits<G>::type value_type;
    typedef value_type& reference;
    typedef read_write_property_map_tag category;

    edge_name_property_map(G const& g)
      : graph(const_cast<G&>(g))
    { }

    G& graph;
  };

  template<typename G>
  inline typename edge_name_property_map<G>::value_type const&
  get(edge_name_property_map<G> const& pm,
      typename edge_name_property_map<G>::key_type const& k)
  { return get_edge_name(k, pm.graph); }

  template<typename G>
  inline void
  put(edge_name_property_map<G>& pm,
      typename edge_name_property_map<G>::key_type const& k,
      typename edge_name_property_map<G>::value_type const& x)
  { put_edge_name(k, x, pm.graph); }

  // Name Binding
  template<typename G>
  struct property_map<G, edge_name_t> {
    typedef edge_name_property_map<G> type;
    typedef edge_name_property_map<G const> const_type;
  };

  template<typename G>
  inline typename property_map<G, edge_name_t>::type
  get(edge_name_t, G& g)
  { return typename property_map<G, edge_name_t>::type(g); }

  template<typename G>
  inline typename property_map<G, edge_name_t>::const_type
  get(edge_name_t, G const& g)
  { return typename property_map<G, edge_name_t>::const_type(g); }

  ///////// graph_named_vertices /////////
  // Interface
  template<typename G>
  typename graph_traits<G>::vertex_descriptor
  get_named_vertex(typename vertex_name_traits<G>::type const&, G const&);

  template<typename G>
  void
  put_named_vertex(typename vertex_name_traits<G>::type const& name,
                   typename graph_traits<G>::vertex_descriptor v,
                   G& g);

  // Property map
  template<typename G>
  struct named_vertex_property_map
  {
    typedef typename vertex_name_traits<G>::type key_type;
    typedef typename graph_traits<G>::vertex_descriptor value_type;
    typedef value_type reference;
    typedef read_write_property_map_tag category;

    named_vertex_property_map(G const& g)
      : graph(const_cast<G&>(g))
    { }

    G& graph;
  };

  template<typename G>
  typename named_vertex_property_map<G>::reference
  get(named_vertex_property_map<G> const& pm,
      typename named_vertex_property_map<G>::key_type const& k)
  { return get_named_vertex(k, pm.graph); }

  template<typename G>
  inline void
  put(named_vertex_property_map<G>& pm,
      typename named_vertex_property_map<G>::key_type const& k,
      typename named_vertex_property_map<G>::value_type const& x)
  { put_named_vertex(k, x, pm.graph); }

  // Binding
  template<typename G>
  struct property_map<G, graph_named_vertices_t>
  {
    typedef named_vertex_property_map<G> type;
    typedef named_vertex_property_map<G const> const_type;
  };

  template<typename G>
  inline typename property_map<G, graph_named_vertices_t>::type
  get(graph_named_vertices_t, G& g)
  { return typename property_map<G, graph_named_vertices_t>::type(g); }

  template<typename G>
  inline typename property_map<G, graph_named_vertices_t>::const_type
  get(graph_named_vertices_t, G const& g)
  { return typename property_map<G, graph_named_vertices_t>::const_type(g); }

  ///////// graph_named_vertices /////////
  // Interface
  template<typename G>
  typename graph_traits<G>::edge_descriptor
  get_named_edge(typename edge_name_traits<G>::type const&, G const&);

  template<typename G>
  void
  put_named_edge(typename edge_name_traits<G>::type const&,
                 typename graph_traits<G>::edge_descriptor e,
                 G&);

  // Property Map
  template<typename G>
  struct named_edge_property_map {
    typedef typename edge_name_traits<G>::type key_type;
    typedef typename graph_traits<G>::edge_descriptor value_type;
    typedef value_type reference;
    typedef read_write_property_map_tag category;

    named_edge_property_map(G& g)
      : graph(const_cast<G&>(g))
    { }

    G& graph;
  };

  template<typename G>
  typename named_edge_property_map<G>::reference
  get(named_edge_property_map<G> const& pm,
      typename named_edge_property_map<G>::key_type const& k)
  { return get_named_edge(k, pm.graph); }

  template<typename G>
  inline void
  put(named_edge_property_map<G>& pm,
      typename named_edge_property_map<G>::key_type const& k,
      typename named_edge_property_map<G>::value_type const& x)
  { put_named_edge(k, x, pm.graph); }

  // Binding
  template<typename G>
  struct property_map<G, graph_named_edges_t>
  {
    typedef named_edge_property_map<G> type;
    typedef named_edge_property_map<G const> const_type;
  };

  template<typename G>
  inline typename property_map<G, graph_named_edges_t>::type
  get(graph_named_edges_t, G& g)
  { return typename property_map<G, graph_named_edges_t>::type(g); }

  template<typename G>
  inline typename property_map<G, graph_named_edges_t>::const_type
  get(graph_named_edges_t, G const& g)
  { return typename property_map<G, graph_named_edges_t>::const_type(g); }
} // namespace boost

#endif