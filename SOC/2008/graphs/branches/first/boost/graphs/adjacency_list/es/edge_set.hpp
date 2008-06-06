
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_EDGE_SET_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_EDGE_SET_HPP

#include <set>

#include <boost/graphs/adjacency_list/es/simple_edge_iterator.hpp>

namespace boost {
namespace graphs {
namespace adj_list {

namespace detail {
    // Extend the notion of an edge for the edge set. Here, each edge also
    // stores an iterator to itself in the store. A little incestuous, but
    // it enables constant-time removals.
    template <
            typename Edge,
            template <typename> class Compare,
            template <typename> class Alloc
        >
    class basic_edge_set_node
        : public Edge
    {
        typedef basic_edge_set_node<Edge, Compare, Alloc> this_type;
    public:
        typedef typename Edge::vertex_descriptor vertex_descriptor;
        typedef typename Edge::properties_type properties_type;

        typedef typename std::set<
                this_type, Compare<this_type>, Alloc<this_type>
            >::iterator iterator;

        inline basic_edge_set_node(vertex_descriptor u,
                                   vertex_descriptor v,
                                   properties_type const& ep)
            : Edge(u, v, ep)
            , iter()
        { }

        iterator iter;
    };

} /* namespace detail */

/**
 * The edge set is basically used to implement simple graphs by not adding
 * duplicate (parallel) edges to the graph. This class considers edges to
 * be parallel if they have the same edge descriptors (i.e., the edges are
 * equivalent).
 *
 * Note that this underlying store also supports edge removals.
 */
template <
        typename Edge,
        template <typename> class Compare,
        template <typename> class Alloc
    >
class basic_edge_set
{
public:
    typedef detail::basic_edge_set_node<Edge, Compare, Alloc> edge_type;
    typedef typename edge_type::descriptor_type edge_descriptor;
    typedef typename edge_type::properties_type edge_properties;

    typedef typename edge_type::vertex_descriptor vertex_descriptor;

    typedef std::set<edge_type, Compare<edge_type>, Alloc<edge_type> > edge_store;
    typedef simple_edge_iterator<edge_store> edge_iterator;
    typedef typename edge_store::size_type edges_size_type;

    // FIXME:
    typedef hashed_property_map_tag edge_property_map_category;

    basic_edge_set();

    // Add edge
    edge_descriptor add_edge(vertex_descriptor u, vertex_descriptor v);
    edge_descriptor add_edge(vertex_descriptor u, vertex_descriptor v, edge_properties const& ep);

    // Insert edge
    std::pair<edge_descriptor, bool> insert_edge(vertex_descriptor u, vertex_descriptor v);
    std::pair<edge_descriptor, bool> insert_edge(vertex_descriptor u, vertex_descriptor v, edge_properties const& ep);

    // Number of edges
    edges_size_type num_edges() const;

    // Edge iterator accessors.
    std::pair<edge_iterator, edge_iterator> edges() const;
    edge_iterator begin_edges() const;
    edge_iterator end_edges() const;

    // Edge accessors
    edge_type& edge(edge_descriptor e);
    edge_type const& edge(edge_descriptor e) const;
    edge_properties& properties(edge_descriptor e);
    edge_properties const& properties(edge_descriptor e) const;

private:
    edge_store _edges;
};

/**
 * The default specialization uses the standard less comparator and the
 * standard allocator.
 */
template <typename Edge>
struct edge_set : basic_edge_set<Edge, std::less, std::allocator> { };

// Functions

#define BOOST_GRAPH_ES_PARAMS \
    typename E, template <typename> class C, template <typename> class A

template <BOOST_GRAPH_ES_PARAMS>
basic_edge_set<E,C,A>::basic_edge_set()
    : _edges()
{ }

/**
 * Add an edge to the set with no or default properties. If the edge already
 * exists in the set, no action is taken. Return a descruptor for the added
 * or existing edge.
 */
template <BOOST_GRAPH_ES_PARAMS>
typename basic_edge_set<E,C,A>::edge_descriptor
basic_edge_set<E,C,A>::add_edge(vertex_descriptor u, vertex_descriptor v)
{
    return insert_edge(u, v).first;
}

/**
 * Add an edge to the set with the given properties. If the edge already exists,
 * then no action is taken. Return a descriptor for the added or existing edge.
 */
template <BOOST_GRAPH_ES_PARAMS>
typename basic_edge_set<E,C,A>::edge_descriptor
basic_edge_set<E,C,A>::add_edge(vertex_descriptor u,
                              vertex_descriptor v,
                              edge_properties const& ep)
{
    return insert_edge(u, v, ep).first;
}

/**
 *
 */
template <BOOST_GRAPH_ES_PARAMS>
std::pair<typename basic_edge_set<E,C,A>::edge_descriptor, bool>
basic_edge_set<E,C,A>::insert_edge(vertex_descriptor u, vertex_descriptor v)
{
    return insert_edge(u, v, edge_properties());
}


/**
 * Add an edge with the given properties to the edge store.
 */
template <BOOST_GRAPH_ES_PARAMS>
std::pair<typename basic_edge_set<E,C,A>::edge_descriptor, bool>
basic_edge_set<E,C,A>::insert_edge(vertex_descriptor u,
                                   vertex_descriptor v,
                                   edge_properties const& ep)
{
    edge_type edge(u, v, ep);

    std::pair<edge_descriptor, bool> ret;
    std::pair<typename edge_store::iterator, bool> ins = _edges.insert(edge);
    if(ins.second) {
        // Not very pretty... the addition succeeds, so re-cast the added edge
        // and set the descriptor based on that.
        edge_type& e = const_cast<edge_type&>(*(ins.first));
        e.iter = ins.first;
        ret.first = &e;
    }
    else {
        ret.first = &const_cast<edge_type&>(*ins.first);
    }
    ret.second = ins.second;
    return ret;
}

/**
 * Get the number of edges in the edge set.
 */
template <BOOST_GRAPH_ES_PARAMS>
typename basic_edge_set<E,C,A>::edges_size_type
basic_edge_set<E,C,A>::num_edges() const
{
    return _edges.size();
}

/**
 * Get an iterator range for the edge set.
 */
template <BOOST_GRAPH_ES_PARAMS>
std::pair<
    typename basic_edge_set<E,C,A>::edge_iterator,
    typename basic_edge_set<E,C,A>::edge_iterator
>
basic_edge_set<E,C,A>::edges() const
{
    return std::make_pair(begin_edges(), end_edges());
}

/**
 * Get an iterator to the first edge in the set.
 */
template <BOOST_GRAPH_ES_PARAMS>
typename basic_edge_set<E,C,A>::edge_iterator
basic_edge_set<E,C,A>::begin_edges() const
{
    return _edges.begin();
}

/**
 * Get an iterator past the end of the edge set.
 */
template <BOOST_GRAPH_ES_PARAMS>
typename basic_edge_set<E,C,A>::edge_iterator
basic_edge_set<E,C,A>::end_edges() const
{
    return _edges.end();
}

/**
 * Get access to the given edge.
 */
template <BOOST_GRAPH_ES_PARAMS>
typename basic_edge_set<E,C,A>::edge_type&
basic_edge_set<E,C,A>::edge(edge_descriptor e)
{
    return *static_cast<edge_type*>(e.desc);
}

/**
 * Get access to the given edge.
 */
template <BOOST_GRAPH_ES_PARAMS>
typename basic_edge_set<E,C,A>::edge_type const&
basic_edge_set<E,C,A>::edge(edge_descriptor e) const
{
    return *static_cast<edge_type*>(e.desc);
}

/**
 * Access the properties of the given edge.
 */
template <BOOST_GRAPH_ES_PARAMS>
typename basic_edge_set<E,C,A>::edge_properties&
basic_edge_set<E,C,A>::properties(edge_descriptor e)
{
    return *edge(e);
}

/**
 * Access the properties of the given edge.
 */
template <BOOST_GRAPH_ES_PARAMS>
typename basic_edge_set<E,C,A>::edge_properties const&
basic_edge_set<E,C,A>::properties(edge_descriptor e) const
{
    return *edge(e);
}

#undef BOOST_GRAPH_ES_PARAMS

} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */

#endif
