
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_ADJACENCY_ITERATOR_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_ADJACENCY_ITERATOR_HPP

#include <iterator>

namespace boost {
namespace graphs {
namespace adj_list {

/**
 * The adjacency iterator is an abstraction over incidence iterators (provided
 * by the vertex edge store or an incidence sets). The adjacency iterator
 * actually provides the same functionality as the incidence iterator, but
 * simply provides access to the other vertices rather than the edges.
 *
 * @todo Depending on the underlying incidence iterator provided by the graph,
 * we could actually possibly make this a random access iterator rather than
 * just a bidirectional iterator. For now, this is simply a bidi iterator.
 */
template <typename Graph>
class adjacency_iterator
{
    typedef typename Graph::incidence_iterator iterator;
    typedef typename Graph::vertex_descriptor vertex_descriptor;
    typedef typename Graph::edge_descriptor edge_descriptor;
public:

    typedef std::bidirectional_iterator_tag iterator_category;
    typedef std::size_t difference_type;
    typedef vertex_descriptor value_type;
    typedef vertex_descriptor reference;
    typedef vertex_descriptor pointer;

    // Constructors
    inline adjacency_iterator();
    inline adjacency_iterator(Graph const* g, vertex_descriptor v, iterator i);

    inline adjacency_iterator& operator=(adjacency_iterator const& x);
    inline adjacency_iterator& operator++();
    inline adjacency_iterator& operator--();

    inline bool operator==(adjacency_iterator const& x) const;
    inline bool operator!=(adjacency_iterator const& x) const;

    reference operator*();

private:
    Graph const* graph;
    vertex_descriptor vertex;
    iterator iter;
};

// Functions

template <typename G>
adjacency_iterator<G>::adjacency_iterator()
    : graph(0)
    , vertex()
    , iter()
{ }

template <typename G>
adjacency_iterator<G>::adjacency_iterator(G const* g, vertex_descriptor v, iterator i)
    : graph(g)
    , vertex(v)
    , iter(i)
{ }

template <typename G>
adjacency_iterator<G>&
adjacency_iterator<G>::operator=(adjacency_iterator const& x)
{
    graph = x.graph;
    vertex = x.vertex;
    iter = x.iter;
    return *this;
}

template <typename G>
adjacency_iterator<G>&
adjacency_iterator<G>::operator++()
{
    ++iter;
    return *this;
}

template <typename G>
adjacency_iterator<G>&
adjacency_iterator<G>::operator--()
{
    --iter;
    return *this;
}

template <typename G>
bool
adjacency_iterator<G>::operator==(adjacency_iterator const& x) const
{
    return (graph == x.graph) && (vertex == x.vertex) && (iter == x.iter);
}

template <typename G>
bool
adjacency_iterator<G>::operator!=(adjacency_iterator const& x) const
{
    return !operator==(x);
}

template <typename G>
typename adjacency_iterator<G>::reference
adjacency_iterator<G>::operator*()
{
    typename G::edge_type const& e = graph->edge(*iter);
    return e.opposite(vertex);
}

} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */

#endif

