
#ifndef UTILITY_HPP
#define UTILITY_HPP

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


#endif
