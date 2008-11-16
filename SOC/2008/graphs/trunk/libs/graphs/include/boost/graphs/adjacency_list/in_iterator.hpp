
#ifndef BOOST_GRAPHS_ADJLIST_IN_ITERATOR_HPP
#define BOOST_GRAPHS_ADJLIST_IN_ITERATOR_HPP

namespace boost { namespace graphs { namespace adjacency_list {

/**
 * The in edge iterator is a wrapper around out store iterators that, when
 * dereferenced will return an edge descriptor. The iterator category is taken
 * from the base iterator. The in edge iterator also needs to know the type
 * of the out edge iterator since in edges contain placeheld references to
 * out edges.
 */
template <typename Iter, typename Edge>
class basic_in_iterator
{
public:
    typedef Iter iterator;
    typedef typename Iter::value_type base_value_type;
    typedef typename base_value_type::first_type vertex_descriptor;
    typedef typename base_value_type::second_type out_descriptor;

    typedef typename iterator::iterator_category iterator_category;
    typedef typename iterator::difference_type difference_type;
    typedef Edge value_type;
    typedef value_type reference;
    typedef value_type pointer;

    /** @name Constructors */
    //@{
    inline basic_in_iterator()
        : tgt(), iter()
    { }

    inline basic_in_iterator(vertex_descriptor v, iterator x)
        : tgt(v), iter(x)
    { }
    //@}

    /** Return the source vertex of the iterated edge. */
    vertex_descriptor source() const
    { return iter->first; }

    /** Return the target vertex of the iterated edge. */
    vertex_descriptor target() const
    { return tgt; }

    inline basic_in_iterator& operator++()
    { ++iter; return *this; }

    inline basic_in_iterator& operator--()
    { --iter; return *this; }

    inline bool operator==(basic_in_iterator const& x) const
    { return iter == x.iter; }

    inline bool operator!=(basic_in_iterator const& x) const
    { return iter != x.iter; }

    inline reference operator*() const
    { return Edge(source(), target(), iter->second); }

    vertex_descriptor   tgt;
    iterator            iter;
};

} } } /* namespace boost::graphs::adjacency_list */

#endif
