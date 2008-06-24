
#ifndef IN_LIST_HPP
#define IN_LIST_HPP

#include <list>

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
    typedef std::list<Edge, Alloc> store_type;
public:
    typedef Edge in_pair;
    typedef typename Edge::first_type vertex_descriptor;
private:
    typedef typename Edge::second_type out_edge_place;
public:
    typedef typename store_type::iterator iterator;
    typedef typename store_type::size_type size_type;

    in_list()
        : _edges()
        , _size(0)
    { }

    /**
     * Add the edge to list.
     * @complexity O(1)
     */
    iterator add(vertex_descriptor v)
    {
        ++_size;
        _edges.push_back(make_pair(v, out_edge_place()));
        return boost::prior(_edges.end());
    }

    /**
     * Remove the edge referenced by the given iterator.
     * @complexity O(1)
     */
    void remove(iterator i)
    {
        --_size;
        _edges.erase(i);
    }

    /**
     * Find the edge with the given iterator.
     * @complexity O(deg(u))
     */
    iterator find(vertex_descriptor v) const
    {
        iterator i = _edges.begin(), end = _edges.end();
        for( ; i != end; ++i) {
            if(i->first == v) return i;
        }
        return end;
    }

    /** Remove all incoming edges from the list, resetting the size to 0. */
    void clear()
    {
        _size = 0;
        _edges.clear();
    }

    /** Get the number of incoming edges (in degree). */
    size_type size() const
    { return _size; }

    /** @name Iterators */
    //@{
    iterator begin() const
    { return _edges.begin(); }

    iterator end() const
    { return _edges.end(); }
    //@}

private:
    mutable store_type  _edges;
    size_type           _size;
};

#endif
