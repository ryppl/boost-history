
#ifndef IN_VECTOR_HPP
#define IN_VECTOR_HPP

#include <vector>
#include <algorithm>

#include <boost/descriptors.hpp>
#include <boost/graphs/utility.hpp>

/**
 * The in-edge vector references incoming edges from other vertices. Each edge
 * can be uniquely identified by its source vertex and property descriptor.
 *
 * @param Edge A pair describing a vertex descriptor and out edgedescriptor.
 * @param Alloc The allocator for edge pairs.
 */
template <typename Edge, typename Alloc>
class in_vector
{
public:
    typedef typename Edge::first_type vertex_descriptor;
    typedef typename Edge::second_type out_descriptor;

    typedef std::vector<Edge, Alloc> store_type;
    typedef typename store_type::iterator iterator;
    typedef typename store_type::size_type size_type;

    typedef typename descriptor_traits<store_type>::descriptor_type in_descriptor;

    // Constructor
    inline in_vector()
        : _edges()
    { }

    /**
     * Add the edge to the vector, returning an iterator to the added element.
     * @complexity O(1)
     */
    inline in_descriptor add(vertex_descriptor e)
    {
        iterator i = _edges.insert(_edges.end(), std::make_pair(e, out_descriptor()));
        return make_descriptor(_edges, i);
    }

    /**
     * Find the edge whose source originates at the given vertex descriptor.
     * @complexity O(d)
     */
    inline in_descriptor find(vertex_descriptor v) const
    {
        iterator i = std::find_if(_edges.begin(), _edges.end(), find_first(v));
        return make_descriptor(_edges, i);
    }

    /** Get the number of incoming edges (in degree). */
    inline size_type size() const
    { return _edges.size(); }

    /** Returns true if there are no in edges. */
    inline bool empty() const
    { return _edges.empty(); }

private:
    mutable store_type _edges;
};

#endif
