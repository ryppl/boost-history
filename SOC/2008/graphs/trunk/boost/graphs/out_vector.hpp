
#ifndef OUT_VECTOR_HPP
#define OUT_VECTOR_HPP

#include <vector>

#include "out_descriptor.hpp"

/**
 * The out vector implements vector-based, out-edge storage for directed graphs.
 * out-edges are uniquely identified by their target vertex and property
 * descriptor. As an out-edge store, this type stores an edge property with the
 * target vertex descriptor. Vector-based stores support fast inserts, slow
 * finds, and do not allow remove operations.
 *
 * The edge is required to be a pair containing a vertex descriptor and a edge
 * property (not descriptor). This type defines the out_descriptor - an opaque
 * reference to a target/property pair.
 *
 * @param Edge A pair describing a vertex descriptor and the edge properties.
 * @param Alloc The allocator for edge pairs.
 */
template <typename Edge, typename Alloc>
class out_vector
{
    typedef std::vector<Edge, Alloc> store_type;
public:
    typedef Edge out_pair;
    typedef typename Edge::first_type vertex_descriptor;
    typedef typename Edge::second_type edge_properties;

    typedef typename store_type::const_iterator const_iterator;
    typedef typename store_type::size_type size_type;

    typedef basic_out_descriptor<const_iterator> out_descriptor;

    inline out_vector()
        : _edges()
    { }

    /** Allow the addition? */
    std::pair<out_descriptor, bool> allow(vertex_descriptor v)
    { return std::make_pair(_edges.end(), true); }

    /**
     * Add the edge to the vector.
     * @complexity O(1)
     */
    out_descriptor add(out_pair e)
    {
        _edges.push_back(e);
        return out_descriptor(boost::prior(_edges.end()));
    }

    /** Get the number of outgoing edges. */
    inline size_type size() const
    { return _edges.size(); }

    /** @name Iterators */
    //@{
    inline const_iterator begin() const
    { return _edges.begin(); }

    inline const_iterator end() const
    { return _edges.end(); }

    //@{

private:
    store_type _edges;
};

#endif
