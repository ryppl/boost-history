
#ifndef OUT_LIST_HPP
#define OUT_LIST_HPP

#include <list>

#include "out_descriptor.hpp"
#include "select.hpp"

/**
 * The out list implements list-based, out-edge storage for directed graphs.
 * out-edges are uniquely identified by their target vertex and property
 * descriptor. List-based stores support fast inserts, slow finds, but do allow
 * removals.
 *
 * The edge is required to be a pair containing a vertex descriptor and a edge
 * property (not descriptor). This type defines the out_descriptor - an opaque
 * reference to a target/property pair.
 *
 * @param Edge A pair describing a vertex descriptor and the edge properties.
 * @param Alloc The allocator for edge pairs.
 */
template <typename Edge, typename Alloc>
class out_list
{
    typedef std::list<Edge, Alloc> store_type;
public:
    typedef Edge out_pair;
    typedef typename Edge::first_type vertex_descriptor;
    typedef typename Edge::second_type edge_properties;

    typedef typename store_type::iterator iterator;
    typedef typename store_type::const_iterator const_iterator;
    typedef typename store_type::size_type size_type;

    typedef basic_out_descriptor<out_pair> out_descriptor;

    inline out_list()
        : _edges()
        , _size(0)
    { }

    /** Allow an edge insertion? */
    std::pair<const_iterator, bool> allow(vertex_descriptor v) const
    { return std::make_pair(_edges.end(), true); }

    /** Add the edge to the list. */
    out_descriptor add(out_pair e)
    {
        ++_size;
        _edges.push_back(e);
        return _edges.back();
    }

    /** Find the edge with the given vertex. */
    iterator find(vertex_descriptor v)
    {
        iterator i = _edges.begin(), end = _edges.end();
        for( ; i != end; ++i) {
            if(i->first == v) return i;
        }
        return end;
    }

    /** Find the edge with the given vertex. */
    const_iterator find(vertex_descriptor v) const
    {
        const_iterator i = _edges.begin(), end = _edges.end();
        for( ; i != end; ++i) {
            if(i->first == v) return i;
        }
        return end;
    }

    /** Remove the edge with the given vertex. */
    void remove(vertex_descriptor v)
    {
        --_size;
        _edges.erase(find(v));
    }

    /** Remove all edges. */
    void clear()
    { _edges.clear(); }

    /** @name Iterators and Size */
    //@{
    inline const_iterator begin() const
    { return _edges.begin(); }

    inline const_iterator end() const
    { return _edges.end(); }

    /** Get the number of outgoing edges. */
    inline size_type size() const
    { return _size; }
    //@{

private:
    store_type  _edges;
    size_type   _size;
};

#endif
