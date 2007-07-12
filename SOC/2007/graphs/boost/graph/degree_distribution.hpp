// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_DEGREE_DISTRIBUTION_HXX
#define BOOST_GRAPH_DEGREE_DISTRIBUTION_HXX

#include <boost/graph/named_parameters.hpp>

namespace boost
{
    // Helper functions for computing degree distributions, histograms. Note
    // that when passed a not_given argumemt, all of these operations are
    // no-ops. The effect is that the actual computations shouldn't add hidden
    // constants at run-time.
    namespace detail
    {
        template <typename Dist, typename Graph>
        inline void
        max_degree(Dist&,
                   typename graph_traits<Graph>::degree_size_type& m,
                   typename graph_traits<Graph>::vertex_descriptor v,
                   const Graph& g)
        { m = max(m, degree(v, g)); }

        template <typename Graph>
        inline void
        max_degree(not_given,
                   typename graph_traits<Graph>::degree_size_type&,
                   typename graph_traits<Graph>::vertex_descriptor,
                   const Graph&)
        {}


        template <typename Dist, typename Graph>
        inline void
        max_out_degree(Dist&,
                       typename graph_traits<Graph>::degree_size_type& m,
                       typename graph_traits<Graph>::vertex_descriptor v,
                       const Graph& g)
        { m = max(m, out_degree(v, g)); }

        template <typename Graph>
        inline void
        max_out_degree(not_given,
                       typename graph_traits<Graph>::degree_size_type&,
                       typename graph_traits<Graph>::vertex_descriptor,
                       const Graph&)
        {}


        template <typename Dist, typename Graph>
        inline void
        max_in_degree(Dist&,
                      typename graph_traits<Graph>::degree_size_type& m,
                      typename graph_traits<Graph>::vertex_descriptor v,
                      const Graph& g)
        { m = max(m, in_degree(v, g)); }

        template <typename Graph>
        inline void
        max_in_degree(not_given,
                      typename graph_traits<Graph>::degree_size_type&,
                      typename graph_traits<Graph>::vertex_descriptor,
                      const Graph&)
        { }


        template <typename Dist>
        inline void resize_distribution(Dist& d, size_t n)
        { d.resize(n); }

        inline void resize_distribution(not_given, size_t n)
        { }


        template <typename Hist, typename Graph>
        inline void observe_degree(Hist& d,
                                   typename graph_traits<Graph>::vertex_descriptor v,
                                   const Graph& g)
        { d[degree(v, g)] += 1; }

        template <typename Graph>
        inline void observe_degree(not_given,
                                   typename graph_traits<Graph>::vertex_descriptor v,
                                   const Graph& g)
        { }


        template <typename Hist, typename Graph>
        inline void observe_out_degree(Hist& d,
                                       typename graph_traits<Graph>::vertex_descriptor v,
                                       const Graph& g)
        { d[out_degree(v, g)] += 1; }

        template <typename Graph>
        inline void observe_out_degree(not_given,
                                       typename graph_traits<Graph>::vertex_descriptor v,
                                       const Graph& g)
        { }


        template <typename Dist, typename Graph>
        inline void observe_in_degree(Dist& d,
                                      typename graph_traits<Graph>::vertex_descriptor v,
                                      const Graph& g)
        { d[in_degree(v, g)] += 1; }

        template <typename Graph>
        inline void observe_in_degree(not_given,
                                      typename graph_traits<Graph>::vertex_descriptor v,
                                      const Graph& g)
        { }


        template <typename Hist, typename Graph>
        inline void record_degree(Hist& h,
                                  typename graph_traits<Graph>::vertex_descriptor v,
                                  const Graph& g)
        { h[degree(v, g)].push_back(v); }

        template <typename Graph>
        inline void record_degree(not_given,
                                  typename graph_traits<Graph>::vertex_descriptor v,
                                  const Graph& g)
        { }


        template <typename Hist, typename Graph>
        inline void record_out_degree(Hist& h,
                                      typename graph_traits<Graph>::vertex_descriptor v,
                                      const Graph& g)
        { h[out_degree(v, g)].push_back(v); }

        template <typename Graph>
        inline void record_out_degree(not_given,
                                      typename graph_traits<Graph>::vertex_descriptor v,
                                      const Graph& g)
        { }


        template <typename Hist, typename Graph>
        inline void record_in_degree(Hist& h,
                                     typename graph_traits<Graph>::vertex_descriptor v,
                                     const Graph& g)
        { h[in_degree(v, g)].push_back(v); }

        template <typename Graph>
        inline void record_in_degree(not_given,
                                     typename graph_traits<Graph>::vertex_descriptor v,
                                     const Graph& g)
        { }
    }

    // A helper function for initializing distributions and histograms
    BOOST_PARAMETER_FUNCTION(
        (void), initialize_distribution, tag,
        (required (graph, *))
        (optional
            (out(distribution), *, not_given())
            (out(out_distribution), *, not_given())
            (out(in_distribution), *, not_given()))
        )
    {
        typename graph_traits<graph_type>::vertex_iterator i, end;

        // part 1: find the max observable degrees for the graph so we
        // only have to resize once. note that this relaxes requirements on
        // the distribution type - it just needs to be resizable.
        size_t max_d = 0, max_od = 0, max_id = 0;
        for(tie(i, end) = vertices(graph); i != end; ++i) {
            typename graph_type::vertex_descriptor v = *i;
            detail::max_degree(distribution, max_d, v, graph);
            detail::max_out_degree(out_distribution, max_od, v, graph);
            detail::max_in_degree(in_distribution, max_id, v, graph);
        }
        detail::resize_distribution(distribution, max_d + 1);
        detail::resize_distribution(out_distribution, max_od + 1);
        detail::resize_distribution(in_distribution, max_id + 1);
    }

    // the actual degree_distribution function
    BOOST_PARAMETER_FUNCTION(
        (void), degree_distribution, tag,
        (required (graph, *))
        (optional
            (out(distribution), *, not_given())
            (out(out_distribution), *, not_given())
            (out(in_distribution), *, not_given()))
        )
    {
        typename graph_traits<graph_type>::vertex_iterator i, end;

        // part 1: initialize distributions
        initialize_distribution(graph,
            _distribution = distribution,
            _out_distribution = out_distribution,
            _in_distribution = in_distribution);

        // part 2: record observed distributions
        for(tie(i, end) = vertices(graph); i != end; ++i) {
            typename graph_type::vertex_descriptor v = *i;
            detail::observe_degree(distribution, v, graph);
            detail::observe_out_degree(out_distribution, v, graph);
            detail::observe_in_degree(in_distribution, v, graph);
        }
    }

    // the actual degree_histogram function
    BOOST_PARAMETER_FUNCTION(
        (void), degree_histogram, tag,
        (required (graph, *))
        (optional
            (out(histogram), *, not_given())
            (out(out_histogram), *, not_given())
            (out(in_histogram), *, not_given()))
        )
    {
        typename graph_traits<graph_type>::vertex_iterator i, end;

        // part 1: initialize distributions
        initialize_distribution(graph,
            _distribution = histogram,
            _out_distribution = out_histogram,
            _in_distribution = in_histogram);

        // part 2: record observed distributions
        for(tie(i, end) = vertices(graph); i != end; ++i) {
            typename graph_type::vertex_descriptor v = *i;
            detail::record_degree(histogram, v, graph);
            detail::record_out_degree(out_histogram, v, graph);
            detail::record_in_degree(in_histogram, v, graph);
        }
    }
}

#endif


