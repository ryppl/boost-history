
#ifndef INCIDENCE_SET_HPP
#define INCICENCE_SET_HPP

#include <map>

/**
 * The incidence vector stores incident "edges" of a vertex. In actuality,
 * this stores pairs containing an adjacent vertex descriptor and a property
 * descriptor, that points to the edges global properties. This pair uniquely
 * identifies incident edges of the vertex.
 *
 * This type allows logarithmic time insertions, searches, and removals.
 */
template <typename IncEdge, typename Compare, typename Alloc>
class incidence_set
{
public:
    typedef IncEdge incidence_pair;
    typedef typename IncEdge::first_type vertex_descriptor;
    typedef typename IncEdge::second_type property_descriptor;
private:
    // Actually, the incident set, as it fundamentally implements a simple
    // graph is based on a map keyed on the adjacenct vertex and mapped to
    // the edge properties that describe it.
    // We're basically undwinding and rebuilding the edge pair for this map.
    typedef std::map<vertex_descriptor, property_descriptor, Compare, Alloc> store_type;
public:
    typedef typename store_type::iterator iterator;
    typedef typename store_type::const_iterator const_iterator;
    typedef typename store_type::size_type size_type;

    typedef none incidence_iterator;

    // Constructors
    incidence_set();

    inline void add(incidence_pair);

    inline iterator find(incidence_pair);
    inline const_iterator find(incidence_pair) const;

    inline void remove(incidence_pair);

    template <typename Eraser>
    inline void remove(vertex_descriptor, Eraser);

    inline void clear();

    inline size_type size() const;

private:
    store_type _edges;
};

// Functions

#define BOOST_GRAPH_IS_PARAMS \
    typename E, typename C, typename A

template <BOOST_GRAPH_IS_PARAMS>
incidence_set<E,C,A>::incidence_set()
    : _edges()
{ }

/**
 * Add the incidence pair to the set.
 *
 * @complexity O(lg(d))
 */
template <BOOST_GRAPH_IS_PARAMS>
void
incidence_set<E,C,A>::add(incidence_pair p)
{
    _edges.insert(p);
}

/**
 * Remove the incidence pair from the set.
 *
 * @complexity O(lg(d))
 */
template <BOOST_GRAPH_IS_PARAMS>
void
incidence_set<E,C,A>::remove(incidence_pair p)
{
    // The erase function takes a key! Not the entire pair.
    _edges.erase(p.first);
}

/**
 * Remove the incident edge indicated by the given vertex and invoke the
 * erase function on the associated property descriptor.
 */
template <BOOST_GRAPH_IS_PARAMS>
template <typename Erase>
void
incidence_set<E,C,A>::remove(vertex_descriptor v, Erase erase)
{
    // Find the mapped property descriptor before erasing the edge.
    iterator iter = _edges.find(v);
    property_descriptor p = iter->second;
    _edges.erase(iter);

    // Invoke the eraser to remove the corresponding global property.
    erase(p);
}

/**
 * Return the number of incident edges (degree) in the set.
 */
template <BOOST_GRAPH_IS_PARAMS>
typename incidence_set<E,C,A>::size_type
incidence_set<E,C,A>::size() const
{
    return _edges.size();
}

#undef BOOST_GRAPH_IS_PARAMS

#if 0

template <typename E>
void
incidence_set<E>::add(edge_descriptor e)
{
    this->_store.insert(e);
}

template <typename E>
typename incidence_set<E>::iterator
incidence_set<E>::find(edge_descriptor e)
{
    return this->_store.find(e);
}

template <typename E>
typename incidence_set<E>::const_iterator
incidence_set<E>::find(edge_descriptor e) const
{
    return this->_store.find(e);
}

template <typename E>
void
incidence_set<E>::clear()
{
    this->_store.clear();
}

template <typename E>
typename incidence_set<E>::size_type
incidence_set<E>::size() const
{
    return this->_store.size();
}

#endif

#endif
