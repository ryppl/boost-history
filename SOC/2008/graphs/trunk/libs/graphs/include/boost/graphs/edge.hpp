
#ifndef BOOST_GRAPHS_EDGE_HPP
#define BOOST_GRAPHS_EDGE_HPP

#include <boost/none.hpp>

namespace boost { namespace graphs {

// The edge interface...

/**
 * The edge traits class provides information about the end points and semantics
 * of edges, but not their labels.
 */
template <typename Edge>
struct edge_traits
{
    typedef typename Edge::end_pair end_pair;
};

} } /* namespace boost::graphs */


#endif
