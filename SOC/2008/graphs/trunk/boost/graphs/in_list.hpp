
#ifndef IN_LIST_HPP
#define IN_LIST_HPP

#include <list>

/**
 * The in-edge list references incoming edges from other vertices. Each edge
 * can be uniquely identified by its source vertex and property descriptor.
 *
 * @param Edge A pair describing a vertex descriptor and out edgedescriptor.
 * @param Alloc The allocator for edge pairs.
 */
template <typename Edge, typename Alloc>
class in_list
{
    typedef std::list<Edge, Alloc> store_type;
public:
    typedef Edge in_pair;
    typedef typename Edge::first_type vertex_descriptor;
    typedef typename Edge::second_type edge_descriptor;

    typedef typename store_type::iterator iterator;
    typedef typename store_type::const_iterator const_iterator;
    typedef typename store_type::size_type size_type;

    in_list()
        : _edges()
    { }

    /**
     * Add the edge to list.
     * @complexity O(1)
     */
    void add(in_pair e)
    { _edges.push_back(e); }

    /**
     * Find the edge with the given iterator.
     * @complexity O(deg(u))
     */
    iterator find(vertex_descriptor v)
    {
        iterator i = _edges.begin(), end = _edges.end();
        for( ; i != end; ++i) {
            if(i->first == v) return i;
        }
        return end;
    }

    /**
     * Find the edge with the given iterator.
     * @complexity O(deg(u))
     */
    const_iterator find(vertex_descriptor v) const
    {
        const_iterator i = _edges.begin(), end = _edges.end();
        for( ; i != end; ++i) {
            if(i->first == v) return i;
        }
        return end;
    }

    /**
     * Remove the edge with the given vertex.
     * @complexity O(deg(u))
     */
    void remove(vertex_descriptor v)
    { _edges.erase(find(v)); }

    void clear()
    { _edges.clear(); }

    /** Get the number of incoming edges (in degree). */
    size_type size() const
    { return _edges.size(); }

private:
    store_type _edges;
};

#endif
