
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
    typedef typename store_type::size_type size_type;

    typedef Props property_type;
    typedef proplist_descriptor<typename store_type::iterator> property_descriptor;

    inline property_list();

    // Add properties
    inline property_descriptor add();
    inline property_descriptor add(property_type const&);

    // Remove properties
    inline void remove(property_descriptor);

    // Property access.
    inline property_type& properties(property_descriptor);

    /** Return the number of properties. */
    inline size_type size() const
    { return _size; }

private:
    store_type  _props;
    size_type   _size;
};

// TODO: This eventually needs to be specialized for empty edges. Basically, if
// you don't want edges, then we can just hand out integers that for each
// edge - probably.
#if 0

/**
 * Partially specialize the property list for the case where the user does
 * not want properties. This will completely remove the property list from
 * the graph, instead creating a simple index enumerator.
 */
template <typename Alloc>
class property_list<none, Alloc>
{
public:
    typedef std::size_t property_descriptor;
};

#endif


template <typename P, typename A>
property_list<P,A>::property_list()
    : _props()
    , _size(0)
{ }

/**
 * Add an empty or default property to the list.
 */
template <typename P, typename A>
typename property_list<P,A>::property_descriptor
property_list<P,A>::add()
{
    ++_size;
    return add(property_type());
}

/**
 * Add the specified properties to the list.
 */
template <typename P, typename A>
typename property_list<P,A>::property_descriptor
property_list<P,A>::add(property_type const& p)
{
    ++_size;
    return _props.insert(_props.end(), p);
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
 * Return the properties corresponding to the given descriptor.
 */
template <typename P, typename A>
typename property_list<P,A>::property_type&
property_list<P,A>::properties(property_descriptor p)
{
    return *p.iter;
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
    inline proplist_descriptor()
        : iter()
    { }

    inline proplist_descriptor(Iter const& iter)
        : iter(iter)
    { }

    inline bool operator==(proplist_descriptor const& x) const
    { return iter == x.iter; }

    inline bool operator<(proplist_descriptor const& x) const
    { return &*iter < &*x.iter; }

    Iter iter;
};


// This helper type is used to erase global edge properties during edge removal
// of undirected graphs.
template <typename PropList>
struct property_eraser
{
    property_eraser(PropList& props)
        : props(props)
    { }

    template <typename PropDesc>
    void operator()(PropDesc p)
    { props.remove(p); }

    PropList& props;
};

// The noop eraser does nothing.
struct noop_eraser
{
    noop_eraser() { }

    template <typename PropDesc>
    void operator()(PropDesc)
    { }
};


#endif

