// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

#include <boost/graph/properties.hpp>

template <typename V, typename G>
struct vertex_printer
{
    vertex_printer(V v, const G& g) : vert(v), graph(g) { }
    V vert;
    const G& graph;
};

template <typename V, typename G>
inline vertex_printer<V,G> print_vertex(V v, const G& g)
{ return vertex_printer<V,G>(v, g); }

template <typename V, typename G>
inline std::ostream& operator << (std::ostream& os, const vertex_printer<V, G>& vp)
{
    os << boost::get(boost::vertex_index, vp.graph, vp.vert);
    return os;
}

template <typename E, typename G>
struct edge_printer
{
    edge_printer(E e, const G& g) : edge(e), graph(g) { }
    E edge;
    const G& graph;
};

template <typename E, typename G>
inline edge_printer<E,G> print_edge(E e, const G& g)
{ return edge_printer<E,G>(e, g); }

template <typename E, typename G>
inline std::ostream& operator <<(std::ostream& os, const edge_printer<E,G>& ep)
{
    os << "(" << print_vertex(source(ep.edge, ep.graph), ep.graph) << ","
       << print_vertex(target(ep.edge, ep.graph), ep.graph) << ")";
    return os;
}

template <typename P, typename G>
struct path_printer
{
    path_printer(const P& p, const G& g) : path(p), graph(g) { }
    P path;
    const G& graph;
};

template <typename P, typename G>
inline path_printer<P,G> print_path(const P& p, const G& g)
{ return path_printer<P,G>(p, g); }

template <typename P, typename G>
inline std::ostream& operator <<(std::ostream& os, const path_printer<P,G>& pp)
{
    typename P::const_iterator i, end = pp.path.end();
    for(i = pp.path.begin(); i != end; ++i) {
        os << print_vertex(*i, pp.graph) << " ";
    }
    return os;
}

template <typename M, typename G>
struct vertex_map_printer
{
    vertex_map_printer(M m, const G& g) : map(m), graph(g) { }
    M map;
    const G& graph;
};

template <typename M, typename G>
inline vertex_map_printer<M,G>
print_vertex_map(const M& m, const G& g)
{ return vertex_map_printer<M,G>(m, g); }

template <typename M, typename G>
inline std::ostream&
operator <<(std::ostream& os, const vertex_map_printer<M,G>& mp)
{
    typename boost::graph_traits<G>::vertex_iterator i, end;
    os << "{\n";
    for(tie(i, end) = boost::vertices(mp.graph); i != end; ++i) {
        os << print_vertex(*i, mp.graph) << ": " <<  mp.map[*i] << "\n";
    }
    os << "}\n";
    return os;
}

template <typename M, typename G>
struct edge_map_printer
{
    edge_map_printer(M m, const G& g) : map(m), graph(g) { }
    M map;
    const G& graph;
};

template <typename M, typename G>
inline edge_map_printer<M,G>
print_edge_map(const M& m, const G& g)
{ return edge_map_printer<M,G>(m, g); }

template <typename M, typename G>
inline std::ostream&
operator <<(std::ostream& os, const edge_map_printer<M,G>& mp)
{
    typename boost::graph_traits<G>::edge_iterator i, end;
    os << "{\n";
    for(tie(i, end) = boost::edges(mp.graph); i != end; ++i) {
        os << print_edge(*i, mp.graph) << mp.map[*i] << "\n";
    }
    os << "}\n";
    return os;
}

template <typename M, typename G>
struct vertex_matrix_printer
{
    vertex_matrix_printer(const M& m, const G& g) : matrix(m), graph(g) { }
    const M& matrix;
    const G& graph;
};

template <typename M, typename G>
inline vertex_matrix_printer<M,G>
print_vertex_matrix(const M& m, const G& g)
{ return vertex_matrix_printer<M,G>(m, g); }

template <typename M, typename G>
inline std::ostream&
operator <<(std::ostream& os, const vertex_matrix_printer<M,G>& mp)
{
    typename boost::graph_traits<G>::vertex_iterator i, j, end;
    os << "{\n";
    for(tie(i, end) = boost::vertices(mp.graph); i != end; ++i) {
        os << "{ ";
        for(j = boost::vertices(mp.graph).first; j != end; ++j) {
            os << mp.matrix[*i][*j] << " ";
        }
        os << "}\n";
    }
    os << "}\n";
    return os;
}
