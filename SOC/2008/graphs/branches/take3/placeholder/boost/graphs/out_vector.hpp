
#ifndef OUT_VECTOR_HPP
#define OUT_VECTOR_HPP

#include <vector>

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
    typedef std::vector<Edge, Alloc> store_type;
public:
    typedef Edge out_tuple;
    typedef typename Edge::first_type vertex_descriptor;
    typedef typename Edge::second_type edge_properties;
private:
    typedef typename Edge::third_type in_edge_place;
public:
    typedef std::pair<vertex_descriptor, edge_properties> out_pair;
    typedef typename store_type::iterator iterator;
    typedef typename store_type::size_type size_type;

    inline out_vector()
        : _edges()
    { }

    /**
     * Allow the edge addition? Unless policy dictates otherwise, always allow
     * the addition of the edge.
     */
    std::pair<iterator, bool> allow(vertex_descriptor v) const
    { return std::make_pair(_edges.end(), true); }

    /**
     * Add the edge to the vector.
     * @complexity O(1)
     */
    iterator add(vertex_descriptor v, edge_properties const& ep)
    {
        _edges.push_back(out_tuple(v, ep, in_edge_place()));
        return boost::prior(_edges.end());
    }

    /** Find the edge with the given vertex. */
    iterator find(vertex_descriptor v) const
    {
        // TODO How do I write this with std::find?
        iterator i = _edges.begin(), end = _edges.end();
        for( ; i != end; ++i) {
            if(i->first == v) return i;
        }
        return end;
    }

    /** Get the number of outgoing edges. */
    inline size_type size() const
    { return _edges.size(); }

    /** @name Iterators */
    //@{
    inline iterator begin() const
    { return _edges.begin(); }

    inline iterator end() const
    { return _edges.end(); }
    //@}

private:
    mutable store_type _edges;
};

#endif