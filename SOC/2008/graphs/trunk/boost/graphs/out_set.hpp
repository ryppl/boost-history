
#ifndef OUT_SET_HPP
#define OUT_SET_HPP

#include <map>
#include <memory>

#include <boost/triple.hpp>
#include <boost/descriptors.hpp>

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
    typedef typename Edge::first_type vertex_descriptor;
    typedef typename Edge::second_type edge_properties;
    typedef typename Edge::third_type in_descriptor;

    // Reconstruct the edge triple into a key/value type thing for the map.
    typedef std::map< vertex_descriptor, std::pair<edge_properties, in_descriptor>, Compare, Alloc> store_type;
    typedef typename store_type::iterator iterator;
    typedef typename store_type::size_type size_type;

    typedef typename descriptor_traits<store_type>::descriptor_type out_descriptor;

    // Constructor
    inline out_set()
        : _edges()
    { }

    /** Allow the edge insertion? */
    std::pair<iterator, bool> allow(vertex_descriptor v) const
    { return std::make_pair(_edges.find(v), true); }

    /**
     * Try to add the given edge to the set. If the edge already exists, return
     * a null descriptor.
     * @complexity O(log(deg(v)))
     */
    out_descriptor add(vertex_descriptor v, edge_properties const& ep)
    {
        std::pair<iterator, bool> i = _edges.insert(std::make_pair(v, std::make_pair(ep, in_descriptor())));
        return i.second ? make_descriptor(_edges, i.first) : out_descriptor();
    }

    /**
     * Find the edge with the given vertex descriptor.
     * @complexity O(log(deg(v)))
     */
    out_descriptor find(vertex_descriptor v) const
    { return make_descriptor(_edges, _edges.find(v)); }

    /**
     * Remove the edge with the given vertex descriptor.
     * @complexity O(log(deg(u)))
     */
    void remove(out_descriptor d)
    { _edges.erase(make_iterator(_edges, d)); }

    /** Remove all edges. */
    void clear()
    { _edges.clear(); }

    /** Get the number of outgoing edges. */
    inline size_type size() const
    { return _edges.size(); }

    /** Returns true if there are no out edges. */
    inline bool empty() const
    { return _edges.empty(); }

    /** @name Iterators */
    //@{
    inline iterator begin() const
    { return _edges.begin(); }

    inline iterator end() const
    { return iterator(); }
    //@}

private:
    mutable store_type _edges;
};

#endif
