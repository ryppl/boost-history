
#ifndef OUT_SET_HPP
#define OUT_SET_HPP

#include <map>
#include <memory>

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
 * @param Edge A pair describing a vertex descriptor and the edge label.
 * @param Alloc The allocator for edge pairs.
 */
template <typename Edge, typename Compare, typename Alloc>
class out_set
{
public:
    typedef typename Edge::first_type vertex_descriptor;
    typedef typename Edge::second_type edge_pair;
    typedef typename edge_pair::first_type edge_label;
    typedef typename edge_pair::second_type in_descriptor;

    // Reconstruct the edge triple into a key/value type thing for the map.
    typedef std::map<vertex_descriptor, edge_pair, Compare, Alloc> store_type;
    typedef typename store_type::iterator iterator;
    typedef typename store_type::size_type size_type;

    typedef typename descriptor_traits<store_type>::descriptor_type out_descriptor;

    // Constructor
    inline out_set()
        : _edges()
    { }

    /**
     * Try to add the given edge to the set.
     * @complexity O(log(deg(v)))
     */
    insertion_result<out_descriptor> add(vertex_descriptor v, edge_label const& ep)
    {
        std::pair<iterator, bool> i = _edges.insert(std::make_pair(v, std::make_pair(ep, in_descriptor())));
        return make_result(_edges, i);
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
    { return _edges.end(); }
    //@}

    /** Bind the edge to the corresponding in edge descriptor. */
    inline void bind(out_descriptor o, in_descriptor i)
    { make_iterator(_edges, o)->second.second = i; }

    /** Return the target vertex of this edge. */
    inline vertex_descriptor target(out_descriptor o) const
    { return make_iterator(_edges, o)->first; }

    /** Return the label stored with this edge. */
    inline edge_label& label(out_descriptor o)
    { return make_iterator(_edges, o)->second.first; }

    inline edge_label const& label(out_descriptor o) const
    { return make_iterator(_edges, o)->second.first; }

    /** Return the in edge descriptor bound to this edge. */
    inline in_descriptor in_edge(out_descriptor o) const
    { return make_iterator(_edges, o)->second.second; }

    /** Return an out descriptor for the given iterator. */
    inline out_descriptor out_edge(iterator i) const
    { return make_descriptor(_edges, i); }

private:
    mutable store_type _edges;
};

#endif
