
#ifndef IN_SET_HPP
#define IN_SET_HPP

#include <map>

#include "mapped_iterator.hpp"

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
private:
    typedef typename Edge::second_type out_edge_place;
    typedef std::map<
            vertex_descriptor,
            std::pair<vertex_descriptor const, out_edge_place>,
            Compare, Alloc
        > store_type;
public:
    typedef mapped_iterator<typename store_type::iterator> iterator;
    typedef typename store_type::size_type size_type;

    in_set()
        : _edges()
    { }

    /**
     * Add the edge to the set.
     * @complexity O(deg(u))
     */
    iterator add(vertex_descriptor v)
    {
        return _edges.insert(std::make_pair(v,
                                std::make_pair(v, out_edge_place()))
                            ).first;
    }

    /** Find the edge with the given vertex. */
    iterator find(vertex_descriptor v)
    { return _edges.find(v); }

    /**
     * Find the edge with the given vertex. Is this function ever called?
     */
    iterator find(vertex_descriptor v) const
    { return _edges.find(v); }

    /**
     * Remove the edge with the given descriptor.
     * @complexity O(log(in-deg(v)))
     */
    void remove(iterator i)
    { _edges.erase(i.iter); }

    /** Remove all edges. */
    void clear()
    { _edges.clear(); }

    /** Get the number of incoming edges (in degree). */
    size_type size() const
    { return _edges.size(); }

    /** @name Iterators */
    //@{
    inline iterator begin() const
    { return _edges.begin(); }

    inline iterator end() const
    { return _edges.end(); }
    //@}

private:
   mutable  store_type _edges;
};

#endif
