// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <vector>

#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/constant_property_map.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/visitors.hpp>

using namespace std;
using namespace boost;

// I think I see how this is done. Basically, there's an initial search
// for a baseline path from every vertex to v. A second search can be
// used to build heaps that record the off-path distance between the source
// the off-path vertex and the target. Or something like that. I'm not sure.

typedef property<edge_index_t, unsigned> EdgeIndex;

struct path_visitor
{
};

template <typename V, typename G>
struct vertex_printer
{
    vertex_printer(V v, const G& g) : vert(v), graph(g) { }
    V vert; const G& graph;
};
template <typename V, typename G>
inline vertex_printer<V,G> print_vertex(V v, const G& g)
{ return vertex_printer<V,G>(v, g); }
template <typename V, typename G>
inline ostream& operator << (ostream& os, const vertex_printer<V, G>& vp)
{
    os << get(vertex_index, vp.graph, vp.vert);
    return os;
}

template <typename E, typename G>
struct edge_printer
{
    edge_printer(E e, const G& g) : edge(e), graph(g) { }
    E edge; const G& graph;
};
template <typename E, typename G>
inline edge_printer<E,G> print_edge(E e, const G& g)
{ return edge_printer<E,G>(e, g); }
template <typename E, typename G>
inline ostream& operator <<(ostream& os, const edge_printer<E,G>& ep)
{
    os << "(" << print_vertex(source(ep.edge, ep.graph), ep.graph) << ","
       << print_vertex(target(ep.edge, ep.graph), ep.graph) << ")";
    return os;
}

template <typename P, typename G>
struct path_printer
{
    path_printer(const P& p, const G& g) : path(p), graph(g) { }
    P path; const G& graph;
};
template <typename P, typename G>
inline path_printer<P,G> print_path(const P& p, const G& g)
{ return path_printer<P,G>(p, g); }
template <typename P, typename G>
inline ostream& operator <<(ostream& os, const path_printer<P,G>& pp)
{
    typename P::const_iterator i, end = pp.path.end();
    for(i = pp.path.begin(); i != end; ++i) {
        os << print_edge(*i, pp.graph) << " ";
    }
    return os;
}



template <typename Graph>
void build_graph(Graph& g)
{
    typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
    typedef typename graph_traits<Graph>::edge_descriptor Edge;

    static const unsigned N = 5;
    vector<Vertex> v(N);
    vector<Edge> e;

    // add some vertices
    for(size_t i = 0; i < N; ++i) {
        // v[i] = add_vertex(g);
        v[i] = add_vertex(g);
    }

    // add some edges (with weights)
    e.push_back(add_edge(v[0], v[1], g).first);
    e.push_back(add_edge(v[1], v[2], g).first);
    e.push_back(add_edge(v[2], v[0], g).first);
    e.push_back(add_edge(v[3], v[4], g).first);
    e.push_back(add_edge(v[4], v[0], g).first);

    // allocate indices for the vertices
    for(size_t i = 0; i < N; ++i) {
        put(edge_index, g, e[i], i);
    }
};

// The tail(e,g) of a directed edge is its source vertex (the tail
// of the arrow).
template <typename Graph>
inline typename graph_traits<Graph>::vertex_descriptor
tail(typename graph_traits<Graph>::edge_descriptor e, const Graph& g)
{ return source(e, g); }

// The head(e,g) of a directed edge is its target vertex (the head
// of the arrow).
template <typename Graph>
inline typename graph_traits<Graph>::vertex_descriptor
head(typename graph_traits<Graph>::edge_descriptor e, const Graph& g)
{ return target(e, g); }

// Given a vertex v (in graph g), this returns the next vertex reached
// after v on the path from v to /some other vertex/ in the shortest
// path tree, T. Note that the other vertex is implicit in the parent
// edge map - it's the root.
template <typename Graph, typename ParentEdgeMap>
inline typename graph_traits<Graph>::vertex_descriptor
next_in_path(typename graph_traits<Graph>::vertex_descriptor v,
             const Graph& g,
             ParentEdgeMap pm)
{
    return source(pm[v], g);
}


template <typename Graph,
          typename DistanceMap,
          typename WeightMap>
typename property_traits<WeightMap>::value_type
lost_distance(const Graph& g,
              typename graph_traits<Graph>::edge_descriptor e,
              DistanceMap dm,
              WeightMap wm)
{
    typedef typename property_traits<WeightMap>::value_type Value;
    Value dhead = dm[head(e, g)];
    Value dtail = dm[tail(e, g)];
    return wm[e] + dhead - dtail;
}


template <typename Graph,
          typename DistanceMap,
          typename ParentEdgeMap>
void
shortest_paths_tree(const Graph& g,
                   typename graph_traits<Graph>::vertex_descriptor v,
                   DistanceMap dm,
                   ParentEdgeMap pm)
{
    // This function needs to be genericized to work with arbitrary shortest
    // paths algorithms.
    breadth_first_search(g, v,
        visitor(make_bfs_visitor(make_pair(
                record_distances(dm, on_tree_edge()),
                record_edge_predecessors(pm, on_tree_edge()))))
        );
}

template <typename Graph,
          typename ParentEdgeMap,
          typename Path>
void
extract_path(const Graph& g,
             typename graph_traits<Graph>::vertex_descriptor src,
             typename graph_traits<Graph>::vertex_descriptor dst,
             ParentEdgeMap pm,
             Path& path)
{
    typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
    typedef typename graph_traits<Graph>::edge_descriptor Edge;

    // This function walks the sp-tree from src, an arbitrary vertex
    // in g to dst - the initial target of a shortest path search.
    while(src != dst) {
        Edge e = pm[src];
        path.push_front(e); // guarntee a correct ordering in the path
        src = next_in_path(src, g, pm);
    }
}


template <typename Graph,
          typename Vertex,
          typename EdgeIndexMap,
          typename WeightMap>
void
eppstein_k_shortest_paths(const Graph& g, Vertex src, Vertex dst, size_t k,
                          EdgeIndexMap eim, WeightMap wm)
{
    typedef typename graph_traits<Graph>::edge_descriptor Edge;
    typedef typename graph_traits<Graph>::edge_iterator EdgeIterator;
    typedef list<Edge> Path;
    typedef list<Path> PathList;

    typedef exterior_vertex_property<Graph, size_t> DistanceProperty;
    typedef typename DistanceProperty::container_type DistanceVector;
    typedef typename DistanceProperty::map_type DistanceMap;

    typedef exterior_vertex_property<Graph, Edge> ParentEdgeProperty;
    typedef typename ParentEdgeProperty::container_type ParentEdgeVector;
    typedef typename ParentEdgeProperty::map_type ParentEdgeMap;

    typedef exterior_edge_property<Graph, int> EdgeLossProperty;
    typedef typename EdgeLossProperty::container_type EdgeLossContainer;
    typedef typename EdgeLossProperty::map_type EdgeLossMap;

    PathList paths;

    // make sure that these vertices are (at least) connected to
    // something else - and that they aren't the same vertex.
    if((out_degree(src, g) == 0) ||
       (out_degree(dst, g) == 0) ||
       (src == dst))
    {
        return;
    }

    // This is kind of dirty, but the computation proceeds a little
    // bit "backwardsly" if we leave src and dst the way they are.
    // Swapping them makes the output a little more coherent given
    // the input.
    std::swap(src, dst);

    // start by computing the shortest paths from t. this forms our
    // "root" shortest path. Everything else is a "sidetrack".
    DistanceVector distances(num_vertices(g));
    DistanceMap dm(distances, g);
    ParentEdgeVector parents(num_vertices(g));
    ParentEdgeMap pm(parents, g);

    // this is the big "preprocessing" stage. We establish a baseline
    // of all shortest paths to the destination (target)
    shortest_paths_tree(g, dst, dm ,pm);

    // compute the sidetracks for the resultant "tree". the lost vector
    // stores the lost distance for each edge
    EdgeLossContainer loss(num_edges(g));
    EdgeLossMap elm(loss, g);
    EdgeIterator i, end;
    for(tie(i, end) = edges(g); i != end; ++i) {
        elm[*i] = lost_distance(g, *i, dm, wm);
    }

    // this becomes one of the shortest paths...
    typename PathList::iterator pi = paths.insert(paths.end(), Path());
    extract_path(g, src, dst, pm, *pi);
    std::cout << print_path(*pi, g) << "\n";
}


template <typename Graph>
void test()
{
    typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
    typedef typename graph_traits<Graph>::edge_descriptor Edge;
    typedef typename graph_traits<Graph>::vertex_iterator VertexIterator;

    typedef constant_property_map<Edge, unsigned> WeightMap;
    typedef typename property_map<Graph, edge_index_t>::type EdgeIndexMap;

    Graph g;
    build_graph(g);
    VertexIterator i, j;
    tie(i, j) = vertices(g);

    // See the labels for correct ordering of flow
    Vertex s = *i;              // Vertex s
    Vertex t = *prior(j, 2);    // Vertex t

    WeightMap wm(1);
    EdgeIndexMap eim(get(edge_index, g));
    eppstein_k_shortest_paths(g, s, t, 3, eim, wm);
}

int
main()
{
    typedef undirected_graph<no_property, EdgeIndex > Graph;

    test<Graph>();

    return 0;
}
