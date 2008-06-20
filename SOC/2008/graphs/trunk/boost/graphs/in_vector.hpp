
#ifndef IN_VECTOR_HPP
#define IN_VECTOR_HPP

#include <vector>

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
    typedef std::vector<Edge, Alloc> store_type;
public:
    typedef Edge in_pair;
    typedef typename Edge::first_type vertex_descriptor;
    typedef typename Edge::second_type edge_descriptor;

    typedef typename store_type::const_iterator const_iterator;
    typedef typename store_type::size_type size_type;

    in_vector()
        : _edges()
    { }

    /** Add the edge to the vector. */
    void add(in_pair e)
    { _edges.push_back(e); }

    /** Get the number of incoming edges (in degree). */
    size_type size() const
    { return _edges.size(); }

private:
    store_type _edges;
};

#endif
