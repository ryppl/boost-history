
#ifndef PROPERTY_LIST_HPP
#define PROPERTY_LIST_HPP

#include <list>

// Forward descriptor
template <typename Iter> class proplist_descriptor;

/**
 * The property list implements global list of properties for node-based edge
 * storage. Note that we can get away with only a list because the edge
 * addition logic is implemented by the incidence list.
 *
 * The property store actually maintains the number of elements internally.
 * Because this store is built over a list, but only allows the insertion and
 * removal of one element at a time, we do this to optimize calls to the size()
 * function (which is used for the number of edges).
 */
template <typename Props, typename Alloc>
class property_list
{
    typedef std::list<Props, Alloc> store_type;
public:
    typedef Props properties_triple;
    typedef typename Props::first_type edge_properties;
private:
    typedef typename Props::second_type first_iterator;
    typedef typename Props::third_type second_iterator;
public:
    typedef typename store_type::iterator iterator;
    typedef typename store_type::size_type size_type;
    typedef proplist_descriptor<typename store_type::iterator> property_descriptor;

    inline property_list()
        : _props()
        , _size(0)
    { }

    // Add properties
    inline property_descriptor add();
    inline property_descriptor add(edge_properties const&);

    // Remove properties
    inline void remove(property_descriptor);

    // Property access.
    inline edge_properties& properties(property_descriptor d)
    { return d.iter->first; }

    /** Bind iterators into the incidence lists into the global property. */
    template <typename Iter>
    void bind(property_descriptor d, Iter src, Iter tgt)
    {
        d.iter->second.put(src);
        d.iter->third.put(tgt);
    }

    /** Convert an iterator to a descriptor. */
    inline property_descriptor describe(iterator i) const
    { return i; }

    /** Return the number of properties. */
    inline size_type size() const
    { return _size; }

    inline iterator begin() const
    { return _props.begin(); }

    inline iterator end() const
    { return _props.end(); }

private:
    mutable store_type  _props;
    size_type           _size;
};

/**
 * Add an empty or default property to the list.
 */
template <typename P, typename A>
typename property_list<P,A>::property_descriptor
property_list<P,A>::add()
{
    return add(edge_properties());
}

/**
 * Add the specified properties to the list.
 */
template <typename P, typename A>
typename property_list<P,A>::property_descriptor
property_list<P,A>::add(edge_properties const& p)
{
    ++_size;
    return _props.insert(_props.end(), make_triple(p, first_iterator(), second_iterator()));
}

/**
 * Remove the indicated property from the list. This invalidates any
 * descriptors and iterators to the given property, but no others.
 *
 * @complexity O(1)
 */
template <typename P, typename A>
void
property_list<P,A>::remove(property_descriptor p)
{
    --_size;
    _props.erase(p.iter);
}

/**
 * The proplist descriptor provides a wrapper around a list iterator that
 * provides comparability for the underlying iterator. Note that the comparison
 * has little to do with actual ordering of elements. This is to say that
 * i \< j !=> *i \< *j. This just provides a mechanism for ordering the
 * descriptors.
 */
template <typename Iter>
struct proplist_descriptor
{
    typedef typename Iter::pointer value_type;

    inline proplist_descriptor()
        : iter()
    { }

    inline proplist_descriptor(Iter const& iter)
        : iter(iter)
    { }

    inline value_type get() const
    { return &*iter; }

    Iter iter;
};

template <typename I>
inline bool
operator==(proplist_descriptor<I> const& a, proplist_descriptor<I> const& b)
{ return a.iter == b.iter; }

template <typename I>
inline bool
operator<(proplist_descriptor<I> const& a, proplist_descriptor<I> const& b)
{ return a.get() < b.get(); }

/**
 * Hashing a property descriptor returns the hash value over the address of
 * the property object pointed at by the descriptor inside the iterator.
 */
template <typename Iter>
inline std::size_t
hash_value(proplist_descriptor<Iter> const& x)
{ return boost::hash_value(x.get()); }


#endif

