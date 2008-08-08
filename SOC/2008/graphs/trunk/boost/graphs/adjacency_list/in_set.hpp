
#ifndef IN_SET_HPP
#define IN_SET_HPP

#include <map>

#include <boost/descriptors.hpp>

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
    typedef typename Edge::first_type vertex_descriptor;
    typedef typename Edge::second_type out_descriptor;

    typedef std::map<vertex_descriptor, out_descriptor, Compare, Alloc> store_type;
    typedef typename store_type::iterator iterator;
    typedef typename store_type::size_type size_type;

    typedef typename descriptor_traits<store_type>::descriptor_type in_descriptor;

    // Constructor
    inline in_set()
        : _edges()
    { }

    /**
     * Try to add the given vertex to the result set.
     * @complexity O(lg(d))
     */
    insertion_result<in_descriptor> add(vertex_descriptor v, out_descriptor o)
    {
        std::pair<iterator, bool> i = _edges.insert(std::make_pair(v, o));
        return make_result(_edges, i);
    }

    /**
     * Find the edge whose source originates at the given vertex descriptor.
     * @complexity O(lg(d))
     */
    in_descriptor find(vertex_descriptor v) const
    { return make_descriptor(_edges, _edges.find(v)); }

    /**
     * Remove the edge with the given descriptor.
     * @complexity O(lg(d))
     */
    void remove(in_descriptor d)
    { _edges.erase(make_iterator(_edges, d)); }

    /** Remove all edges. */
    inline void clear()
    { _edges.clear(); }

    /** Get the number of incoming edges (in degree). */
    inline size_type size() const
    { return _edges.size(); }

    /** Return true if there are no in edges. */
    inline bool empty() const
    { return _edges.empty(); }

    /** @name Iterators */
    //@{
    inline iterator begin() const
    { return _edges.begin(); }

    inline iterator end() const
    { return _edges.end(); }
    //@}

    /** Return the source vertex of the in edge. */
    inline vertex_descriptor source(in_descriptor i) const
    { return make_iterator(_edges, i)->first; }

    /** Return the reverse edge descriptor bound to the in edge. */
    inline out_descriptor out_edge(in_descriptor i) const
    { return make_iterator(_edges, i)->second; }

    /** Return a desecriptor for an iterator into this object. */
    inline in_descriptor in_edge(iterator i) const
    { return make_descriptor(_edges, i); }

private:
   mutable store_type _edges;
};

#endif
