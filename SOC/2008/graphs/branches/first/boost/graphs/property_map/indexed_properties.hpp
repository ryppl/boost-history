
#ifndef BOOST_GRAPHS_PROPERTY_MAP_INDEXED_PROPERTIES_HPP
#define BOOST_GRAPHS_PROPERTY_MAP_INDEXED_PROPERTIES_HPP

#include <iterator>
#include <vector>

namespace boost {
namespace graphs {

/**
 * A simple wrapper around a vector. Because the "key" to this vector is
 * actually given as a descriptor, we have to get the underlying index that
 * allows us to map this value to a property.
 */
template <typename Iterator, typename Property>
struct indexed_property_container
{
    typedef Property value_type;
    typedef typename Iterator::value_type key_type;
    typedef std::vector<Property> container_type;

    // Construct the container over the elements given by the iterator range
    // and the default property value.
    indexed_property_container(Iterator f, Iterator l)
        : _map(std::distance(f, l), Property())
    { }

    indexed_property_container(std::pair<Iterator, Iterator> rng)
        : _map(std::distance(rng.first, rng.second), Property())
    { }

    value_type& operator[](key_type const& k)
    { return _map[k.get()]; }

    container_type _map;
};


/**
 * The corresponding property map implements a lightweight, regular accessor
 * to a indexed property container.
 */
template <typename Iterator, typename Property>
struct indexed_property_map
{
    typedef indexed_property_container<Iterator, Property> container_type;

    typedef typename container_type::value_type property_type;
    typedef typename container_type::key_type key_type;

    indexed_property_map(container_type& cont)
        : data(cont)
    { }

    indexed_property_map(indexed_property_map const& x)
        : data(x.data)
    { }

    container_type& data;
};

template <typename I, typename P>
typename indexed_property_map<I,P>::property_type const&
get(indexed_property_map<I,P> const& pm,
    typename indexed_property_map<I,P>::key_type const& x)
{
    return pm.data[x];
}

template <typename I, typename P>
void
put(indexed_property_map<I,P>& pm,
    typename indexed_property_map<I,P>::key_type const& x,
    typename indexed_property_map<I,P>::property_type const& v)
{
    pm.data[x] = v;
}

} /* namesapce graphs */
} /* namespace boost */

#endif
