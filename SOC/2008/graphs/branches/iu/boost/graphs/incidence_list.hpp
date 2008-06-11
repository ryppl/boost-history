
#ifndef INCIDENCE_LIST_HPP
#define INCIDENCE_LIST_HPP

#include <list>
#include <algorithm>

/**
 * The incidence vector stores incident "edges" of a vertex. In actuality,
 * this stores pairs containing an adjacent vertex descriptor and a property
 * descriptor, that points to the edges global properties. This pair uniquely
 * identifies incident edges of the vertex.
 *
 * This type allows constant time insertions, and linear search and remove.
 */
template <typename IncEdge, typename Alloc>
class incidence_list
{
    typedef std::list<IncEdge, Alloc> store_type;
public:
    typedef IncEdge incidence_pair;
    typedef typename IncEdge::first_type vertex_descriptor;
    typedef typename IncEdge::second_type property_descriptor;

    typedef typename store_type::iterator iterator;
    typedef typename store_type::const_iterator const_iterator;
    typedef typename store_type::size_type size_type;

    typedef none incidence_iterator;

    // Constructors
    incidence_list();

    inline void add(incidence_pair);

    inline std::pair<const_iterator, bool> allow(vertex_descriptor) const;
    inline iterator find(incidence_pair);
    inline const_iterator find(incidence_pair) const;

    inline void remove(incidence_pair);
    template <typename Eraser> inline void remove(vertex_descriptor, Eraser);

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

#define BOOST_GRAPH_IL_PARAMS \
    typename E, typename A

template <BOOST_GRAPH_IL_PARAMS>
incidence_list<E,A>::incidence_list()
    : _edges()
{ }

/**
 * Add the incident edge to the incidence set.
 *
 * @complexity O(1)
 */
template <BOOST_GRAPH_IL_PARAMS>
void
incidence_list<E,A>::add(incidence_pair p)
{
    _edges.push_back(p);
}

/**
 * Incidence lists always allow the addition of edges, assuming that no policy
 * conflicts exist. The first element of the return is the end() of the list.
 *
 * @complexity O(1)
 */
template <BOOST_GRAPH_IL_PARAMS>
std::pair<typename incidence_list<E,A>::const_iterator, bool>
incidence_list<E,A>::allow(vertex_descriptor v) const
{
    // Since there's no policy, there we must be able to add the edge.
    return make_pair(_edges.end(), true);
}

/**
 * Remove the incidence pair from the list. This operation is linear with
 * respect to the number of incident edges.
 *
 * @require EqualityComparable<vertex_descriptor>
 * @require EqualityComparable<property_descriptor>
 * @complexity O(d)
 */
template <BOOST_GRAPH_IL_PARAMS>
void
incidence_list<E,A>::remove(incidence_pair p)
{
    // Find the pair in the list and erase it.
    iterator iter = std::find(_edges.begin(), _edges.end(), p);
    this->_edges.erase(iter);
}

/**
 * Remove the all edges connecting the adjacent vertex from the list. This
 * operations is exactly linear w.r.t. the number of incident edges.
 *
 * @require EqualityComparable<vertex_descriptor>
 * @require EqualityComparable<property_descriptor>
 * @complexity Theta(d)
 */
template <BOOST_GRAPH_IL_PARAMS>
template <typename Eraser>
void
incidence_list<E,A>::remove(vertex_descriptor v, Eraser erase)
{
    iterator i = _edges.begin(), end = _edges.end();
    for( ; i != end; ) {
        if(i->first == v) {
            erase(i->second);
            i = _edges.erase(i);
        }
        else {
            ++i;
        }
    }
}

#if 0

// Functions

template <typename E>
vertex_edge_list<E>::vertex_edge_list()
    : base_type()
{ }

template <typename E>
void
vertex_edge_list<E>::add(edge_descriptor e)
{
    this->_store.push_back(e);
}

template <typename E>
typename vertex_edge_list<E>::iterator
vertex_edge_list<E>::find(edge_descriptor e)
{
    return std::find(this->_store.begin(), this->_store.end(), e);
}

template <typename E>
typename vertex_edge_list<E>::const_iterator
vertex_edge_list<E>::find(edge_descriptor e) const
{
    return std::find(this->_store.begin(), this->_store.end(), e);
}

template <typename E>
void
vertex_edge_list<E>::clear()
{
    this->_store.clear();
}

template <typename E>
typename vertex_edge_list<E>::size_type
vertex_edge_list<E>::size() const
{
    return this->_store.size();
}

#endif

#endif
