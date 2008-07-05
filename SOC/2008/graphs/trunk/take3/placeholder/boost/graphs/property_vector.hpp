
#ifndef PROPERTY_VECTOR_HPP
#define PROPERTY_VECTOR_HPP

#include <vector>

#include "triple.hpp"
#include "descriptor.hpp"

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
    typedef descriptor<store_type> property_descriptor;

    inline property_vector()
        : _props()
    { }

    property_descriptor add();
    property_descriptor add(edge_properties const&);

    /** Return the properties referenced by the given descriptor. */
    inline edge_properties& properties(property_descriptor d)
    { return d.value().first; }

    /** Bind descriptors into the incidence lists into the global property. */
    template <typename VertexDesc>
    void bind(property_descriptor d, VertexDesc src, VertexDesc tgt)
    {
        d.value().second.put(src);
        d.value().third.put(tgt);
    }

    /** Convert an iterator to a descriptor. */
    inline property_descriptor describe(iterator i) const
    { return property_descriptor(_props, i); }

    /** Return the number of properties in the store. */
    inline size_type size() const
    { return _props.size(); }

    /** @name Iterators */
    //@{
    inline iterator begin() const
    { return _props.begin(); }

    inline iterator end() const
    { return _props.end(); }
    //@}

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
    return property_descriptor(_props, boost::prior(_props.end()));
}

#endif

