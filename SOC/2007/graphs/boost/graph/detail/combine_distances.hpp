// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_DETAIL_COMBINE_DISTANCES_GRAPH_HPP
#define BOOST_GRAPH_DETAIL_COMBINE_DISTANCES_GRAPH_HPP

#include <functional>

namespace boost
{
    namespace detail
    {
        // Note that this assumes T == property_traits<DistanceMap>::value_type
        // and that the args and return of combine are also T.
        template <typename Graph,
                  typename DistanceMap,
                  typename Combinator,
                  typename DistanceNumbers>
        inline typename DistanceNumbers::value_type
        combine_distances(const Graph& g,
                          DistanceMap dist,
                          Combinator combine,
                          DistanceNumbers distnum)
        {
            // If there's ever an infinite distance, then we simply
            // return infinity.
            typename DistanceNumbers::value_type ret(DistanceNumbers::zero());
            typename graph_traits<Graph>::vertex_iterator i, end;
            for(tie(i, end) = vertices(g); i != end; ++i) {
                if(dist[*i] != DistanceNumbers::infinity()) {
                    ret = combine(ret, dist[*i]);
                }
                else {
                    ret = DistanceNumbers::infinity();
                    break;
                }
            }
            return ret;
        }


        // Similar to std::plus<T>, but maximizes parameters
        // rather than adding them.
        template <typename T>
        struct maximize : public std::binary_function<T, T, T>
        {
            T operator ()(T x, T y) const
            { return std::max(x, y); }
        };
    }
}
#endif
