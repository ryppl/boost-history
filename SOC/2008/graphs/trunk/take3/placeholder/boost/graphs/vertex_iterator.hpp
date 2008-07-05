
#ifndef VERTEX_ITERATOR_HPP
#define VERTEX_ITERATOR_HPP

// This file is a little gross. Basically, I'm cramming three different classes
// and their implementations into it. This file contains:
// 1. indexed_vertex_iterator
// 2. simple_vertex_iterator
// 3. mapped_vertex_iterator
// Functions for these follow all the class definitions.
// TODO Consider inlining all the functions for some degree of brevity.

/**
 * The indexed vertex iterator provides a vertex iterator for stores whose
 * descriptors cannot be pointers (i.e., vectors). By virtue of the fact that
 * Store is required to be a vector of something, this is a random access
 * iterator.
 */
template <typename Store>
class indexed_vertex_iterator
{
    typedef typename Store::const_iterator iterator;
public:
    typedef typename Store::value_type vertex_type;
    typedef typename vertex_type::vertex_descriptor vertex_descriptor;

    typedef typename iterator::iterator_category iterator_category;
    typedef typename iterator::difference_type difference_type;
    typedef vertex_descriptor value_type;
    typedef vertex_descriptor reference;
    typedef vertex_descriptor pointer;

    // Constructors
    inline indexed_vertex_iterator();
    inline indexed_vertex_iterator(indexed_vertex_iterator const& x);
    inline indexed_vertex_iterator(Store const& s, iterator const& x);

    // Assignment and increment
    inline indexed_vertex_iterator& operator=(indexed_vertex_iterator const& x);
    inline indexed_vertex_iterator& operator+=(difference_type n);
    inline indexed_vertex_iterator& operator-=(difference_type n);
    inline indexed_vertex_iterator& operator++();
    inline indexed_vertex_iterator& operator--();

    inline indexed_vertex_iterator operator+(difference_type n) const;
    inline indexed_vertex_iterator operator-(difference_type n) const;
    inline difference_type operator-(indexed_vertex_iterator const& x) const;

    inline reference operator*();

    inline bool operator==(indexed_vertex_iterator const& x) const;
    inline bool operator!=(indexed_vertex_iterator const& x) const;

private:
    Store const* store;
    iterator iter;
};

/**
 * The value vertex iterator provides a vertex iterator unique associative
 * containers and sequences that don't invalidate memory on insertions
 * (lists).
 */
template <typename Store>
class simple_vertex_iterator
{
    typedef typename Store::const_iterator iterator;
public:
    typedef typename Store::value_type vertex_type;
    typedef typename vertex_type::vertex_descriptor vertex_descriptor;

    typedef typename iterator::iterator_category iterator_category;
    typedef typename iterator::difference_type difference_type;
    typedef vertex_descriptor value_type;
    typedef vertex_descriptor reference;
    typedef vertex_descriptor pointer;

    inline simple_vertex_iterator();
    inline simple_vertex_iterator(simple_vertex_iterator const& x);
    inline simple_vertex_iterator(iterator const& x);

    inline simple_vertex_iterator& operator=(simple_vertex_iterator const& x);
    inline simple_vertex_iterator& operator++();
    inline simple_vertex_iterator& operator--();

    inline reference operator*();

    inline bool operator==(simple_vertex_iterator const& x) const;
    inline bool operator!=(simple_vertex_iterator const& x) const;

private:
    iterator iter;
};


/**
 * The mapped vertex iterator provides a vertex iterator pair associative
 * storage containers. This essintially wraps a store iterator, providing
 * the ability to dererence to descriptors. Because this iterator is taken
 * from pair associative structures, it is bidirectional but not random access.
 */
template <typename Store>
class mapped_vertex_iterator
{
    typedef typename Store::const_iterator iterator;
public:
    typedef typename Store::mapped_type vertex_type;
    typedef typename vertex_type::vertex_descriptor vertex_descriptor;

    typedef typename iterator::iterator_category iterator_category;
    typedef typename iterator::difference_type difference_type;
    typedef vertex_descriptor value_type;
    typedef vertex_descriptor reference;
    typedef vertex_descriptor pointer;

    inline mapped_vertex_iterator();
    inline mapped_vertex_iterator(mapped_vertex_iterator const& x);
    inline mapped_vertex_iterator(iterator const& x);

    inline mapped_vertex_iterator& operator=(mapped_vertex_iterator const& x);
    inline mapped_vertex_iterator& operator++();
    inline mapped_vertex_iterator& operator--();

    inline reference operator*();

    inline bool operator==(mapped_vertex_iterator const& x) const;
    inline bool operator!=(mapped_vertex_iterator const& x) const;

private:
    iterator iter;
};

//
// Indexed Vertex Iterator Functions
//

template <typename S>
indexed_vertex_iterator<S>::indexed_vertex_iterator()
    : store(0)
    , iter()
{ }

template <typename S>
indexed_vertex_iterator<S>::indexed_vertex_iterator(indexed_vertex_iterator const& x)
    : store(x.store)
    , iter(x.iter)
{ }

template <typename S>
indexed_vertex_iterator<S>::indexed_vertex_iterator(S const& s, iterator const& x)
    : store(&s)
    , iter(x)
{ }

template <typename S>
indexed_vertex_iterator<S>&
indexed_vertex_iterator<S>::operator=(indexed_vertex_iterator const& x)
{
    iter = x.iter;
    store = x.store;
    return *this;
}

template <typename S>
indexed_vertex_iterator<S>&
indexed_vertex_iterator<S>::operator+=(difference_type n)
{
    iter += n;
    return *this;
}

template <typename S>
indexed_vertex_iterator<S>&
indexed_vertex_iterator<S>::operator-=(difference_type n)
{
    iter -= n;
    return *this;
}

template <typename S>
indexed_vertex_iterator<S>&
indexed_vertex_iterator<S>::operator++()
{
    ++iter;
    return *this;
}

template <typename S>
indexed_vertex_iterator<S>&
indexed_vertex_iterator<S>::operator--()
{
    --iter;
    return *this;
}

template <typename S>
indexed_vertex_iterator<S>
indexed_vertex_iterator<S>::operator+(difference_type n) const
{
    return iter + n;
}

template <typename S>
indexed_vertex_iterator<S>
indexed_vertex_iterator<S>::operator-(difference_type n) const
{
    return iter - n;
}

template <typename S>
typename indexed_vertex_iterator<S>::difference_type
indexed_vertex_iterator<S>::operator-(indexed_vertex_iterator const& x) const
{
    return iter - x.iter;
}

template <typename S>
typename indexed_vertex_iterator<S>::reference
indexed_vertex_iterator<S>::operator*()
{
    // The returned descriptor is simply the distance from the beginning of
    // the underlying store to the end.
    return std::distance(store->begin(), iter);
}

template <typename S>
bool
indexed_vertex_iterator<S>::operator==(indexed_vertex_iterator const& x) const
{
     return (store == x.store) && (iter == x.iter);
}

template <typename S>
bool
indexed_vertex_iterator<S>::operator!=(indexed_vertex_iterator const& x) const
{
    return (store == x.store) && (iter != x.iter);
}


//
// Simple Vertex Iterator Functions
//

template <typename S>
simple_vertex_iterator<S>::simple_vertex_iterator()
    : iter()
{ }

template <typename S>
simple_vertex_iterator<S>::simple_vertex_iterator(simple_vertex_iterator const& x)
    : iter(x.iter)
{ }

template <typename S>
simple_vertex_iterator<S>::simple_vertex_iterator(iterator const& x)
    : iter(x)
{ }

template <typename S>
simple_vertex_iterator<S>&
simple_vertex_iterator<S>::operator=(simple_vertex_iterator const& x)
{
    iter = x.iter;
    return *this;
}

template <typename S>
simple_vertex_iterator<S>&
simple_vertex_iterator<S>::operator++()
{
    ++iter;
    return *this;
}

template <typename S>
simple_vertex_iterator<S>&
simple_vertex_iterator<S>::operator--()
{
    --iter;
    return *this;
}

template <typename S>
typename simple_vertex_iterator<S>::reference
simple_vertex_iterator<S>::operator*()
{
    return &const_cast<vertex_type&>(*iter);
}

template <typename S>
bool
simple_vertex_iterator<S>::operator==(simple_vertex_iterator const& x) const
{
    return iter == x.iter;
}

template <typename S>
bool
simple_vertex_iterator<S>::operator!=(simple_vertex_iterator const& x) const
{
    return iter != x.iter;
}

//
// Mapped Vertex Iterator Functions

template <typename S>
mapped_vertex_iterator<S>::mapped_vertex_iterator()
    : iter()
{ }

template <typename S>
mapped_vertex_iterator<S>::mapped_vertex_iterator(mapped_vertex_iterator const& x)
    : iter(x.iter)
{ }

template <typename S>
mapped_vertex_iterator<S>::mapped_vertex_iterator(iterator const& x)
    : iter(x)
{ }

template <typename S>
mapped_vertex_iterator<S>&
mapped_vertex_iterator<S>::operator=(mapped_vertex_iterator const& x)
{
    iter = x.iter;
    return *this;
}

template <typename S>
mapped_vertex_iterator<S>&
mapped_vertex_iterator<S>::operator++()
{
    ++iter;
    return *this;
}

template <typename S>
mapped_vertex_iterator<S>&
mapped_vertex_iterator<S>::operator--()
{
    --iter;
    return *this;
}

template <typename S>
typename mapped_vertex_iterator<S>::reference
mapped_vertex_iterator<S>::operator*()
{
    return &const_cast<vertex_type&>(iter->second);
}

template <typename S>
bool
mapped_vertex_iterator<S>::operator==(mapped_vertex_iterator const& x) const
{
    return iter == x.iter;
}

template <typename S>
bool
mapped_vertex_iterator<S>::operator!=(mapped_vertex_iterator const& x) const
{
    return iter != x.iter;
}

#endif
