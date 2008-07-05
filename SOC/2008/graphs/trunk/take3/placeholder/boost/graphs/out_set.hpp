
#ifndef OUT_SET_HPP
#define OUT_SET_HPP

#include <map>

#include "out_descriptor.hpp"

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
    typedef Edge out_pair;
    typedef typename Edge::first_type vertex_descriptor;
    typedef typename Edge::second_type edge_properties;
private:
    typedef std::map<vertex_descriptor, edge_properties, Compare, Alloc> store_type;
public:
    typedef typename store_type::iterator iterator;
    typedef typename store_type::const_iterator const_iterator;
    typedef typename store_type::size_type size_type;

    typedef basic_out_descriptor<iterator> out_descriptor;

    inline out_set()
        : _edges()
    { }

    /** Allow the edge insertion? */
    std::pair<out_descriptor, bool> allow(vertex_descriptor v)
    { return std::make_pair(out_descriptor(find(v)), true); }

    /** Add the edge to the set. */
    out_descriptor add(out_pair e)
    { return out_descriptor(_edges.insert(e).first); }

    /** Find the edge with the given vertex descriptor. */
    iterator find(vertex_descriptor v)
    { return _edges.find(v); }

    /** Find the edge with the given vertex descriptor. */
    const_iterator find(vertex_descriptor v) const
    { return _edges.find(v); }

    /**
     * Remove the edge with the given vertex descriptor.
     * @complexity O(log(deg(u)))
     */
    void remove(out_descriptor d)
    { _edges.erase(d.iter); }

    /** Remove all edges. */
    void clear()
    { _edges.clear(); }

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

#endif
