
#ifndef BOOST_GRAPHS_ADJLIST_IN_LIST_HPP
#define BOOST_GRAPHS_ADJLIST_IN_LIST_HPP

#include <list>
#include <algorithm>

#include <boost/descriptors.hpp>
#include <boost/graphs/utility.hpp>
namespace boost { namespace graphs { namespace adjacency_list {

/**
 * The in-edge list references incoming edges from other vertices. Each edge
 * can be uniquely identified by its source vertex and property descriptor.
 *
 * @param Edge A pair describing a vertex descriptor and out edge descriptor.
 * @param Alloc The allocator for edge pairs.
 */
template <typename Edge, typename Alloc>
class in_list
{
public:
    typedef typename Edge::first_type vertex_descriptor;
    typedef typename Edge::second_type out_descriptor;

    typedef std::list<Edge, Alloc> store_type;
    typedef typename store_type::iterator iterator;
    typedef typename store_type::size_type size_type;

    typedef typename descriptor_traits<store_type>::descriptor_type in_descriptor;

    // Constructor
    inline in_list()
        : _edges(), _size(0)
    { }

    /**
     * Add the edge to list.
     * @complexity O(1)
     */
    insertion_result<in_descriptor> add(vertex_descriptor v, out_descriptor o)
    {
        ++_size;
        iterator i = _edges.insert(_edges.end(), std::make_pair(v, o));
        return make_result(make_descriptor(_edges, i));
    }

    /**
     * Find the edge whose source originates at the given vertex descriptor.
     * @complexity O(d)
     */
    in_descriptor find(vertex_descriptor v) const
    {
        iterator i = std::find_if(_edges.begin(), _edges.end(), find_first(v));
        return make_descriptor(_edges, i);
    }

    /**
     * Remove the edge referenced by the given iterator.
     * @complexity O(1)
     */
    void remove(in_descriptor d)
    {
        _edges.erase(make_iterator(_edges, d));
        --_size;
    }

    /** Remove all incoming edges from the list, resetting the size to 0. */
    inline void clear()
    {
        _size = 0;
        _edges.clear();
    }

    /** Get the number of incoming edges (in degree). */
    inline size_type size() const
    { return _size; }

    /** Returns true if there are no in edges. */
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
    mutable store_type  _edges;
    size_type           _size;
};

} } } /* namespace boost::graphs::adjacency_list */

#endif