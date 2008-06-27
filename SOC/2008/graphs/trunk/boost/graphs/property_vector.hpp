
#ifndef PROPERTY_VERTEX_HPP
#define PROPERTY_VERTEX_HPP

#include <vector>

#include "triple.hpp"

template <typename Store> class propvec_descriptor;

// NOTE: Property stores hold two additional values. Right now, they contain
// vertex descriptors, but would ideally by "iterators" into the incidence store
// to make remove operations involing recipricol edge stubs more efficient.
//
// This really highlights the need for something like a "stable" iterator that
// isn't invalidated by little things like complete reallocations and copies.
// The problem can be stated as: how do you describe a never-invalidating
// iterator into arbitrary containers and, can we map this onto iterators.

/**
 * The property vector implements a vector-based global property store for
 * vector-based edge storage. Assuming, of course, that there are actually edge
 * properties...
 */
template <typename Props, typename Alloc>
class property_vector
{
    typedef std::vector<Props, Alloc> store_type;
public:
    typedef Props properties_triple;
    typedef typename Props::first_type edge_properties;
private:
    typedef typename Props::second_type first_iterator;
    typedef typename Props::third_type second_iterator;
public:
    typedef typename store_type::iterator iterator;
    typedef typename store_type::size_type size_type;
    typedef propvec_descriptor<store_type> property_descriptor;

    inline property_vector()
        : _props()
    { }

    property_descriptor add();
    property_descriptor add(edge_properties const&);

    inline edge_properties& properties(property_descriptor d)
    { return d->first; }

    /** Bind descriptors into the incidence lists into the global property. */
    template <typename VertexDesc>
    void bind(property_descriptor d, VertexDesc src, VertexDesc tgt)
    {
        BOOST_ASSERT(&_props == d.store);
        _props[d.off].second.put(src);
        _props[d.off].third.put(tgt);
    }

    /** Convert an iterator to a descriptor. */
    inline property_descriptor describe(iterator i) const
    { return property_descriptor(&_props, std::distance(_props.begin(), i)); }

    /** Return the number of properties in the store. */
    inline size_type size() const
    { return _props.size(); }

    inline iterator begin() const
    { return _props.begin(); }

    inline iterator end() const
    { return _props.end(); }

private:
    mutable store_type _props;
};


template <typename P, typename A>
typename property_vector<P,A>::property_descriptor
property_vector<P,A>::add()
{
    return add(edge_properties());
}

template <typename P, typename A>
typename property_vector<P,A>::property_descriptor
property_vector<P,A>::add(edge_properties const& p)
{
    _props.push_back(make_triple(p, first_iterator(), second_iterator()));
    return property_descriptor(&_props, _props.size() - 1);
}

/**
 * The propvec descriptor provides an opaque reference into the property list
 * for undirected graphs.
 */
template <typename Store>
struct propvec_descriptor
{
    typedef Store store_type;
    typedef typename Store::size_type value_type;

    propvec_descriptor()
        : store(0), off(value_type(-1))
    { }

    propvec_descriptor(store_type* store, value_type n)
        : store(store), off(n)
    { }

    value_type get() const
    { return off; }

    store_type* store;
    value_type  off;
};

template <typename S>
inline bool
operator==(propvec_descriptor<S> const& a, propvec_descriptor<S> const& b)
{ return a.store == b.store && a.off == b.off; }

template <typename S>
inline bool
operator<(propvec_descriptor<S> const& a, propvec_descriptor<S> const& b)
{ return a.get() < b.get(); }

/**
 * Hashing a property descriptor returns the hash value over the address of
 * the property object pointed at by the descriptor inside the iterator.
 */
template <typename S>
inline std::size_t
hash_value(propvec_descriptor<S> const& x)
{ return boost::hash_value(x.get()); }



#endif

