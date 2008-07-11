
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
    typedef typename Edge::second_type end_pair;
    typedef typename end_pair::first_type end_type; // same as second_type
    typedef typename end_type::first_type vertex_descriptor;
    typedef typename end_type::second_type incidence_descriptor;

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
        iterator i = _props.insert(_props.end(), make_pair(ep, end_pair()));
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

    /** Return the number of properties in the store. */
    inline size_type size() const
    { return _props.size(); }

    /** Return true if this is empty. */
    inline bool empty() const
    { return _props.empty(); }

    /** @name Iterators */
    //@{
    inline iterator begin() const
    { return _props.begin(); }

    inline iterator end() const
    { return _props.end(); }
    //@}

    /**
     * Bind vertex descriptors into the incidence lists into the global
     * property. This is the last step of edge creation for undirected graphs.
     */
    void bind(property_descriptor d, end_pair const& p)
    { make_iterator(_props, d)->second = p; }

    /** Return the properties referenced by the given descriptor. */
    inline edge_properties& properties(property_descriptor d)
    { return make_iterator(_props, d)->first; }

    /** Return the ends referened by the given descriptor. */
    inline end_pair const& ends(property_descriptor d) const
    { return make_iterator(_props, d)->second; }

    /** Return the first vertex of the edge. */
    inline vertex_descriptor first_vertex(property_descriptor d) const
    { return ends(d).first.first; }

    /** Return the second vertex of the edge. */
    inline vertex_descriptor second_vertex(property_descriptor d) const
    { return ends(d).second.first; }

    /** Return the first incidence descriptor of the edge. */
    inline incidence_descriptor first_edge(property_descriptor d) const
    { return ends(d).first.second; }

    /** Return the second incidence descriptor of the edge. */
    inline incidence_descriptor second_edge(property_descriptor d) const
    { return ends(d).second.second; }


    /** Return a descriptor for the iterator. */
    inline property_descriptor describe(iterator i) const
    { return make_descriptor(_props, i); }

private:
    mutable store_type _props;
};

#endif

