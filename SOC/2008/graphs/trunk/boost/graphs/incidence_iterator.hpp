
#ifndef INCIDENCE_ITERATOR_HPP
#define INCIDENCE_ITERATOR_HPP

#include <boost/type_traits.hpp>

#include "undirected_edge.hpp"

/**
 * The incidence iterator is an abstraction over the incidence iterators of
 * a vertice's incidence store. Specifically when dereferenced, these iterators
 * will result in edge descriptors.
 */
template <typename Iter>
class incidence_iterator
{
    typedef Iter base_iterator;
    typedef typename base_iterator::value_type base_value_type;
public:
    typedef typename base_value_type::first_type vertex_descriptor;
    typedef typename base_value_type::second_type property_descriptor;

    // This is a little misleading. This iterator can be either bidi or random.
    // Clearly, we're going to be constraining members using some concept stuff
    // when it becomes available.
    typedef typename base_iterator::iterator_category iterator_category;

    // Because incidence sets are built on maps, the key type of an incidence
    // iterator is given as const key - which causes edges to be instantiated
    // over constant descriptors (which, perhaps, they should be). For now,
    // just remove the const and get on with things.
    typedef undirected_edge<
            typename boost::remove_const<vertex_descriptor>::type,
            property_descriptor
        > value_type;
    typedef value_type reference;
    typedef value_type pointer;

    inline incidence_iterator()
        : _base(), _iter()
    { }

    inline incidence_iterator(incidence_iterator const& x)
        : _base(x._base), _iter(x._iter)
    { }

    inline incidence_iterator(vertex_descriptor v, base_iterator const& x)
        : _base(v), _iter(x)
    { }

    /**
     * Extended iterator functionality. Return the source vertex of the
     * iterator. This is the vertex for which the iterator was originally
     * created.
     */
    vertex_descriptor source() const
    { return _base; }

    /**
     * Extended iterator functionality. Return the opposite vertex of the
     * edge indicated by the iterator. This is mostly provided for use by
     * the adjacency iterator.
     */
    vertex_descriptor opposite() const
    { return _iter->first; }

    inline incidence_iterator& operator++()
    { ++_iter; return *this; }

    inline incidence_iterator& operator--()
    { --_iter; return *this; }

    // Iterators are equivalent if they reference the same edge.
    inline bool operator==(incidence_iterator const& x) const
    { return operator*() == *x; }

    inline bool operator!=(incidence_iterator const& x) const
    { return !this->operator==(x); }

    inline reference operator*() const
    { return reference(_base, _iter->first, _iter->second); }

private:
    vertex_descriptor   _base;
    base_iterator       _iter;
};


#endif
