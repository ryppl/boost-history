
#ifndef INCIDENCE_LIST_HPP
#define INCIDENCE_LIST_HPP

#include <list>
#include <algorithm>

#include <boost/graphs/utility.hpp>

/**
 * The incidence vector stores incident "edges" of a vertex. In actuality,
 * this stores pairs containing an adjacent vertex descriptor and a property
 * descriptor, that points to the edges global label. This pair uniquely
 * identifies incident edges of the vertex.
 *
 * This type allows constant time insertions, and linear search and remove.
 */
template <typename Edge, typename Alloc>
class incidence_list
{
public:
    typedef typename Edge::first_type vertex_descriptor;
    typedef typename Edge::second_type label_descriptor;

    typedef std::list<Edge, Alloc> store_type;
    typedef typename store_type::iterator iterator;
    typedef typename store_type::size_type size_type;

    typedef typename descriptor_traits<store_type>::descriptor_type incidence_descriptor;

    // Constructors
    incidence_list()
        : _edges(), _size(0)
    { }

    /** @name Add Vertex
     * Add a vertex to the list. The first version adds a stub that must be
     * bound to a property descriptor in the second phase.
     * @complexity O(1)
     */
    //@{
    inline insertion_result<incidence_descriptor> add(vertex_descriptor v)
    { return add(v, label_descriptor()); }

    insertion_result<incidence_descriptor> add(vertex_descriptor v, label_descriptor p)
    {
        ++_size;
        iterator i = _edges.insert(_edges.end(), make_pair(v, p));
        return make_result(make_descriptor(_edges, i));
    }
    //@}

    /**
     * Find the given incidence pair in the vertex.
     * @complexity O(1)
     */
    incidence_descriptor find(vertex_descriptor v) const
    {
        iterator i = std::find_if(_edges.begin(), _edges.end(), find_first(v));
        return make_descriptor(_edges, i);
    }

    /**
     * Remove the given incidence pair in this vertex.
     * @complexity O(deg(v))
     */
    void remove(incidence_descriptor d)
    {
        _edges.erase(make_iterator(_edges, d));
        --_size;
    }

    /** Remove all edges from the vertex. */
    void clear()
    {
        _size = 0;
        _edges.clear();
    }

    /** Return the number of edges in this store. */
    inline size_type size() const
    { return _size; }

    /** Return true if this is empty. */
    inline bool empty() const
    { return _edges.empty(); }

    /** @name Iterators */
    //@{
    inline iterator begin() const
    { return _edges.begin(); }

    inline iterator end() const
    { return _edges.end(); }
    //@}

    /** Bind this incident edge to the given property descriptor. */
    inline void bind(incidence_descriptor d, label_descriptor p)
    { make_iterator(_edges, d)->second = p; }

    /** Return the connected vertex referenced by this edge. */
    inline vertex_descriptor vertex(incidence_descriptor d) const
    { return make_iterator(_edges, d)->first; }

    /** Return the property referenced by this edge. */
    inline label_descriptor label(incidence_descriptor d) const
    { return make_iterator(_edges, d)->second; }

private:
    mutable store_type  _edges;
    size_type           _size;
};

#endif
