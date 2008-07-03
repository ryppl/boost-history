
#ifndef OUT_SET_HPP
#define OUT_SET_HPP

#include <map>
#include <memory>

#include <boost/type_traits.hpp>

#include "mapped_iterator.hpp"

/**
 * The out set implements set-based, out-edge storage for directed graphs.
 * out-edges are uniquely identified by their target vertex and property
 * descriptor. List-based stores support medium inserts, mediam finds, and allow
 * removals.
 *
 * The edge is required to be a pair containing a vertex descriptor and a edge
 * property (not descriptor). This type defines the out_descriptor - an opaque
 * reference to a target/property pair.
 *
 * The out edge set is actually implemented as a mapping from vertex descriptor
 * to stored edge property.
 *
 * @param Edge A pair describing a vertex descriptor and the edge properties.
 * @param Alloc The allocator for edge pairs.
 */
template <typename Edge, typename Compare, typename Alloc>
class out_set
{
public:
    typedef Edge out_tuple;
    typedef typename Edge::first_type vertex_descriptor;
    typedef typename Edge::second_type edge_properties;
private:
    typedef typename Edge::third_type in_edge_place;

    // Reconstruct the edge triple into a key/value type thing for the map.
    // Unfortunately, we're storing the descriptor twice, but this does make
    // iteration and referencing a bit easier.
    typedef std::map<
            vertex_descriptor,
            triple<vertex_descriptor, edge_properties, in_edge_place>,
            Compare, Alloc
        > store_type;
public:
    typedef mapped_iterator<typename store_type::iterator> iterator;
    typedef typename store_type::size_type size_type;

    inline out_set()
        : _edges()
    { }

    /** Allow the edge insertion? */
    std::pair<iterator, bool> allow(vertex_descriptor v) const
    { return std::make_pair(_edges.find(v), true); }

    /**
     * Add the edge to the set.
     * @complexity O(log(deg(v)))
     */
    iterator add(vertex_descriptor v, edge_properties const& ep)
    { return _edges.insert(std::make_pair(v, make_triple(v, ep, in_edge_place()))).first; }

    /**
     * Find the edge with the given vertex descriptor.
     * @complexity O(log(deg(v)))
     */
    iterator find(vertex_descriptor v) const
    { return _edges.find(v); }

    /**
     * Remove the edge with the given vertex descriptor.
     * @complexity O(log(deg(u)))
     */
    void remove(iterator d)
    { _edges.erase(d.iter); }

    /** Remove all edges. */
    void clear()
    { _edges.clear(); }

    /** @name Iterators */
    //@{
    inline iterator begin() const
    { return _edges.begin(); }

    inline iterator end() const
    { return iterator(); }
    //@}

    /** Get the number of outgoing edges. */
    inline size_type size() const
    { return _edges.size(); }

private:
    mutable store_type _edges;
};

#endif
