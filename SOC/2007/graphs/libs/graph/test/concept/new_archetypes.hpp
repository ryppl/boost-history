// (C) Copyright 2002 Indiana University.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// (C) Copyright 2007 Andrew Sutton
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_NEW_ARCHETYPES_HPP
#define BOOST_GRAPH_NEW_ARCHETYPES_HPP

#include <boost/property_map.hpp>
#include <boost/concept_archetype.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>

namespace boost
{
    // The regular_archetype is basically a collection of interfaces  that makes
    // the archetype operate like most built-in types. It is  default and copy
    // constructible, assignable, and equality and less-than comparable.
    template <typename Base = null_archetype<> >
    struct regular_archetype
        : public
            default_constructible_archetype<
                copy_constructible_archetype<
                    sgi_assignable_archetype<
                        equality_comparable_archetype<
                            less_than_comparable_archetype<Base> > > > >
    {
        regular_archetype() { }
        regular_archetype(detail::dummy_constructor) { }
    };

    namespace detail
    {
        struct null_graph_archetype : public null_archetype<>
        {
            // Graph concepts
            typedef void vertex_descriptor;
            typedef void edge_descriptor;
            typedef void directed_category;
            struct traversal_category { };

            // IncidenceGraph concepts
            typedef void degree_size_type;
            typedef void out_edge_iterator;

            // BidirectionalGraph concepts
            typedef void in_edge_iterator;

            // AdjacencyGraph concepts
            typedef void adjacency_iterator;

            // VertexListGraph concepts
            typedef void vertex_iterator;
            typedef void vertices_size_type;

            // EdgeListGraph concepts
            typedef void edge_iterator;
            typedef void edges_size_type;
        };
    }

    struct descriptor_archetype
        : public regular_archetype<>
    {
        descriptor_archetype() { }
        descriptor_archetype(detail::dummy_constructor) { }
    };

    //
    // Graph
    //
    template <typename Directed,
              typename Parallel,
              typename Base = detail::null_graph_archetype>
    struct graph_archetype
        : public Base
    {
        typedef descriptor_archetype vertex_descriptor;
        typedef descriptor_archetype edge_descriptor;
        typedef Directed directed_category;
        typedef Parallel edge_parallel_category;
        typedef typename Base::traversal_category traversal_category;

        static vertex_descriptor null_vertex()
        { return static_object<vertex_descriptor>::get(); }
    };

    //
    // IncidenceGraph
    //
    template <typename Directed,
              typename Parallel,
              typename Base = detail::null_graph_archetype>
    struct incidence_graph_archetype
        : public graph_archetype<Directed, Parallel, Base>
    {
        typedef graph_archetype<Directed,Parallel,Base> BaseType;
        typedef unsigned degree_size_type;
        typedef input_iterator_archetype<typename BaseType::edge_descriptor> out_edge_iterator;
        struct traversal_category : incidence_graph_tag, Base::traversal_category { };
    };

    template <typename D, typename P, typename B>
    typename incidence_graph_archetype<D,P,B>::vertex_descriptor
    source(typename incidence_graph_archetype<D,P,B>::edge_descriptor,
           const incidence_graph_archetype<D,P,B>&)
    {
        typedef typename incidence_graph_archetype<D,P,B>::vertex_descriptor Vertex;
        return static_object<Vertex>::get();
    }

    template <typename D, typename P, typename B>
    typename incidence_graph_archetype<D,P,B>::vertex_descriptor
    target(typename incidence_graph_archetype<D,P,B>::edge_descriptor,
           const incidence_graph_archetype<D,P,B>& )
    {
        typedef typename incidence_graph_archetype<D,P,B>::vertex_descriptor Vertex;
        return static_object<Vertex>::get();
    }

    template <typename D, typename P, typename B>
    std::pair<typename incidence_graph_archetype<D,P,B>::out_edge_iterator,
              typename incidence_graph_archetype<D,P,B>::out_edge_iterator>
    out_edges(typename incidence_graph_archetype<D,P,B>::vertex_descriptor,
              const incidence_graph_archetype<D,P,B>& )
    {
        typedef typename incidence_graph_archetype<D,P,B>::out_edge_iterator Iterator;
        return std::make_pair(Iterator(), Iterator());
    }

    template <typename D, typename P, typename B>
    typename incidence_graph_archetype<D,P,B>::degree_size_type
    out_degree(typename incidence_graph_archetype<D,P,B>::vertex_descriptor,
               const incidence_graph_archetype<D,P,B>&)
    { return 0; }

    //
    // BidirectionalGraph
    //
    template <typename Directed,
              typename Parallel,
              typename Base = detail::null_graph_archetype>
    struct bidirectional_graph_archetype
        : public incidence_graph_archetype<Directed, Parallel, Base>
    {
        typedef incidence_graph_archetype<Directed,Parallel,Base> BaseType;
        typedef input_iterator_archetype<typename BaseType::edge_descriptor> in_edge_iterator;
        struct traversal_category : bidirectional_graph_tag, Base::traversal_category { };
    };

    template <typename D, typename P, typename B>
    std::pair<typename bidirectional_graph_archetype<D,P,B>::in_edge_iterator,
              typename bidirectional_graph_archetype<D,P,B>::in_edge_iterator>
    in_edges(typename bidirectional_graph_archetype<D,P,B>::vertex_descriptor,
             const bidirectional_graph_archetype<D,P,B>& )
    {
        typedef typename bidirectional_graph_archetype<D,P,B>::in_edge_iterator Iterator;
        return std::make_pair(Iterator(), Iterator());
    }

    template <typename D, typename P, typename B>
    typename bidirectional_graph_archetype<D,P,B>::degree_size_type
    in_degree(typename bidirectional_graph_archetype<D,P,B>::vertex_descriptor,
              const bidirectional_graph_archetype<D,P,B>&)
    { return 0; }

    //
    // AdjacencyGraph
    //
    template <typename Directed,
              typename Parallel,
              typename Base = detail::null_graph_archetype>
    struct adjacency_graph_archetype
        : public graph_archetype<Directed, Parallel, Base>
    {
        typedef graph_archetype<Directed,Parallel,Base> BaseType;
        typedef input_iterator_archetype<typename BaseType::vertex_descriptor> adjacency_iterator;
        struct traversal_category : adjacency_graph_tag, Base::traversal_category { };
    };

    template <typename D, typename P, typename B>
    std::pair<typename adjacency_graph_archetype<D,P,B>::adjacency_iterator,
              typename adjacency_graph_archetype<D,P,B>::adjacency_iterator>
    adjacent_vertices(typename adjacency_graph_archetype<D,P,B>::vertex_descriptor,
                      const adjacency_graph_archetype<D,P,B>&)
    {
        typedef typename adjacency_graph_archetype<D,P,B>::adjacency_iterator Iterator;
        return std::make_pair(Iterator(), Iterator());
    }

    //
    // VertexListGraph
    //
    template <typename Directed,
              typename Parallel,
              typename Base = detail::null_graph_archetype >
    struct vertex_list_graph_archetype
        : public graph_archetype<Directed, Parallel, Base>
    {
        typedef graph_archetype<Directed,Parallel,Base> BaseType;
        typedef unsigned vertices_size_type;
        typedef input_iterator_archetype<typename BaseType::vertex_descriptor> vertex_iterator;
        struct traversal_category : public vertex_list_graph_tag, Base::traversal_category { };
    };

    template <typename D, typename P, typename B>
    std::pair<typename vertex_list_graph_archetype<D,P,B>::vertex_iterator,
              typename vertex_list_graph_archetype<D,P,B>::vertex_iterator>
    vertices(const vertex_list_graph_archetype<D,P,B>&)
    {
        typedef typename vertex_list_graph_archetype<D,P,B>::vertex_iterator Iterator;
        return std::make_pair(Iterator(), Iterator());
    }

    template <typename D, typename P, typename B>
    typename vertex_list_graph_archetype<D,P,B>::vertices_size_type
    num_vertices(const vertex_list_graph_archetype<D,P,B>& )
    { return 0; }

    //
    // EdgeListGraph
    //
    template <typename Directed,
              typename Parallel,
              typename Base = detail::null_graph_archetype >
    struct edge_list_graph_archetype
        : public graph_archetype<Directed, Parallel, Base>
    {
        typedef graph_archetype<Directed,Parallel,Base> BaseType;
        typedef unsigned edges_size_type;
        typedef input_iterator_archetype<typename BaseType::edge_descriptor> edge_iterator;
        struct traversal_category : public edge_list_graph_tag, Base::traversal_category { };
    };

    template <typename D, typename P, typename B>
    std::pair<typename edge_list_graph_archetype<D,P,B>::edge_iterator,
              typename edge_list_graph_archetype<D,P,B>::edge_iterator>
    edges(const edge_list_graph_archetype<D,P,B>&)
    {
        typedef typename edge_list_graph_archetype<D,P,B>::edge_iterator Iterator;
        return std::make_pair(Iterator(), Iterator());
    }

    template <typename D, typename P, typename B>
    typename edge_list_graph_archetype<D,P,B>::edges_size_type
    num_edges(const edge_list_graph_archetype<D,P,B>& )
    { return 0; }


    //
    // ProprertyGraph
    //
    // I don't know how this works for bundled properties. I might need to
    // build a separate archetype for building bundled properties - and it
    // might be really ugly.

    // This little tag type here is used to associate the binding of graphs
    // of this type to the appropriate property map types.
    struct property_graph_archetype_tag { };

    template <typename GraphArchetype, typename Property, typename ValueArch>
    struct property_graph_archetype : public GraphArchetype
    {
        typedef property_graph_archetype_tag graph_tag;
        typedef ValueArch vertex_property_type;
        typedef ValueArch edge_property_type;
    };

    struct choose_edge_property_map_archetype
    {
        template <typename Graph, typename Property, typename Tag>
        struct bind_ {
            typedef typename graph_traits<Graph>::edge_descriptor Edge;
            typedef mutable_lvalue_property_map_archetype<Edge, Property> type;
            typedef lvalue_property_map_archetype<Edge, Property> const_type;
        };
    };

    template <>
    struct edge_property_selector<property_graph_archetype_tag>
    {
        typedef choose_edge_property_map_archetype type;
    };

    struct choose_vertex_property_map_archetype
    {
        template <typename Graph, typename Property, typename Tag>
        struct bind_
        {
            typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
            typedef mutable_lvalue_property_map_archetype<Vertex, Property> type;
            typedef lvalue_property_map_archetype<Vertex, Property> const_type;
        };
    };

    template <>
    struct vertex_property_selector<property_graph_archetype_tag>
    {
        typedef choose_vertex_property_map_archetype type;
    };

    template <typename G, typename P, typename V>
    typename property_map<property_graph_archetype<G,P,V>, P>::type
    get(P, property_graph_archetype<G,P,V>&)
    {
        typename property_map<property_graph_archetype<G,P,V>, P>::type pmap;
        return pmap;
    }

    template <typename G, typename P, typename V>
    typename property_map<property_graph_archetype<G,P,V>, P>::const_type
    get(P, const property_graph_archetype<G,P,V>&)
    {
        typename property_map<property_graph_archetype<G,P,V>, P>::const_type pmap;
        return pmap;
    }

    template <typename G, typename P, typename K, typename V>
    typename property_traits<
            typename property_map<property_graph_archetype<G,P,V>, P>::const_type
        >::value_type
    get(P p, const property_graph_archetype<G,P,V>& g, K k)
    {
        return get(get(p, g), k);
    }

    template <typename G, typename P, typename V, typename K>
    void
    put(P p, property_graph_archetype<G,P,V>& g, K k, const V& v)
    {
        typedef typename boost::property_map<property_graph_archetype<G,P,V>, P>::type Map;
        put(get(p, g), k, v);
    }
}


#endif // BOOST_GRAPH_ARCHETYPES_HPP
