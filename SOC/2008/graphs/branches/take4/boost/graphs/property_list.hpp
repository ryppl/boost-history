
#ifndef PROPERTY_LIST_HPP
#define PROPERTY_LIST_HPP

#include <list>

#include "triple.hpp"
#include "descriptor.hpp"

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
    typedef typename store_type::const_iterator iterator;
    typedef typename store_type::size_type size_type;
    typedef descriptor<store_type> property_descriptor;

    inline property_list()
        : _props(), _size(0)
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
        d.value().second.put(src);
        d.value().third.put(tgt);
    }

    /** Return the number of properties. */
    inline size_type size() const
    { return _size; }

    inline iterator begin() const
    { return _props.begin(); }

    inline iterator end() const
    { return _props.end(); }

private:
    store_type  _props;
    size_type   _size;
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
    _props.push_back(make_triple(p, first_iterator(), second_iterator()));
    return property_descriptor(_props, boost::prior(_props.end()));
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

