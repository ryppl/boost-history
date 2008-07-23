
#ifndef BREADTH_FIRST_EDGE_ITERATOR_HPP
#define BREADTH_FIRST_EDGE_ITERATOR_HPP

namespace detail
{
    // Edges returned by edge iterators are best used in a directed sense. This
    // because the walk flows from one vertex to the next and it is important
    // to push this data to the user. However, undirected graphs don't have
    // any notion of edge direction so we have to fake it by using rooted
    // edges. A little extra space and time, but a viable solution.
    template <typename Graph>
    struct choose_rooted_edge
    {
        typedef typename Graph::edge_descriptor type;
    };

    template <typename VP, typename EP, typename VS, typename ES>
    struct choose_rooted_edge<undirected_graph<VP,EP,VS,ES>>
    {
        typedef rooted_undirected_edge<
            typename undirected_graph<VP,EP,VS,ES>::edge_descriptor
        > type;
    };

    // Helper functions for creating rooted edges. The default is to simply
    // return the given edge, and does not use the source vertex.
    template <typename Edge, typename Vertex>
    inline Edge
    make_rooted_edge(Edge const& e, Vertex)
    { return e; }

    template <typename Vertex, typename Prop>
    inline rooted_undirected_edge<undirected_edge<Vertex,Prop>>
    make_rooted_edge(undirected_edge<Vertex,Prop> const& e, Vertex src)
    { return rooted_undirected_edge<undirected_edge<Vertex,Prop>>(e, src); }
}

/**
 * Provide an iterator abstraction over the walk. Dereferencing a walk iterator
 * returns the walk itself.
 *
 * This is kind of a strange iterator in the sense that it doesn't support
 * post-increment operations. That would require making a copy of underlying
 * algorithm, which would be very, very expensive. Is this a basic iterator
 * concept?
 */
template <typename Walk>
struct breadth_first_edge_iterator
{
    typedef Walk walk_type;
    typedef typename Walk::graph_type Graph;

    typedef std::forward_iterator_tag iterator_category;
    typedef std::size_t difference_type;
    typedef typename detail::choose_rooted_edge<Graph>::type value_type;
    typedef value_type reference;

    // Construct an end iterator.
    breadth_first_edge_iterator()
        : walk()
    { }

    // Explicitly build an iterator over the state of the given walk.
    breadth_first_edge_iterator(walk_type* walk)
        : walk(walk)
    { }

    inline breadth_first_edge_iterator& operator++()
    { walk->next(); return *this; }

    // Return true if this iterator is past the end. An iterator is past the
    // end if a) the walk pointer is null or b) the current edge is null.
    bool end() const
    { return !walk || (walk && !walk->edge()); }

    // Two iterators are equivalent if they're both at the end or both refer
    // to the same edge.
    inline bool operator==(breadth_first_edge_iterator const& x) const
    {
        return (!walk && x.end())
            || (!x.walk && end())
            || (walk && x.walk && walk->edge() == x.walk->edge());
    }

    inline bool operator!=(breadth_first_edge_iterator const& x) const
    { return !operator==(x); }

    inline reference operator*() const
    { return detail::make_rooted_edge(walk->edge(), walk->source()); }

    inline walk_type const* operator->() const
    { return walk; }

    walk_type* walk;
};

#endif
