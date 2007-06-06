
#ifndef CLUSTERING_COEFFICIENT_HXX
#define CLUSTERING_COEFFICIENT_HXX

// std includes
#include <utility>
#include <algorithm>

/**
 * The num_centered_triples() method computes the number of connected triples
 * centered at the given vertex. This can also be described as the number
 * of length 2 paths through the given vertex. This value is actually easy
 * to compute as choose(k, 2) where k is the size of the vertices neighborhood.
 *
 * This could be written a little more correctly since the vertex type is
 * actually an associated type of the graph. I'm lazy and the parameter list
 * looked a little long.
 */
template <
    typename AdjacencyGraph,
    typename AdjacencyVertex
    >
size_t
num_centered_triples(AdjacencyVertex const& v, AdjacencyGraph const& g)
{
    using namespace std;
    using namespace boost;

    typedef AdjacencyGraph graph;
    typedef AdjacencyVertex vertex;
    typedef typename graph_traits<graph>::adjacency_iterator adjacency_iterator;

    // find all of the adjacent vertices
    adjacency_iterator i, j;
    tie(i, j) = adjacent_vertices(v, g);

    // compute the size of the neighborhood
    size_t k = distance(i, j);

    // use that to compute choose(k, 2)
    return (k * (k - 1)) / 2;
}

/**
 * The num_centered_trianges() method is used to compute the number of triangles
 * centered on this point - not really centered, but rather triangles in which
 * this point participates. This is actually closely related to the computation
 * of cenetered triples. Note that a triange at vertex v is formed if two neighbors
 * i and j share a common edge. Therefore, if we examine neighbors of i and find
 * j, we can increment the number of triangles. after examing each neigbor, we
 * should never need to consider it again (otherwise, we'll get too many triangles).
 *
 * The runtime of this function is proportional to the N^2 (neigborhood squared)
 * of vertex v. That is to say, that for each adjacent vertex i of v, we also
 * have to loook at adjacent vertices j of i.
 */
template <
    typename AdjacencyGraph,
    typename AdjacencyVertex,
    typename TriangleVector
    >
size_t
num_centered_triangles(AdjacencyVertex const& v,
		       TriangleVector &tris,
		       AdjacencyGraph const& g)
{
    using namespace std;
    using namespace boost;

    // graph types
    typedef AdjacencyGraph graph;
    typedef AdjacencyVertex vertex;
    typedef typename graph_traits<graph>::adjacency_iterator adjacency_iterator;

    // triangle types
    typedef typename TriangleVector::value_type triangle;

    // outer loop - iterate over all adjacent vertices of v
    size_t count = 0;
    adjacency_iterator i, j;
    for(tie(i, j) = adjacent_vertices(v, g); i != j; ++i) {
	adjacency_iterator k, l;
	for(tie(k, l) = adjacent_vertices(*i, g); k != l; ++k) {
	    // once again... we're going to look at the adjacent vertices
	    // of v. this time we start at the vertex after i and go to
	    // the end. this is all vertices m > i
	    for(adjacency_iterator m = next(i); m != j; ++m) {
		// if k (vertex in N^2) is the same as m (vertex in N)
		// thin this is a triangle {v,i,k}.
		if(*k == *m) {
		    tris.push_back(triangle(v, *i, *k));
		    ++count;
		}
	    }
	}
    }

    return count;
}


/**
 * Compute the clustering coefficient for the entire graph. This is
 * done by averaging, for each node, the proportion of triangles centered
 * at each node and the number of triples (lenght 2 paths through) each
 * node.
 *
 * Note that the triangle vector must store vertex triples because we're
 * actually going to store all the triangles formed by the graph.
 */
template <
    typename AdjacencyGraph,
    typename GraphTriangles,
    typename VertexTriangles,
    typename VertexTriples,
    typename VertexCluster
    >
double
clustering_coefficient(AdjacencyGraph &g,
		       GraphTriangles &tri_vec,
		       VertexTriangles &tri_count,
		       VertexTriples &trip_count,
		       VertexCluster &cluster_coef)
{
    using namespace std;
    using namespace boost;

    // graph types
    typedef AdjacencyGraph graph;
    typedef typename graph_traits<graph>::vertex_descriptor vertex;
    typedef typename graph_traits<graph>::vertex_iterator vertex_iterator;

    // iterate over each vertex and compute its clustering coefficient
    double acc = 0.0;
    size_t ix = 0;
    vertex_iterator i, j;
    for(tie(i, j) = vertices(g); i != j; ++i) {
	vertex const& v = *i;

	// first, compute the number of triples because its easy
	size_t tris = num_centered_triangles(v, tri_vec, g);
	size_t trips = num_centered_triples(v, g);

	double ci = 0.0;
	if(trips > 0) {
	  ci = (double)tris / (double)trips;
	}

	// remember these values
	tri_count[ix] = tris;
	trip_count[ix] = trips;
	cluster_coef[ix] = ci;

	// accumulate the sum of ci's.
	acc += ci;

	// increment the index!
	++ix;
    }

    return acc / (double)num_vertices(g);
}

#endif
