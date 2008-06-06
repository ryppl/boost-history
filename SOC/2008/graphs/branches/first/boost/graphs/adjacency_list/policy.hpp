
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_POLICY_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_POLICY_HPP

namespace boost {
namespace graphs {
namespace adj_list {

/**
 * Given valid vertex descriptors u and v, this policy will allow the addition
 * of loops (i.e., u == v).
 */
template <typename Graph>
struct allow_loops_policy
{
    static const bool allows_loose = false;
    static const bool allows_loops = true;

    typedef typename Graph::vertex_descriptor vertex;
    typedef typename Graph::edge_descriptor edge;
    typedef typename Graph::edge_properties properties;

    static bool allow(vertex u, vertex v, properties const& ep)
    {
        BOOST_ASSERT(u.is_valid() && v.is_valid());
        return true;
    }
};

/**
 * Given valid vertex descriptors u and v, this policy will not allow the
 * addition of loops.
 */
template <typename Graph>
struct no_loops_policy
{
    static const bool allows_loose = false;
    static const bool allows_loops = false;

    typedef typename Graph::vertex_descriptor vertex;
    typedef typename Graph::edge_descriptor edge;
    typedef typename Graph::edge_properties properties;

    static bool allow(vertex u, vertex v, properties const& ep)
    {
        BOOST_ASSERT(u.is_valid() && v.is_valid());
        return u != v;
    }
};


} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */

#endif
