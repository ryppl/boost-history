
#ifndef PROPERTY_VERTEX_HPP
#define PROPERTY_VERTEX_HPP

#include <vector>

#include "triple.hpp"

// The property vector implements a vector-based global property
// store for vector-based edge storage. Assuming, of course, that
// there are actually edge properties.

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
    typedef typename store_type::const_iterator iterator;
    typedef typename store_type::size_type size_type;
    typedef std::size_t property_descriptor;

    inline property_vector()
        : _props()
    { }

    property_descriptor add();
    property_descriptor add(edge_properties const&);

    inline edge_properties& properties(property_descriptor d)
    { return _props[d].first; }

    /** Bind iterators into the incidence lists into the global property. */
    template <typename Iter>
    void bind(property_descriptor d, Iter src, Iter tgt)
    {
        _props[d].second.put(src);
        _props[d].third.put(tgt);
    }

    /** Return the number of properties in the store. */
    inline size_type size() const
    { return _props.size(); }

    inline iterator begin() const
    { return _props.begin(); }

    inline iterator end() const
    { return _props.end(); }

private:
    store_type _props;
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
    return _props.size() - 1;
}

#endif

