// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_DEGREE_DISTRIBUTION_HXX
#define BOOST_GRAPH_DEGREE_DISTRIBUTION_HXX

namespace boost
{
    template <
	typename BidirectionalGraph,
	typename Container
	>
    typename graph_traits<BindirectionalGraph>::degree_size_type
    degree_distribution(BidirectionalGraph &g, Container &dist)
    {
	typedef BidirectionalGraph graph;
	typedef typename graph_traits<Graph>::vertex_descriptor vertex;
	typedef typename graph_traits<Graph>::degree_size_type degree;

	// stash the degree of each vertex into its bucket - degree 1
	// goes into index 1, degree 90 goes into index 90, etc.
	degree max = 0;
	vertex_iterator i, j;
	for(tie(i, j) = vertices(g); i != j; ++i) {
	    degree d = degree(*i, g);

	    // we may need to resize the array to accomodate the
	    // incrementation of this degrees bucket.
	    if(d >= dist.size()) {
		dist.resize(d + 1);
	    }

	    // remember the max degree that we've seen
	    if(d > max) {
		max = d;
	    }

	    // increment the count in that bucket
	    dist[d] += 1;
	}
	return max;
    }
}

#endif


