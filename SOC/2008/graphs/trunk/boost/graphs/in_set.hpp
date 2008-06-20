
#ifndef IN_SET_HPP
#define IN_SET_HPP

#include <map>

/**
 * The in-edge set references incoming edges from other vertices. Each edge
 * can be uniquely identified by its source vertex and property descriptor.
 *
 * @param Edge A pair describing a vertex descriptor and out edge descriptor.
 * @param Alloc The allocator for edge pairs.
 */
template <typename Edge, typename Compare, typename Alloc>
class in_set
{
public:
    typedef Edge in_pair;
    typedef typename Edge::first_type vertex_descriptor;
    typedef typename Edge::second_type out_descriptor;
private:
    typedef std::map<vertex_descriptor, out_descriptor, Compare, Alloc> store_type;
public:
    typedef typename store_type::iterator iterator;
    typedef typename store_type::const_iterator const_iterator;
    typedef typename store_type::size_type size_type;

    in_set()
        : _edges()
    { }

    /**
     * Add the edge to the set.
     * @complexity O(deg(u))
     */
    void add(in_pair e)
    { _edges.insert(e); }

    /** Find the edge with the given vertex. */
    iterator find(vertex_descriptor v)
    { return _edges.find(v); }

    /** Find the edge with the given vertex. */
    const_iterator find(vertex_descriptor v) const
    { return _edges.find(v); }

    /** Remove the edge with the given descriptor. */
    void remove(vertex_descriptor v)
    { _edges.erase(find(v)); }

    /** Remove all edges. */
    void clear()
    { _edges.clear(); }

    /** Get the number of incoming edges (in degree). */
    size_type size() const
    { return _edges.size(); }

private:
    store_type _edges;
};

#endif
