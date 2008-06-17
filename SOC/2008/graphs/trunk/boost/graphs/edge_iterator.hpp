
#ifndef EDGE_ITERATOR_HPP
#define EDGE_ITERATOR_HPP

#include <iterator>

// Edge iteration sucks when there's no global edge store. Basically, we have
// to build a compound iterator that walks over each vertex and over each
// incident edge. Normally, not a big deal. However, we aren't allowed to
// backtrack, which means we have to build some method of "marking" which
// vertices we've already visited.
//
// This would be much easier if the vertex iterators were random because we
// can order them and make that determination in constant time. However, this
// is not the general case.

namespace detail
{
    template <typename Graph, typename Tag>
    class edge_iterator
    {
        typedef typename Graph::vertex_iterator vertex_iterator;
        typedef typename Graph::incident_edge_iterator incidence_iterator;
    public:

        typedef int foo;
    };

    /**
     * This edge iterator is actually the easiest to implement because the
     * iterators can be ordered (i.e,. i \< j => *i < *j).
     */
    template <typename Graph>
    class edge_iterator<Graph, std::random_access_iterator_tag>
    {
        typedef Graph graph_type;
        typedef typename Graph::vertex_iterator vertex_iterator;
        typedef typename Graph::incident_edge_iterator incidence_iterator;
    public:
        typedef std::forward_iterator_tag iterator_category;

        typedef typename incidence_iterator::value_type value_type;
        typedef value_type reference;
        typedef value_type pointer;

        edge_iterator()
            : _graph(0)
            , _vert()
            , _inc()
        { }

        edge_iterator(Graph const* g)
            : _graph(g)
            , _vert(_graph->begin_vertices())
            , _inc(next(_graph->begin_incident_edges(*_vert)))
        { }

        edge_iterator& operator++()
        {
            // If we're not at the last of the incident edges for the current
            // vertex, then we move to the next.
            if(_inc != _graph->end_incident_edges(*_vert)) {
                _inc = next(_inc);
            }
            else {
                ++_vert;
            }
            return *this;
        }

        // Dereferencing is easy. Just dereference the underlying incidence
        // iterator.
        reference operator*() const
        { return *_inc; }

        // Move the the next edge iterator.
        incidence_iterator next(incidence_iterator f)
        {
        }

    private:
        graph_type const*   _graph;
        vertex_iterator     _vert;
        incidence_iterator  _inc;
    };
}

/**
 * The basic edge iterator is responsible for implementing an iterator over
 * the edges in an undirected graph.
 *
 * We actually implement this type through its specialized base classes.
 */
template <typename Graph>
struct basic_edge_iterator
    : detail::edge_iterator<Graph, typename Graph::vertex_iterator::iterator_category>
{
    typedef detail::edge_iterator<Graph, typename Graph::vertex_iterator::iterator_category> base_type;

    basic_edge_iterator()
        : base_type()
    { }

    basic_edge_iterator(Graph* g)
        : base_type(g)
    { }

};


#endif

