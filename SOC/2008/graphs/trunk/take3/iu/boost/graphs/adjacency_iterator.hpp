
#ifndef ADJACENCY_ITERATOR_HPP
#define ADJACENCY_ITERATOR_HPP

#include <iterator>

/**
 * The adjacency iterator is an abstraction over incidence iterators (provided
 * by the incidence store). The adjacency iterator actually provides the same
 * functionality as the incidence iterator, but simply provides access to the
 * other vertices rather than the edges.
 *
 * @todo Depending on the underlying incidence iterator provided by the graph,
 * we could actually possibly make this a random access iterator rather than
 * just a bidirectional iterator. For now, we force it to be a bidi iterator.
 */
template <typename Iterator>
class adjacency_iterator
{
    typedef Iterator iterator;
    typedef typename Iterator::vertex_descriptor vertex_descriptor;
public:

    typedef std::bidirectional_iterator_tag iterator_category;
    typedef std::size_t difference_type;
    typedef vertex_descriptor value_type;
    typedef vertex_descriptor reference;
    typedef vertex_descriptor pointer;

    // Constructors
    inline adjacency_iterator()
        : _iter()
    { }

    inline adjacency_iterator(Iterator iter)
        : _iter(iter)
    { }

    inline adjacency_iterator& operator=(adjacency_iterator const& x)
    {
        _iter = x._iter;
        return *this;
    }

    inline adjacency_iterator& operator++()
    {
        ++_iter;
        return *this;
    }

    inline adjacency_iterator& operator--()
    {
        --_iter;
        return *this;
    }

    inline bool operator==(adjacency_iterator const& x) const
    {
        return _iter == x._iter;
    }

    inline bool operator!=(adjacency_iterator const& x) const
    {
        return _iter != x._iter;
    }

    reference operator*() const
    {
        // Return the opposite vertex referenced by the underlying iterator.
        // This lets us get away without having to store the source vertex
        // multiple types in the layered iterator.
        return _iter.opposite();
    }

private:
    iterator _iter;
};


#endif

