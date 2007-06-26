// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_DEGREE_DISTRIBUTION_HXX
#define BOOST_GRAPH_DEGREE_DISTRIBUTION_HXX

namespace boost
{
    template <typename Graph, typename Distribution>
    void
    degree_distribution(const Graph &g, Distribution &dist)
    {
        typedef typename graph_traits<Graph>::degree_size_type Degree;

            // stash the degree of each vertex into its bucket - degree 1
            // goes into index 1, degree 90 goes into index 90, etc.
        typename Graph::vertex_iterator i, j;
        for(tie(i, j) = vertices(g); i != j; ++i) {
            Degree d = degree(*i, g);

                // we may need to resize the array to accomodate the
                // incrementation of this degrees bucket. this only looks
                // like its an inefficient resize, but its just fine with
                // a vector for the distribution.
            if(d >= dist.size()) {
                dist.resize(d + 1);
            }

                // increment the count in that bucket
            dist[d] += 1;
        }
    }


    template <typename Graph, typename Histogram>
    void
    degree_histogram(const Graph &g, Histogram &hist)
    {
        typedef typename graph_traits<Graph>::degree_size_type Degree;

        // stash the degree of each vertex into its bucket - degree 1
        // goes into index 1, degree 90 goes into index 90, etc.
        typename Graph::vertex_iterator i, j;
        for(tie(i, j) = vertices(g); i != j; ++i) {
            Degree d = degree(*i, g);

            // we may need to resize the array to accomodate the
            // incrementation of this degrees bucket. this only looks
            // like its an inefficient resize, but its just fine with
            // a vector for the distribution.
            if(d >= hist.size()) {
                hist.resize(d + 1);
            }

            // increment the count in that bucket
            hist[d].push_back(*i);
        }
    }
}

#endif


