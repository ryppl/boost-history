
#ifndef OUT_LIST_HPP
#define OUT_LIST_HPP

#include <list>

#include "out_descriptor.hpp"

/**
 * The out list implements list-based, out-edge storage for directed graphs.
 * out-edges are uniquely identified by their target vertex and property
 * descriptor. List-based stores support fast inserts, slow finds, but do allow
 * removals.
 *
 * The edge is required to be a pair containing a vertex descriptor and a edge
 * property (not descriptor). This type defines the out_descriptor - an opaque
 * reference to a target/property pair.
 *
 * @param Edge A pair describing a vertex descriptor and the edge properties.
 * @param Alloc The allocator for edge pairs.
 */
template <typename Edge, typename Alloc>
class out_list
{
    typedef std::list<Edge, Alloc> store_type;
public:
    typedef Edge out_pair;
    typedef typename Edge::first_type vertex_descriptor;
    typedef typename Edge::second_type edge_properties;

    typedef typename store_type::iterator iterator;
    typedef typename store_type::const_iterator const_iterator;
    typedef typename store_type::size_type size_type;

    typedef basic_out_descriptor<out_pair> out_descriptor;

    inline out_list()
        : _edges()
    { }

    std::pair<const_iterator, bool> allow(vertex_descriptor v) const;
    out_descriptor add(out_pair);
    iterator find(out_pair);
    const_iterator find(out_pair) const;
    void remove(out_pair);
    void clear();

    /** @name Iterators and Size */
    //@{
    inline const_iterator begin() const
    { return _edges.begin(); }

    inline const_iterator end() const
    { return _edges.end(); }

    /** Get the number of outgoing edges. */
    inline size_type size() const
    { return _edges.size(); }
    //@{

private:
    store_type _edges;
};

/**
 * Out lists always allow the insertion of new edges, unless configured by
 * policy to do otherwise.
 */
template <typename E, typename A>
std::pair<typename out_list<E,A>::const_iterator, bool>
out_list<E,A>::allow(vertex_descriptor v) const
{
    return std::make_pair(_edges.end(), true);
}

/**
 * Add the incident edge, returning the property descriptor of the added
 * incidence pair.
 */
template <typename E, typename A>
typename out_list<E,A>::out_descriptor
out_list<E,A>::add(out_pair e)
{
    _edges.push_back(e);
    return _edges.back();
}

/**
 * Find the requested incidence pair in the list, returning an iterator to it.
 */
template <typename E, typename A>
typename out_list<E,A>::iterator
out_list<E,A>::find(out_pair e)
{
     return std::find(_edges.begin(), _edges.end(), e);
}

/**
 * Find the requested incidence pair in the list, returning an iterator to it.
 */
template <typename E, typename A>
typename out_list<E,A>::const_iterator
out_list<E,A>::find(out_pair e) const
{
    return std::find(_edges.begin(), _edges.end(), e);
}

/**
 * Remove the given incidence pair from the out list.
 */
template <typename E, typename A>
void
out_list<E,A>::remove(out_pair e)
{
    _edges.erase(find(e));
}

/**
 * Remove all edges from the out list.
 */
template <typename E, typename A>
void
out_list<E,A>::clear()
{
    _edges.clear();
}

#endif
