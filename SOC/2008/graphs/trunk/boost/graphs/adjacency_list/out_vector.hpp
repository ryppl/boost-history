
#ifndef OUT_VECTOR_HPP
#define OUT_VECTOR_HPP

#include <vector>
#include <algorithm>

#include <boost/triple.hpp>
#include <boost/descriptors.hpp>
#include <boost/graphs/utility.hpp>

/**
 * The in/out vector implements vector-based, edge storage for directed graphs.
 * Each out edge is capable of referencing its corresponding in edge in another
 * vertex and vice-versa.
 *
 * @param Edge A tuple describing the out edge type.
 * @param Alloc The allocator for edge pairs.
 */
template <typename Edge, typename Alloc>
class out_vector
{
public:
    typedef typename Edge::first_type vertex_descriptor;
    typedef typename Edge::second_type edge_pair;
    typedef typename edge_pair::first_type edge_properties;
    typedef typename edge_pair::second_type in_descriptor;

    typedef std::vector<Edge, Alloc> store_type;
    typedef typename store_type::iterator iterator;
    typedef typename store_type::size_type size_type;

    typedef typename descriptor_traits<store_type>::descriptor_type out_descriptor;

    // Constructor
    inline out_vector()
        : _edges()
    { }

    /**
     * Add the edge to the vector.
     * @complexity O(1)
     */
    insertion_result<out_descriptor> add(vertex_descriptor v, edge_properties const& ep)
    {
        iterator i = _edges.insert(_edges.end(), std::make_pair(v, std::make_pair(ep, in_descriptor())));
        return make_result(make_descriptor(_edges, i));
    }

    /** Find the edge with the given vertex. */
    out_descriptor find(vertex_descriptor v) const
    {
        iterator i = std::find_if(_edges.begin(), _edges.end(), find_first(v));
        return make_descriptor(_edges, i);
    }

    /** Get the number of outgoing edges. */
    inline size_type size() const
    { return _edges.size(); }

    /** Return true if there are no out edges. */
    inline bool empty() const
    { return _edges.empty(); }

    /** @name Iterators */
    //@{
    inline iterator begin() const
    { return _edges.begin(); }

    inline iterator end() const
    { return _edges.end(); }
    //@}

    /** Bind the edge to the corresponding in edge descriptor. */
    inline void bind(out_descriptor o, in_descriptor i)
    { make_iterator(_edges, o)->second.second = i; }

    /** Return the target vertex of this edge. */
    inline vertex_descriptor target(out_descriptor o) const
    { return make_iterator(_edges, o)->first; }


    /** Return the properties stored with this edge. */
    inline edge_properties& properties(out_descriptor o)
    { return make_iterator(_edges, o)->second.first; }

    inline edge_properties const& properties(out_descriptor o) const
    { return make_iterator(_edges, o)->second.first; }


    /** Return the in edge descriptor bound to this edge. */
    inline in_descriptor in_edge(out_descriptor o) const
    { return make_iterator(_edges, o)->second.second; }

    /** Return an out descriptor for the given iterator. */
    inline out_descriptor out_edge(iterator i) const
    { return make_descriptor(_edges, i); }

private:
    mutable store_type _edges;
};

#endif
