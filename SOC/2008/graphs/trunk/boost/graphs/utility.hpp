
#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <boost/descriptors.hpp>


/**
 * @internal
 * A forwarding comparator for proeprties objects that forwards the comparison
 * to the configured comparator. This type is used internally to forward comparisons of vertices
 * to the property comparison provided by the edge set parameter.
 * @param Vertex The type of vertex being compared
 * @param Compare An ordering over vertex properties.
 */
template <typename Compare>
struct property_comparator
{
    inline property_comparator()
        : comp(Compare())
    { }

    template <typename Object>
    inline bool operator()(Object const& a, Object const& b) const
    { return comp(a.properties(), b.properties()); }

    Compare comp;
};

/**
 * @internal
 * A functor that returns true when we can find a propertied object with the
 * same value as those given in the cosntructor. This works for both vertices
 * and edges.
 * @param Properties The type of properties being compared.
 * @todo This goes away with lambda expressions. Used in vertex_[v|l]::find().
 */
template <typename Properties>
struct property_finder
{
    inline property_finder(Properties const& p)
        : props(p)
    { }

    template <typename Object>
    inline bool operator()(Object const& o) const
    { return o.properties() == props; }

    Properties const& props;
};

template <typename Properties>
inline property_finder<Properties>
find_properties(Properties const& props)
{ return property_finder<Properties>(props); }


/**
 * @internal
 * A functor that returns true when the first value of a pair (or other tuple
 * with a named first element) matches that of the constructed value.
 * @param First The type of the first element.
 * @todo This goes away with lambda expression (in property_*::find and
 * incidence_*::find).
 */
template <typename First>
struct first_finder
{
    inline first_finder(First const& x)
        : value(x)
    { }

    template <typename Object>
    inline bool operator()(Object const& o) const
    { return o.first == value; }

    First const& value;
};

template <typename First>
inline first_finder<First>
find_first(First const& props)
{ return first_finder<First>(props); }


#endif
