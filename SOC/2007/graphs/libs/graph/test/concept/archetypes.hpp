// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_GRAPH_CONCEPT_CHECK_ARCHETYPES_HPP
#define BOOST_GRAPH_CONCEPT_CHECK_ARCHETYPES_HPP

#include <utility>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graph_archetypes.hpp>

namespace boost
{
    struct descriptor_archetype
        : public
            default_constructible_archetype<
                copy_constructible_archetype<
                    sgi_assignable_archetype<
                        equality_comparable_archetype<> > > >
    {
        descriptor_archetype() { }
        descriptor_archetype(detail::dummy_constructor) { }
    };

    // Apparently, the Boost.Graph library doesn't have a bidirectional
    // archetype - kind of a strange oversight. This is actually derived
    // from incidence graph archetype in order have access to its
    // archetypical functions and members
    template <typename Vertex,
              typename Directed,
              typename Parallel,
              typename Base = detail::null_graph_archetype >
    struct bidirectional_graph_archetype : public Base
    {
        struct traversal_category
            : public bidirectional_graph_tag
            , public Base::traversal_category
        { };

        typedef descriptor_archetype vertex_descriptor;
        typedef descriptor_archetype edge_descriptor;

        typedef unsigned int degree_size_type;
        typedef unsigned int vertices_size_type;
        typedef unsigned int edges_size_type;

        typedef input_iterator_archetype<edge_descriptor> out_edge_iterator;
        typedef input_iterator_archetype<edge_descriptor> in_edge_iterator;

        typedef Directed directed_category;
        typedef Parallel edge_parallel_category;

        typedef void adjacency_iterator;
        typedef void vertex_iterator;
        typedef void edge_iterator;
    };

    /*
    template <typename V, typename D, typename P, typename B>
    typename bidirectional_graph_archetype<V,D,P,B>::vertex_descriptor
    source(typename bidirectional_graph_archetype<V,D,P,B>::edge_descriptor,
           const bidirectional_graph_archetype<V,D,P,B>&)
    {
        return V(static_object<detail::dummy_constructor>::get());
    }

    template <typename V, typename D, typename P, typename B>
    typename bidirectional_graph_archetype<V,D,P,B>::vertex_descriptor
    target(typename bidirectional_graph_archetype<V,D,P,B>::edge_descriptor,
           const bidirectional_graph_archetype<V,D,P,B>&)
    {
        return V(static_object<detail::dummy_constructor>::get());
    }

    template <typename V, typename D, typename P, typename B>
    std::pair<typename bidirectional_graph_archetype<V,D,P,B>::out_edge_iterator,
    typename bidirectional_graph_archetype<V,D,P,B>::out_edge_iterator>
    out_edges(typename bidirectional_graph_archetype<V,D,P,B>::vertex_descriptor,
              const bidirectional_graph_archetype<V,D,P,B>&)
    {
        typedef typename bidirectional_graph_archetype<V,D,P,B>::out_edge_iterator Iter;
        return std::make_pair(Iter(), Iter());
    }

    template <typename V, typename D, typename P, typename B>
    typename bidirectional_graph_archetype<V,D,P,B>::degree_size_type
    out_degree(typename bidirectional_graph_archetype<V,D,P,B>::vertex_descriptor,
               const bidirectional_graph_archetype<V,D,P,B>&)
    { return 0; }
    */

    template <typename V, typename D, typename P, typename B>
    std::pair<typename bidirectional_graph_archetype<V,D,P,B>::in_edge_iterator,
    typename bidirectional_graph_archetype<V,D,P,B>::in_edge_iterator>
    in_edges(typename bidirectional_graph_archetype<V,D,P,B>::vertex_descriptor,
             const bidirectional_graph_archetype<V,D,P,B>&)
    {
        typedef typename incidence_graph_archetype<V,D,P,B>::in_edge_iterator Iter;
        return std::make_pair(Iter(), Iter());
    }

    template <typename V, typename D, typename P, typename B>
    typename bidirectional_graph_archetype<V,D,P,B>::degree_size_type
    in_degree(typename bidirectional_graph_archetype<V,D,P,B>::vertex_desriptior,
              const bidirectional_graph_archetype<V,D,P,B>&)
    {
        return 0;
    }
}

#endif
