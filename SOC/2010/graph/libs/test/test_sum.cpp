
#include <iostream>
#include <tr1/unordered_map>

#include "typestr.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/sum.hpp>


using namespace std;
using namespace boost;

// Return the value associated with k or the default value if not found.
template<typename Map, typename Key, typename Value>
Value const&
get_value(Map const& map, Key const& k, Value const& def)
{
  typename Map::const_iterator iter = map.find(k);
  if(iter == map.end()) {
    return def;
  } else {
    return iter->second;
  }
}

typedef adjacency_list_traits<vecS, vecS, bidirectionalS, listS>::vertex_descriptor Vertex;
typedef adjacency_list_traits<vecS, vecS, bidirectionalS, listS>::edge_descriptor Edge;

typedef tr1::unordered_map<int, Vertex> Vertex_Map;
typedef tr1::unordered_map<int, Edge> Edge_Map;

struct Vertex_Prop {
  int name;
};

struct Edge_Prop {
  int name;
};

struct Graph_Prop {
  Vertex_Map vertices;
  Edge_Map edges;
};

typedef adjacency_list <
  vecS, vecS, bidirectionalS, Vertex_Prop, Edge_Prop,  Graph_Prop, listS
> Graph;

// Info about vertex and edge name types.
namespace boost {
  template<> struct vertex_name_traits<Graph> { typedef int type; };
  template<> struct edge_name_traits<Graph> { typedef int type; };
} // namespace boost

// Default accessors for the vertex name
int const& get_vertex_name(Vertex v, Graph const& g)
{ return g[v].name; }

void put_vertex_name(Vertex v, int n, Graph& g)
{ g[v].name = n; }

// Default accessor named vertices.
Vertex get_named_vertex(int n, Graph const& g)
{ return get_value(g[graph_bundle].vertices, n, Graph::null_vertex()); }

void put_named_vertex(int n, Vertex v, Graph& g)
{ g[graph_bundle].vertices[n] = v; }

// Default accessors for edge name
int const& get_edge_name(Edge e, Graph const& g)
{ return g[e].name; }

void put_edge_name(Edge e, int n, Graph& g)
{ g[e].name = n; }

// Default accessors for edge naming.
Edge get_named_edge(int n, Graph const& g)
{ return get_value(g[graph_bundle].edges, n, Edge()); }

void put_named_edge(int n, Edge e, Graph& g)
{ g[graph_bundle].edges[n] = e; }

// NOTE: On accessor functions. It's POSSIBLE to write generic get_*, put_*
// accessors for old-style properties using enable_if. For example, we could
// write a generic get_vertex_name enabled on has_property<vertex_name_t>.
// For obvious reasons, we can't do this with bundled properties, but this
// is a perfectly viable alternative.

int main() {
  Graph g1, g2, g3;
  graph_sum(g1, g2, g3);
}
