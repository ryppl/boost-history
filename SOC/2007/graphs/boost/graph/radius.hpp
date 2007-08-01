// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_RADIUS_HPP
#define BOOST_GRAPH_RADIUS_HPP

namespace boost
{
    template <typename Graph, typename EccentricityMap>
    inline typename property_traits<EccentricityMap>::value_type
    graph_radius(const Graph& g, EccentricityMap ecc)
    {
        typedef typename property_traits<EccentricityMap>::value_type T;
        typename graph_traits<Graph>::vertex_iterator i, end;
        T ret = ecc[*vertices(g).first];
        for(tie(i, end) = vertices(g); i != end; ++i) {
            ret = std::min(ret, ecc[*i]);
        }
        return ret;
    }
}

#endif
