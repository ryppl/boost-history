
#ifndef HASHED_PROPERTIES_HPP
#define HASHED_PROPERTIES_HPP

#include <tr1/unordered_map>

#include <boost/functional/hash.hpp>

namespace detail {

/**
 * Wrap an iterator with a default value so that it generates default values
 * over a range of keys.
 */
template <typename Iter, typename Prop>
struct key_value_iterator
{
    typedef typename std::forward_iterator_tag iterator_category;
    typedef typename std::size_t difference_type;

    typedef std::pair<typename Iter::value_type, Prop> value_type;
    typedef value_type reference;
    typedef value_type pointer;

    key_value_iterator(Iter i)
        : iter(i)
        , value()
    { }

    key_value_iterator(Iter i, Prop const& p)
        : iter(i)
        , value(p)
    { }

    key_value_iterator& operator++()
    { ++iter; return *this; }

    reference operator*()
    { return make_pair(*iter, value); }

    bool operator==(key_value_iterator const& x) const
    { return iter == x.iter; }

    bool operator!=(key_value_iterator const& x) const
    { return iter != x.iter; }

    Iter    iter;
    Prop    value;
};

template <typename Iter, typename Prop>
inline key_value_iterator<Iter, Prop>
make_key_value_iterator(Iter i, Prop const& x)
{ return key_value_iterator<Iter, Prop>(i, x); }

} // namespace detail

/**
 * A simple wrapper around an unordered map, this is used to map descriptors
 * to arbitrary property values. Note that the property type must be default
 * constructible.
 *
 * This may seem a little odd because we're passing an iterator and not the key
 * type. However, the key type is always the iterator's value type.
 */
template <typename Descriptor, typename Property>
class hashed_property_container
{
public:
    typedef Property value_type;
    typedef Descriptor key_type;
    typedef std::tr1::unordered_map<
            key_type, value_type, boost::hash<key_type>
        > container_type;

    /**
     * Construct the hashtable over n buckets. This may not actually allocate
     * n buckets, so we can't necessarily guarantee that memory will actually
     * be allocated for each element, much less what those default values would
     * actually be.
     */
    hashed_property_container(std::size_t n)
        : data(n)
    { }

    /**
     * Construct the hashtable over the keys in the iterator range [f, l) with
     * the default value x.
     */
    template <typename Iter>
    hashed_property_container(Iter f, Iter l, value_type const& x)
        : data(detail::make_key_value_iterator(f, x),
               detail::make_key_value_iterator(l, value_type()))
    { }

    // Get the property associated with the key k.
    inline value_type& operator[](key_type const& k)
    { return data[k]; }

    container_type data;
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

#endif
