// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_CLIQUE_HXX
#define BOOST_GRAPH_CLIQUE_HXX

#include <limits>

namespace boost
{

    // The algorithm implemented in this paper is based on the so-called
    // Algorithm 457, published as:
    //
    //     @article{362367,
    //         author = {Coen Bron and Joep Kerbosch},
    //         title = {Algorithm 457: finding all cliques of an undirected graph},
    //         journal = {Communications of the ACM},
    //         volume = {16},
    //         number = {9},
    //         year = {1973},
    //         issn = {0001-0782},
    //         pages = {575--577},
    //         doi = {http://doi.acm.org/10.1145/362342.362367},
    //             publisher = {ACM Press},
    //             address = {New York, NY, USA},
    //         }
    //
    // Sort of. This implementation is adapted from the 1st version of the
    // algorithm and does not implement the candidate selection optimization
    // described as published - it could, it just doesn't yet.
    //
    // The algorithm is given as proportional to (3.14)^(n/3) power. This is
    // not the same as O(...), but based on time measures and approximation.
    //
    // Unfortunately, this implementation may be less efficient on non-
    // AdjacencyMatrix modeled graphs due to the non-constant implementation
    // of the edge(u,v,g) functions.
    //
    // TODO: It might be worthwhile to provide functionality for passing
    // a connectivity matrix to improve the efficiency of those lookups
    // when needed. This could simply be passed as a BooleanMatrix
    // s.t. edge(u,v,B) returns true or false. This could easily be
    // abstracted for adjacency matricies.
    //
    // The following paper is interesting for a number of reasons. First,
    // it lists a number of other such algorithms and second, it describes
    // a new algorithm (that does not appear to require the edge(u,v,g)
    // function and appears fairly efficient. It is probably worth investigating.
    //
    //      @article{DBLP:journals/tcs/TomitaTT06,
    //          author = {Etsuji Tomita and Akira Tanaka and Haruhisa Takahashi},
    //          title = {The worst-case time complexity for generating all maximal cliques and computational experiments},
    //          journal = {Theor. Comput. Sci.},
    //          volume = {363},
    //          number = {1},
    //          year = {2006},
    //          pages = {28-42}
    //          ee = {http://dx.doi.org/10.1016/j.tcs.2006.06.015}
    //  }

    struct clique_visitor
    {
        template <typename VertexSet, typename Graph>
        void clique(const VertexSet&, Graph&)
        { }
    };

    namespace detail
    {
        template <typename Graph>
        inline bool
        is_connected_to_clique(const Graph& g,
                               typename graph_traits<Graph>::vertex_descriptor u,
                               typename graph_traits<Graph>::vertex_descriptor v,
                               typename graph_traits<Graph>::undirected_category)
        {
            return edge(u, v, g).second;
        }

        template <typename Graph>
        inline bool
        is_connected_to_clique(const Graph& g,
                               typename graph_traits<Graph>::vertex_descriptor u,
                               typename graph_traits<Graph>::vertex_descriptor v,
                               typename graph_traits<Graph>::directed_category)
        {
            // Note that this could alternate between using an || to determine
            // full connectivity. I believe that this should produce strongly
            // connected components. Note that using && instead of || will
            // change the results to a fully connected subgraph (i.e., symmetric
            // edges between all vertices s.t., if a->b, then b->a.
            return edge(u, v, g).second && edge(v, u, g).second;
        }

        template <typename Graph, typename Container>
        inline void
        filter_unconnected_vertices(const Graph& g,
                                    typename graph_traits<Graph>::vertex_descriptor v,
                                    const Container& in,
                                    Container& out)
        {
            typename graph_traits<Graph>::directed_category cat;
            typename Container::const_iterator i, end = in.end();
            for(i = in.begin(); i != end; ++i) {
                if(is_connected_to_clique(g, v, *i, cat)) {
                    out.push_back(*i);
                }
            }
        }

        template <typename Graph,
                  typename Clique,        // compsub type
                  typename Container,     // candidates/not type
                  typename Visitor>
        void extend_clique(const Graph& g,
                           Clique& clique,
                           Container& cands,
                           Container& nots,
                           Visitor vis)
        {
            typedef typename graph_traits<Graph>::vertex_descriptor Vertex;

            {
                // is there vertex in nots that is connected to all vertices
                // in the candidate set? if so, no clique can ever be found.
                typename Container::iterator ni, nend = nots.end();
                typename Container::iterator ci, cend = cands.end();
                for(ni = nots.begin(); ni != nend; ++ni) {
                    for(ci = cands.begin(); ci != cend; ++ci) {
                        // if we don't find an edge, then we're okay.
                        if(!edge(*ni, *ci, g).second) break;
                    }
                    // if we iterated all the way to the end, then *ni
                    // is connected to all *ci
                    if(ci == cend) break;
                }
                // if we broke early, we found *ni connected to all *ci
                if(ni != nend) return;
            }

            // TODO: the original algorithm 457 describes an alternative
            // (albeit really complicated) mechanism for selecting candidates.
            // The given optimizaiton seeks to bring about the above
            // condition sooner (i.e., there is a vertex in the not set
            // that is connected to all candidates). unfortunately, the
            // method they give for doing this is fairly unclear.

            // basically, for every vertex in not, we should know how many
            // vertices it is disconnected from in the candidate set. if
            // we fix some vertex in the not set, then we want to keep
            // choosing vertices that are not connected to that fixed vertex.
            // apparently, by selecting fix point with the minimum number
            // of disconnections (i.e., the maximum number of connections
            // within the candidate set), then the previous condition wil
            // be reached sooner.

            // there's some other stuff about using the number of disconnects
            // as a counter, but i'm jot really sure i followed it.

            // otherwise, iterate over candidates and and test
            // for maxmimal cliquiness.
            typename Container::iterator i, j, end = cands.end();
            for(i = cands.begin(); i != cands.end(); ) {
                Vertex candidate = *i;

                // add the candidate to the clique (keeping the iterator!)
                typename Clique::iterator ci =
                        clique.insert(clique.end(), candidate);

                // remove it from the candidate set
                i = cands.erase(i);

                // build new candidate and not sets by removing all vertices
                // that are not connected to the current candidate vertex.
                // these actually invert the operation, adding them to the new
                // sets if the vertices are connected. its semantically the same.
                Container new_cands, new_nots;
                filter_unconnected_vertices(g, candidate, cands, new_cands);
                filter_unconnected_vertices(g, candidate, nots, new_nots);

                if(new_cands.empty() && new_nots.empty()) {
                    // our current clique is maximal since there's nothing
                    // that's connected that we haven't already visited
                    vis.clique(clique, g);
                }
                else {
                    // recurse to explore the new candidates
                    extend_clique(g, clique, new_cands, new_nots, vis);
                }

                // we're done with this vertex, so we need to move it
                // to the nots, and remove the candidate from the clique.
                nots.push_back(candidate);
                clique.erase(ci);
            }
        }
    }

    template <typename Graph, typename Visitor>
    inline void
    bron_kerbosch_all_cliques(const Graph& g, Visitor vis)
    {
        typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
        typedef typename graph_traits<Graph>::vertex_iterator VertexIterator;
        typedef std::vector<Vertex> VertexSet;
        typedef std::list<Vertex> Clique;

        VertexIterator i, end;
        tie(i, end) = vertices(g);

        VertexSet cands(i, end);    // start with all vertices as candidates
        VertexSet nots;             // start with no vertices visited
        Clique clique;              // the first clique is an empty vertex set
        detail::extend_clique(g, clique, cands, nots, vis);
    }
}

#endif
