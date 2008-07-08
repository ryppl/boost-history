
#ifndef INCIDENCE_LIST_HPP
#define INCIDENCE_LIST_HPP

#include <list>
#include <algorithm>

/**
 * The incidence vector stores incident "edges" of a vertex. In actuality,
 * this stores pairs containing an adjacent vertex descriptor and a property
 * descriptor, that points to the edges global properties. This pair uniquely
 * identifies incident edges of the vertex.
 *
 * This type allows constant time insertions, and linear search and remove.
 */
template <typename IncEdge, typename Alloc>
class incidence_list
{
    typedef std::list<IncEdge, Alloc> store_type;
public:
    typedef IncEdge incidence_pair;
    typedef typename IncEdge::first_type vertex_descriptor;
    typedef typename IncEdge::second_type property_descriptor;

    typedef typename store_type::iterator iterator;
    typedef typename store_type::size_type size_type;

    // Constructors
    incidence_list()
        : _edges()
        , _size(0)
    { }

    /**
     * Incidence lists always allow the addition of edges, assuming that no policy
     * conflicts exist. The first element of the return is the end() of the list.
     * @complexity O(1)
     */
    inline std::pair<iterator, bool> allow(vertex_descriptor) const
    { return make_pair(_edges.end(), true); }

    /**
     * Add a vertex to the list.
     * @complexity O(1)
     */
    inline iterator add(incidence_pair p)
    {
        ++_size;
        return _edges.insert(_edges.end(), p);
    }

    /**
     * Find the given incidence pair in the vertex.
     * @todo Do we need this function?
     * @complexity O(1)
     */
    inline iterator find(incidence_pair p) const
    { return std::find(_edges.begin(), _edges.end(), p); }

    /** Find the edge with the given vertex. */
    inline iterator find(vertex_descriptor v) const
    {
        // TODO How do I write this with std::find?
        iterator i = _edges.begin(), end = _edges.end();
        for( ; i != end; ++i) {
            if(i->first == v) return i;
        }
        return end;
    }

    /**
     * Remove the given incidence pair in this vertex.
     * @complexity O(deg(v))
     */
    inline void remove(incidence_pair p)
    { remove(find(p)); }

    /**
     * Remove the iterator.
     * @complexity O(1)
     */
    inline iterator remove(iterator i)
    {
        --_size;
        return _edges.erase(i);
    }

    /** Remove all edges from the vertex. */
    inline void clear()
    {
        _size = 0;
        _edges.clear();
    }

    inline size_type size() const
    { return _size; }

    inline iterator begin() const
    { return _edges.begin(); }

    inline iterator end() const
    { return _edges.end(); }

private:
    mutable store_type  _edges;
    size_type           _size;
};

#endif
