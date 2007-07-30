// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_GEODESIC_HPP
#define BOOST_GRAPH_GEODESIC_HPP

#include <limits>

#include <boost/graph/named_parameters.hpp>
#include <boost/graph/numeric_values.hpp>
#include <boost/graph/detail/combine_distances.hpp>
#include <boost/graph/exterior_property.hpp>

namespace boost
{
    template <typename Graph,
              typename DistanceType,
              typename ResultType>
    struct closeness_centrality_measure
    {
        typedef DistanceType distance_type;
        typedef ResultType result_type;
        typedef typename graph_traits<Graph>::vertices_size_type size_type;

        typedef numeric_values<distance_type> distance_values;
        typedef numeric_values<result_type> result_values;

        static inline distance_type infinite_distance()
        { return distance_values::infinity(); }

        static inline result_type infinite_result()
        { return result_values::infinity(); }

        static inline result_type zero_result()
        { return result_values::zero(); }
    };

    template <typename Graph,
              typename DistanceNumbers,
              typename ResultNumbers>
    struct total_geodesic_measure
        : public closeness_centrality_measure<Graph, DistanceNumbers, ResultNumbers>
    {
        typedef closeness_centrality_measure<
                Graph, DistanceNumbers, ResultNumbers
            > base_type;
        typedef typename base_type::distance_type distance_type;
        typedef typename base_type::result_type result_type;
        typedef typename base_type::size_type size_type;

        result_type operator ()(distance_type d, size_type n)
        { return d; }
    };

    template <typename Graph, typename DistanceMap>
    inline total_geodesic_measure<Graph, typename property_traits<DistanceMap>::value_type, float>
    measure_total_geodesic(const Graph&, DistanceMap)
    {
        return total_geodesic_measure<Graph, typename property_traits<DistanceMap>::value_type, float>();
    }

    template <typename T, typename Graph, typename DistanceMap>
    inline total_geodesic_measure<Graph, typename property_traits<DistanceMap>::value_type, T>
    measure_total_geodesic(const Graph&, DistanceMap)
    {
        return total_geodesic_measure<Graph, typename property_traits<DistanceMap>::value_type, T>();
    }

    template <typename Graph,
              typename DistanceNumbers,
              typename ResultNumbers>
    struct mean_geodesic_measure
        : public closeness_centrality_measure<Graph, DistanceNumbers, ResultNumbers>
    {
        typedef closeness_centrality_measure<
                Graph, DistanceNumbers, ResultNumbers
            > base_type;
        typedef typename base_type::distance_type distance_type;
        typedef typename base_type::result_type result_type;
        typedef typename base_type::size_type size_type;

        result_type operator ()(distance_type d, size_type n)
        {
            typedef result_type T;
            return
                (d == base_type::infinite_distance()) ?
                    base_type::infinite_result() : T(d) / T(n);
        }
    };

    template <typename Graph, typename DistanceMap>
    inline mean_geodesic_measure<Graph, typename property_traits<DistanceMap>::value_type, float>
    measure_mean_geodesic(const Graph&, DistanceMap)
    {
        return mean_geodesic_measure<Graph, typename property_traits<DistanceMap>::value_type, float>();
    }

    template <typename T, typename Graph, typename DistanceMap>
    inline mean_geodesic_measure<Graph, typename property_traits<DistanceMap>::value_type, T>
    measure_mean_geodesic(const Graph&, DistanceMap)
    {
        return mean_geodesic_measure<Graph, typename property_traits<DistanceMap>::value_type, T>();
    }


    template <typename Graph,
              typename DistanceNumbers,
              typename ResultNumbers>
    struct inverse_geodesic_measure
        : public closeness_centrality_measure<Graph, DistanceNumbers, ResultNumbers>
    {
        typedef closeness_centrality_measure<
                Graph, DistanceNumbers, ResultNumbers
            > base_type;
        typedef typename base_type::distance_type distance_type;
        typedef typename base_type::result_type result_type;
        typedef typename base_type::size_type size_type;

        result_type operator ()(distance_type d, size_type n)
        {
            typedef result_type T;
            return
                (d == base_type::infinite_distance()) ?
                    base_type::zero_result() : T(n) / T(d);
        }
    };

    template <typename Graph, typename DistanceMap>
    inline inverse_geodesic_measure<Graph, typename property_traits<DistanceMap>::value_type, float>
    measure_inverse_geodesic(const Graph&, DistanceMap)
    {
        return inverse_geodesic_measure<Graph, typename property_traits<DistanceMap>::value_type, float>();
    }

    template <typename T, typename Graph, typename DistanceMap>
    inline inverse_geodesic_measure<Graph, typename property_traits<DistanceMap>::value_type, T>
    measure_inverse_geodesic(const Graph&, DistanceMap)
    {
        return inverse_geodesic_measure<Graph, typename property_traits<DistanceMap>::value_type, T>();
    }


    template <typename Graph,
              typename DistanceNumbers,
              typename ResultNumbers>
    struct closeness_measure
        : public closeness_centrality_measure<Graph, DistanceNumbers, ResultNumbers>
    {
        typedef closeness_centrality_measure<
                Graph, DistanceNumbers, ResultNumbers
            > base_type;
        typedef typename base_type::distance_type distance_type;
        typedef typename base_type::result_type result_type;
        typedef typename base_type::size_type size_type;

        result_type operator ()(distance_type d, size_type n)
        {
            typedef result_type T;
            return
                (d == base_type::infinite_distance()) ?
                    base_type::zero_result() : T(1) / T(d);
        }
    };

    template <typename Graph, typename DistanceMap>
    inline closeness_measure<Graph, typename property_traits<DistanceMap>::value_type, float>
    measure_closeness(const Graph&, DistanceMap)
    {
        return closeness_measure<Graph, typename property_traits<DistanceMap>::value_type, float>();
    }

    template <typename T, typename Graph, typename DistanceMap>
    inline closeness_measure<Graph, typename property_traits<DistanceMap>::value_type, T>
    measure_closeness(const Graph&, DistanceMap)
    {
        return closeness_measure<Graph, typename property_traits<DistanceMap>::value_type, T>();
    }


    template <typename Graph,
              typename DistanceMap,
              typename Measure,
              typename Combinator>
    inline typename Measure::result_type
    closeness_centrality(const Graph& g,
                         DistanceMap dist,
                         Measure measure,
                         Combinator combine)
    {
        typedef typename Measure::distance_type Distance;
        typedef typename Measure::distance_values DistanceValues;
        Distance n = detail::combine_distances(g, dist, combine,
                                               DistanceValues());
        return measure(n, num_vertices(g));
    }


    template <typename Graph,
              typename DistanceMap,
              typename Measure>
    inline typename Measure::result_type
    closeness_centrality(const Graph& g,
                         DistanceMap dist,
                         Measure measure)
    {
        typedef typename Measure::distance_type Distance;
        return closeness_centrality(g, dist, measure, std::plus<Distance>());
    }

    template <typename Graph, typename DistanceMap>
    inline float
    total_geodesic_distance(const Graph& g, DistanceMap dist)
    {
        return closeness_centrality(g, dist, measure_total_geodesic(g, dist));
    }

    template <typename T, typename Graph, typename DistanceMap>
    inline T
    total_geodesic_distance(const Graph& g, DistanceMap dist)
    {
        return closeness_centrality(g, dist, measure_total_geodesic<T>(g, dist));
    }

    template <typename Graph, typename DistanceMap>
    inline float
    mean_geodesic_distance(const Graph& g, DistanceMap dist)
    {
        return closeness_centrality(g, dist, measure_mean_geodesic(g, dist));
    }

    template <typename T, typename Graph, typename DistanceMap>
    inline T
    mean_geodesic_distance(const Graph& g, DistanceMap dist)
    {
        return closeness_centrality(g, dist, measure_mean_geodesic<T>(g, dist));
    }

    template <typename Graph, typename DistanceMap>
    inline float
    inverse_geodesic_distance(const Graph& g, DistanceMap dist)
    {
        return closeness_centrality(g, dist, measure_inverse_geodesic(g, dist));
    }

    template <typename T, typename Graph, typename DistanceMap>
    inline T
    inverse_geodesic_distance(const Graph& g, DistanceMap dist)
    {
        return closeness_centrality(g, dist, measure_inverse_geodesic<T>(g, dist));
    }

    template <typename Graph, typename DistanceMap>
    inline float
    closeness(const Graph& g, DistanceMap dist)
    {
        return closeness_centrality(g, dist, measure_closeness(g, dist));
    }

    template <typename T, typename Graph, typename DistanceMap>
    inline T
    closeness(const Graph& g, DistanceMap dist)
    {
        return closeness_centrality(g, dist, measure_closeness<T>(g, dist));
    }

    template <typename Graph,
              typename DistanceMatrix,
              typename ClosenessMap,
              typename Measure>
    inline void
    all_closeness_centralities(const Graph& g,
                               DistanceMatrix& matrix,
                               ClosenessMap& close,
                               Measure measure)
    {
        typedef typename property_matrix_traits<DistanceMatrix>::value_type Distance;

        typename graph_traits<Graph>::vertex_iterator i, end;
        for(tie(i, end) = vertices(g); i != end; ++i) {
            close[*i] = closeness_centrality(g, matrix[*i], measure);
        }
    }

    template <typename Graph,
              typename DistanceMatrix,
              typename ClosenessMap>
    inline void
    all_total_geodesic_distances(const Graph& g,
                                 DistanceMatrix& matrix,
                                 ClosenessMap& close)
    {
        typedef typename property_matrix_traits<DistanceMatrix>::value_type Distance;
        typedef typename property_traits<ClosenessMap>::value_type Result;

        all_closeness_centralities(g, matrix, close,
                                   measure_total_geodesic<Result>(g, close));
    }

    template <typename Graph,
              typename DistanceMatrix,
              typename ClosenessMap>
    inline void
    all_mean_geodesic_distances(const Graph& g,
                                DistanceMatrix& matrix,
                                ClosenessMap& close)
    {
        typedef typename property_matrix_traits<DistanceMatrix>::value_type Distance;
        typedef typename property_traits<ClosenessMap>::value_type Result;

        all_closeness_centralities(g, matrix, close,
                                   measure_mean_geodesic<Result>(g, close));
    }

    template <typename Graph,
              typename DistanceMatrix,
              typename ClosenessMap>
    inline void
    all_inverse_geodesic_distances(const Graph& g,
                                   DistanceMatrix& matrix,
                                   ClosenessMap& close)
    {
        typedef typename property_matrix_traits<DistanceMatrix>::value_type Distance;
        typedef typename property_traits<ClosenessMap>::value_type Result;

        all_closeness_centralities(g, matrix, close,
                                   measure_inverse_geodesic<Result>(g, close));
    }

    template <typename Graph,
              typename DistanceMatrix,
              typename ClosenessMap>
    inline void
    all_closenesses(const Graph& g,
                    DistanceMatrix& matrix,
                    ClosenessMap& close)
    {
        typedef typename property_matrix_traits<DistanceMatrix>::value_type Distance;
        typedef typename property_traits<ClosenessMap>::value_type Result;

        all_closeness_centralities(g, matrix, close,
                                   measure_closeness<Result>(g, close));
    }
}

#endif
