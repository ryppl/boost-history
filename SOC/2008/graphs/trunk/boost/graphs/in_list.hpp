
#ifndef IN_LIST_HPP
#define IN_LIST_HPP

#include <list>
#include <algorithm>

#include <boost/descriptors.hpp>
#include <boost/graphs/utility.hpp>

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
    inline in_descriptor add(vertex_descriptor v)
    {
        ++_size;
        iterator i = _edges.insert(_edges.end(), std::make_pair(v, out_descriptor()));
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

    /**
     * Remove the edge referenced by the given iterator.
     * @complexity O(1)
     */
    inline void remove(in_descriptor d)
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

private:
    mutable store_type  _edges;
    size_type           _size;
};

#endif
