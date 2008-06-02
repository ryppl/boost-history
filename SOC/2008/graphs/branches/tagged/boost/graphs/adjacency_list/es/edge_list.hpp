
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_EDGE_LIST_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_EDGE_LIST_HPP

#include <list>

#include <boost/graphs/adjacency_list/es/simple_edge_iterator.hpp>

namespace boost {
namespace graphs {
namespace adj_list {

namespace detail {
    // Extend the notion of an edge for the edge set. Here, each edge also
    // stores an iterator to itself in the store. A little incestuous, but
    // it enables constant-time removals.
    template <typename Edge, template <typename> class Alloc>
    class basic_edge_list_node
        : public Edge
    {
        typedef basic_edge_list_node<Edge, Alloc> this_type;
    public:
        typedef typename Edge::vertex_descriptor vertex_descriptor;
        typedef typename Edge::properties_type properties_type;

        typedef typename std::list<
                this_type, Alloc<this_type>
            >::iterator iterator;

        inline basic_edge_list_node(vertex_descriptor u,
                                    vertex_descriptor v,
                                    properties_type const& p)
            : Edge(u, v, p)
            , iter()
        { }

        iterator iter;
    };

} /* namespace detail */

/**
 * The edge list can be used to implement multigraphs with removable edges.
 * It's insert and remove functions are constant time functions, but accessing
 * the number of edges is a linear time operation.
 */
template <
        typename Edge,
        template <typename> class Alloc
    >
class basic_edge_list
{
public:
    typedef detail::basic_edge_list_node<Edge, Alloc> edge_type;
    typedef typename edge_type::descriptor_type edge_descriptor;
    typedef typename edge_type::vertex_descriptor vertex_descriptor;
    typedef typename edge_type::properties_type edge_properties;

    typedef std::list<edge_type, Alloc<edge_type> > edge_store;
    typedef simple_edge_iterator<edge_store> edge_iterator;
    typedef typename edge_store::size_type edges_size_type;

    // FIXME:
    typedef hashed_property_map_tag edge_property_map_category;

    basic_edge_list();

    // Add edge
    edge_descriptor add_edge(vertex_descriptor u, vertex_descriptor v);
    edge_descriptor add_edge(vertex_descriptor u, vertex_descriptor v, edge_properties const& ep);

    // Remove edge
    void remove_edge(edge_descriptor e);
    void remove_edge(vertex_descriptor u, vertex_descriptor v);

    // Number of edges
    edges_size_type num_edges() const;

    // Edge iterator accessors.
    std::pair<edge_iterator, edge_iterator> edges() const;
    edge_iterator begin_edges() const;
    edge_iterator end_edges() const;

    // Edge and edge property accessors.
    edge_type& edge(edge_descriptor e);
    edge_type const& edge(edge_descriptor e) const;
    edge_descriptor descriptor(edge_type const& e) const;

    // Property accessors
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
struct edge_list : basic_edge_list<Edge, std::allocator> { };

// Functions

#define BOOST_GRAPH_EL_PARAMS \
    typename E, template <typename> class A

template <BOOST_GRAPH_EL_PARAMS>
basic_edge_list<E,A>::basic_edge_list()
    : _edges()
{ }

/**
 * Add an edge to the set with no or default properties. If the edge already
 * exists in the set, no action is taken. Return a descruptor for the added
 * or existing edge.
 */
template <BOOST_GRAPH_EL_PARAMS>
typename basic_edge_list<E,A>::edge_descriptor
basic_edge_list<E,A>::add_edge(vertex_descriptor u, vertex_descriptor v)
{
    return add(u, v, edge_properties());
}

/**
 * Add an edge to the set with the given properties. If the edge already exists,
 * then no action is taken. Return a descriptor for the added or existing edge.
 */
template <BOOST_GRAPH_EL_PARAMS>
typename basic_edge_list<E,A>::edge_descriptor
basic_edge_list<E,A>::add_edge(vertex_descriptor u,
                               vertex_descriptor v,
                               edge_properties const& ep)
{
    typename edge_store::iterator i =
            _edges.insert(_edges.end(), edge_type(u, v, ep));
    i->iter = i;
    return &(*i);
}

/**
 * Remove the given edge from the edge set.
 *
 * @complexity O(|E|)
 */
template <BOOST_GRAPH_EL_PARAMS>
void
basic_edge_list<E,A>::remove_edge(edge_descriptor e)
{
    // Basically, iterate over the edge list until we find the descriptor
    // e, then erase it. Note that descriptors are unique so there's really
    // only one such e.
    typename edge_store::iterator i = _edges.begin(), end = _edges.end();
    for( ; i != end; ++i) {
        if(descriptor(*i) == e) {
            _edges.erase(i);
            break;
        }
    }
}

/**
 * Remove all edges connecting the vertices u and v from the set.
 *
 * @complexity O(|E|)
 */
template <BOOST_GRAPH_EL_PARAMS>
void
basic_edge_list<E,A>::remove_edge(vertex_descriptor u, vertex_descriptor v)
{
    // Create a temporary edge over u and v. This will sort the vertices as
    // needed before actually inserting it.
    edge_type tmp(u, v, edge_properties());

    // Iterate over the edges and erase those connecting u and v.
    typename edge_store::iterator i = _edges.begin(), end = _edges.end();
    for( ; i != end; ) {
        if(*i == tmp) {
            i = _edges.erase(i);
        }
        else {
            ++i;
        }
    }
}


/**
 * Get the number of edges in the edge set.
 */
template <BOOST_GRAPH_EL_PARAMS>
typename basic_edge_list<E,A>::edges_size_type
basic_edge_list<E,A>::num_edges() const
{
    return _edges.size();
}

/**
 * Get an iterator range for the edge set.
 */
template <BOOST_GRAPH_EL_PARAMS>
std::pair<
    typename basic_edge_list<E,A>::edge_iterator,
    typename basic_edge_list<E,A>::edge_iterator
>
basic_edge_list<E,A>::edges() const
{
    return std::make_pair(begin_edges(), end_edges());
}

/**
 * Get an iterator to the first edge in the set.
 */
template <BOOST_GRAPH_EL_PARAMS>
typename basic_edge_list<E,A>::edge_iterator
basic_edge_list<E,A>::begin_edges() const
{
    return _edges.begin();
}

/**
 * Get an iterator past the end of the edge set.
 */
template <BOOST_GRAPH_EL_PARAMS>
typename basic_edge_list<E,A>::edge_iterator
basic_edge_list<E,A>::end_edges() const
{
    return _edges.end();
}

/**
 * Get access to the given edge.
 */
template <BOOST_GRAPH_EL_PARAMS>
typename basic_edge_list<E,A>::edge_type&
basic_edge_list<E,A>::edge(edge_descriptor e)
{
    return *static_cast<edge_type*>(e.desc);
}

/**
 * Get access to the given edge.
 */
template <BOOST_GRAPH_EL_PARAMS>
typename basic_edge_list<E,A>::edge_type const&
basic_edge_list<E,A>::edge(edge_descriptor e) const
{
    return *static_cast<edge_type*>(e.desc);
}

/**
 * Get a descriptor for the given edge.
 */
template <BOOST_GRAPH_EL_PARAMS>
typename basic_edge_list<E,A>::edge_descriptor
basic_edge_list<E,A>::descriptor(edge_type const& e) const
{
    return &const_cast<edge_type&>(e);
}


/**
 * Access the properties of the given edge.
 */
template <BOOST_GRAPH_EL_PARAMS>
typename basic_edge_list<E,A>::edge_properties&
basic_edge_list<E,A>::properties(edge_descriptor e)
{
    return *edge(e);
}

/**
 * Access the properties of the given edge.
 */
template <BOOST_GRAPH_EL_PARAMS>
typename basic_edge_list<E,A>::edge_properties const&
basic_edge_list<E,A>::properties(edge_descriptor e) const
{
    return *edge(e);
}

#undef BOOST_GRAPH_EL_PARAMS

} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */

#endif
