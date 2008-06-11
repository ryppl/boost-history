
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

    // Constructors
    incidence_set();

    // Add an incident edge.
    inline void add(incidence_pair);

    // Allow a connection to the edge?
    inline std::pair<const_iterator, bool> allow(vertex_descriptor) const;

    // Find the edge record in question.
    inline iterator find(incidence_pair);
    inline const_iterator find(incidence_pair) const;

    // Remove the edge.
    inline void remove(incidence_pair);
    template <typename Eraser> inline void remove(vertex_descriptor, Eraser);

    // Remove all edges.
    inline void clear();


    inline size_type size() const
    { return _edges.size(); }

    inline const_iterator begin() const
    { return _edges.begin(); }

    inline const_iterator end() const
    { return _edges.end(); }

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
 * Deteremine whether or not the edge exists or is even allowed to be added.
 * This returns a pair containing an iterator indicating the position of the
 * edge if it already exists and a bool value indicating whether or not the
 * addition would even be allowed by policy.
 *
 * @complexity O(lg(d))
 */
template <BOOST_GRAPH_IS_PARAMS>
std::pair<typename incidence_set<E,C,A>::const_iterator, bool>
incidence_set<E,C,A>::allow(vertex_descriptor v) const
{
    // For now, always assume that the edge is allowed by policy, and determine
    // "addability" based on whehter or not the edge exists.
    return make_pair(_edges.find(v), true);
}


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
