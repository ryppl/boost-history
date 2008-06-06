
#ifndef BOOST_GRAPHS_PROPERTY_MAP_HASHED_PROPERTIES_HPP
#define BOOST_GRAPHS_PROPERTY_MAP_HASHED_PROPERTIES_HPP

#include <tr1/unordered_map>

#include <boost/functional/hash.hpp>

namespace boost {
namespace graphs {

namespace detail {

template <typename Iterator, typename Property>
struct make_pair_iterator
{
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::difference_type difference_type;
    typedef std::pair<typename Iterator::value_type, Property> value_type;
    typedef value_type reference;
    typedef value_type pointer;

    make_pair_iterator(Iterator i)
        : iter(i)
    { }

    make_pair_iterator& operator++()
    { ++iter; return *this; }

    reference operator*()
    { return make_pair(*iter, Property()); }

    bool operator==(make_pair_iterator const& x) const
    { return iter == x.iter; }

    bool operator!=(make_pair_iterator const& x) const
    { return iter != x.iter; }

    Iterator iter;
};

} /* namespace detail */


/**
 * A simple wrapper around an unordered map, this is used to map descriptors
 * to arbitrary property values. Note that the property type must be default
 * constructible.
 *
 * This may seem a little odd because we're passing an iterator and not the key
 * type. However, the key type is always the iterator's value type.
 */
template <typename Iterator, typename Property>
class hashed_property_container
{
    typedef detail::make_pair_iterator<Iterator, Property> pair_iterator;
public:
    typedef Property value_type;
    typedef typename Iterator::value_type key_type;
    typedef std::tr1::unordered_map<key_type, value_type, hash<key_type> > container_type;


    // Construct the container over n elements with the given default property
    // value. If a number of buckets is explicitly given (and is not 0), then
    // construct the object with b buckets. Otherwise, construct with n buckets.
    // The default behavior will prevent rehashing during construction.
    inline hashed_property_container(Iterator f, Iterator l)
        : _map(pair_iterator(f), pair_iterator(l))
    { }

    inline hashed_property_container(std::pair<Iterator, Iterator> rng)
        : _map(pair_iterator(rng.first), pair_iterator(rng.second))
    { }

    // Get the property associated with the key k.
    inline value_type& operator[](key_type const& k)
    { return _map[k]; }

private:
    container_type _map;
};

/**
 * The corresponding property map implements a lightweight, regular accessor
 * to a hashed property container.
 */
template <typename Iterator, typename Property>
class hashed_property_map
{
    typedef hashed_property_container<Iterator, Property> container_type;
public:
    typedef typename container_type::value_type property_type;
    typedef typename container_type::key_type key_type;

    hashed_property_map(container_type& cont)
        : data(cont)
    { }

    hashed_property_map(hashed_property_map const& x)
        : data(x.data)
    { }

    container_type& data;
};

template <typename I, typename P>
typename hashed_property_map<I,P>::property_type const&
get(hashed_property_map<I,P> const& pm,
    typename hashed_property_map<I,P>::key_type const& x)
{
    return pm.data[x];
}

template <typename I, typename P>
void
put(hashed_property_map<I,P>& pm,
    typename hashed_property_map<I,P>::key_type const& x,
    typename hashed_property_map<I,P>::property_type const& v)
{
    pm.data[x] = v;
}

} /* namesapce graphs */
} /* namespace boost */

#endif
