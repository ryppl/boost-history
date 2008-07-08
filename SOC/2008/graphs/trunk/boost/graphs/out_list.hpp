
#ifndef OUT_LIST_HPP
#define OUT_LIST_HPP

#include <list>
#include <algorithm>

#include <boost/triple.hpp>
#include <boost/descriptors.hpp>
#include <boost/graphs/utility.hpp>

/**
 * The out list implements list-based, out-edge storage for directed graphs.
 * out-edges are uniquely identified by their target vertex and property
 * descriptor. List-based stores support fast inserts, slow finds, but do allow
 * removals.
 *

 * @param Edge A tuple describing a vertex descriptor and the edge properties.
 * @param Alloc The allocator for edge pairs.
 */
template <typename Edge, typename Alloc>
class out_list
{
public:
    typedef typename Edge::first_type vertex_descriptor;
    typedef typename Edge::second_type edge_properties;
    typedef typename Edge::third_type in_descriptor;

    typedef std::list<Edge, Alloc> store_type;
    typedef typename store_type::iterator iterator;
    typedef typename store_type::size_type size_type;

    typedef typename descriptor_traits<store_type>::descriptor_type out_descriptor;

    inline out_list()
        : _edges(), _size(0)
    { }

    /**
     * Add the edge to the list.
     * @complexity O(1)
     */
    out_descriptor add(vertex_descriptor v, edge_properties const& ep)
    {
        ++_size;
        iterator i = _edges.insert(_edges.end(), make_triple(v, ep, in_descriptor()));
        return make_descriptor(_edges, i);
    }

    /**
     * Remove the edge with the given vertex. Return the next iterator in
     * the sequence.
     * @complexity O(1)
     */
    iterator remove(out_descriptor d)
    {
        --_size;
        return _edges.erase(make_iterator(_edges, d));
    }

    /**
     * Find the edge with the given vertex.
     * @complexity O(d)
     */
    out_descriptor find(vertex_descriptor v) const
    {
        iterator i = std::find_if(_edges.begin(), _edges.end(), find_first(v));
        return make_descriptor(_edges, i);
    }

    /** Remove all incoming edges from the list, resetting the size to 0. */
    void clear()
    {
        _size = 0;
        _edges.clear();
    }

    /** Get the number of outgoing edges. */
    inline size_type size() const
    { return _size; }

    /** Returns true if there are not out edges. */
    inline bool empty() const
    { return _edges.empty(); }


    /** @name Iterators and Size */
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
