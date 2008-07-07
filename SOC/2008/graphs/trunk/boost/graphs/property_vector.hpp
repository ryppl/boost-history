
#ifndef PROPERTY_VECTOR_HPP
#define PROPERTY_VECTOR_HPP

#include <vector>
#include <algorithm>

#include <boost/descriptors.hpp>
#include <boost/graphs/utility.hpp>

/**
 * The property vector implements a vector-based global property store for
 * vector-based edge storage. Assuming, of course, that there are actually edge
 * properties.
 */
template <typename Edge, typename Alloc>
class property_vector
{
public:
    typedef std::vector<Edge, Alloc> store_type;

    typedef Edge edge_type;
    typedef typename Edge::first_type edge_properties;
    typedef typename Edge::second_type edge_pair;

    typedef typename store_type::iterator iterator;
    typedef typename store_type::size_type size_type;

    typedef typename descriptor_traits<store_type>::descriptor_type property_descriptor;

    // Constructor
    inline property_vector()
        : _props()
    { }

    /** @name Add Property
     * Add a property tot the global set, leaving the incidence descriptors
     * empty for the time being.
     */
    //@{
    property_descriptor add()
    { return add(edge_properties()); }

    property_descriptor add(edge_properties const& ep)
    {
        iterator i = _props.insert(_props.end(), make_pair(ep, edge_pair()));
        return make_descriptor(_props, i);
    }
    //@}

    /**
     * Find the edge with the given properties. This function is guaranteed to
     * run in O(E) time.
     */
    property_descriptor find(edge_properties const& ep) const
    {
        iterator i = std::find_if(_props.begin(), _props.end(), find_first(ep));
        return make_descriptor(_props, i);
    }

    /** Return the properties referenced by the given descriptor. */
    inline edge_properties& properties(property_descriptor d)
    { return d.value().first; }

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

#endif

