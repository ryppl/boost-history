// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_DEGREE_CENTRALITY_HXX
#define BOOST_GRAPH_DEGREE_CENTRALITY_HXX

namespace boost
{
    template <typename Graph>
    struct degree_centrality_measure
    {
        typedef typename graph_traits<Graph>::degree_size_type value_type;
        typedef typename graph_traits<Graph>::vertex_descriptor vertex_type;
    };

    template <typename Graph>
    struct basic_degree_centrality
        : public degree_centrality_measure<Graph>
    {
        typedef degree_centrality_measure<Graph> base_type;
        typedef typename base_type::value_type value_type;
        typedef typename base_type::vertex_type vertex_type;

        inline value_type operator ()(const Graph& g, vertex_type v)
        {
            typename graph_traits<Graph>::directed_category cat;
            return this->get_degree(g, v, cat);
        }

    private:
        inline value_type
        get_degree(const Graph& g, vertex_type v, undirected_tag)
        {
            return degree(v, g);
        }

        inline value_type
        get_degree(const Graph& g, vertex_type v, directed_tag)
        {
            return out_degree(v, g);
        }
    };

    template <typename Graph>
    inline basic_degree_centrality<Graph>
    measure_basic_degree_centrality(const Graph&)
    {
        return basic_degree_centrality<Graph>();
    }


    template <typename Graph, typename Measure>
    inline typename graph_traits<Graph>::degree_size_type
    vertex_degree_centrality(const Graph& g,
                             typename graph_traits<Graph>::vertex_descriptor v,
                             Measure measure)
    {
        return measure(g, v);
    }

    template <typename Graph>
    inline typename graph_traits<Graph>::degree_size_type
    vertex_degree_centrality(const Graph& g,
                             typename graph_traits<Graph>::vertex_descriptor v)
    {
        return vertex_degree_centrality(g, v,
                                        measure_basic_degree_centrality(g));
    }


    template <typename Graph, typename CentralityMap, typename Measure>
    void degree_centrality(const Graph& g,
                           CentralityMap cent,
                           Measure measure)
    {
        typename graph_traits<Graph>::vertex_iterator i, end;
        for(tie(i, end) = vertices(g); i != end; ++i) {
            cent[*i] = vertex_degree_centrality(g, *i, measure);
        }
    }

    template <typename Graph, typename CentralityMap>
    void degree_centrality(const Graph& g,
                           CentralityMap cent)
    {
        degree_centrality(g, cent, measure_basic_degree_centrality(g));
    }

}

#endif


