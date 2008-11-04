
#ifndef BOOST_GRAPHS_ADJLIST_OUT_ITERATOR_HPP
#define BOOST_GRAPHS_ADJLIST_OUT_ITERATOR_HPP

namespace boost { namespace graphs { namespace adjacency_list {

/**
 * The out edge iterator is a wrapper around out store iterators that, when
 * dereferenced will return an edge descriptor.
 *
 * @note This is unfortunately parameterized over the entire vertex type. Why?
 * Basically because we can't tranlstate between the underlying iterator and
 * its own descriptor without access to its store. Unfortunately, the store
 * that defines the underlying iterator is buried beneath the vertex, preventing
 * easy construction from the outer graph class. Why o' why do I not have self-
 * transflating descriptors? It could greatly simplify some aspects of these
 * implementations.
 */
template <typename Vertex, typename Edge>
class basic_out_iterator
{
public:
    // Decode some type information from the wrapped out iterator.
    typedef Vertex vertex_type;
    typedef typename Vertex::out_iterator iterator;
    typedef typename iterator::value_type base_value_type;
    typedef typename boost::remove_const<typename base_value_type::first_type>::type vertex_descriptor;
    typedef typename base_value_type::second_type edge_pair;
    typedef typename edge_pair::first_type edge_label;
    typedef typename edge_pair::second_type in_descriptor;

    // This is a little misleading. This iterator can be either bidi or random.
    // Clearly, we're going to be constraining members using some concept stuff
    // when it becomes available.
    typedef typename iterator::iterator_category iterator_category;
    typedef typename iterator::difference_type difference_type;
    typedef Edge value_type;
    typedef value_type reference;
    typedef value_type pointer;

    inline basic_out_iterator()
        : vert(0), src(), iter()
    { }

    inline basic_out_iterator(vertex_type& vert, vertex_descriptor v, iterator x)
         : vert(&vert), src(v), iter(x)
    { }

    /** Return the source vertex of the iterated edge. */
    inline vertex_descriptor source() const
    { return src; }

    /** Return the target vertex of the iterated edge. */
    inline vertex_descriptor target() const
    { return iter->first; }

    /**
     * Return the "opposite" (target) vertex of the iterated edge. This is
     * provided to work with the adjacency iterator.
     */
    inline vertex_descriptor opposite() const
    { return target(); }

    inline basic_out_iterator& operator=(basic_out_iterator const& x)
    { vert = x.vert; src = x.src; iter = x.iter; return *this; }

    inline basic_out_iterator& operator++()
    { ++iter; return *this; }

    inline basic_out_iterator& operator--()
    { --iter; return *this; }

    /** @name Equality Comparable */
    //@{
    inline bool operator==(basic_out_iterator const& x) const
    { return iter == x.iter; }

    inline bool operator!=(basic_out_iterator const& x) const
    { return iter != x.iter; }
    //@}

    inline reference operator*() const
    { return Edge(source(), target(), vert->out_edge(iter)); }

    vertex_type*        vert;
    vertex_descriptor   src;
    iterator            iter;
};

} } } /* namespace boost::graphs::adjacency_list */

#endif
