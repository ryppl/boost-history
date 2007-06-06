
#ifndef DIAMETER_HXX
#define DIAMETER_HXX

// std includes
#include <vector>
#include <limits>

// boost includes
#include <boost/graph/johnson_all_pairs_shortest.hpp>

/**
 * Compute the diameter of the graoh - which is essentially the longest
 * of all shortest paths (or the longest geodesic). At the moment, this
 * algorithm uses johnson's variant which runs in O(n*m*log n). Note that
 * when the graph is dense (i.e., m -> n^2), this takes O(n^3 * log n)
 * which is actually worse than floyd warshall. However, this should run
 * fine on power-law graphs since they're relatively sparse. Normally
 * distributed graphs might not be so lucky.
 *
 * There's some strange variations of this algorithm. For example,
 * if the graph is unconnected, then it really doesn't have an actual
 * diameter. If we igore infinite lenghts, then we are computing the
 * diameter of the largest connected component - which may actually
 * by acceptable.
 */
template <
    typename Graph,
    typename Matrix
    >
int
diameter(Graph &g, Matrix &d)
{
    using namespace std;
    using namespace boost;

    // various graph types
    typedef Graph graph;
    typedef typename graph_traits<graph>::vertex_descriptor vertex;

    // matrix types

    // for convenience, get the number of vertices
    size_t n = num_vertices(g);

    // find all pairs of shortest paths
    int ret = 0;
    bool success = johnson_all_pairs_shortest_paths(g, d);
    if(success) {
	// compute the maximum distance of elements in graph
	for(size_t i = 0; i < n; ++i) {
	    for(size_t j = 0; j < n; ++j) {
		int dist = d[i][j];

		// don't compute distances for disconnected
		// vertices - this is kind of a weird point
		// of logic
		if(dist != numeric_limits<int>::max()) {
		    if(dist > ret) {
			ret = dist;
		    }
		}
	    }
 	}
    }

    return ret;
}

#endif

