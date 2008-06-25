
#ifndef INDEXED_PROPERTIES_HPP
#define INDEXED_PROPERTIES_HPP

#include <iterator>
#include <vector>

namespace detail {

/**
 * Wrap an iterator with a default value so that it generates default values
 * over a range of keys.
 */
template <typename Iter, typename Prop>
struct default_value_iterator
{
    typedef typename std::forward_iterator_tag iterator_category;
    typedef std::size_t difference_type;

    typedef Prop value_type;
    typedef value_type const& reference;
    typedef value_type const* pointer;

    default_value_iterator(Iter i)
        : iter(i)
        , value()
    { }

    default_value_iterator(Iter i, Prop const& p)
        : iter(i)
        , value(p)
    { }

    default_value_iterator& operator++()
    { ++iter; return *this; }

    reference operator*()
    { return value; }

    bool operator==(default_value_iterator const& x) const
    { return iter == x.iter; }

    bool operator!=(default_value_iterator const& x) const
    { return iter != x.iter; }

    Iter    iter;
    Prop    value;
};

template <typename Iter, typename Prop>
inline default_value_iterator<Iter, Prop>
make_default_value_iterator(Iter i, Prop const&)
{ return default_value_iterator<Iter, Prop>(i); }

} // namespace detail


/**
 * A simple wrapper around a vector. Because the "key" to this vector is
 * actually given as a descriptor, we have to get the underlying index that
 * allows us to map this value to a property.
 *
 * This definitely rquires that the descriptor values be continuous.
 */
template <typename Descriptor, typename Property>
struct indexed_property_container
{
    typedef Property value_type;
    typedef Descriptor key_type;
    typedef std::vector<Property> container_type;

    inline indexed_property_container(std::size_t n)
        : data(n)
    { }

    /**
     * Construct the hashtable over the keys in the iterator range [f, l) with
     * the default value x.
     */
    template <typename Iter>
    inline indexed_property_container(Iter f, Iter l, value_type const& x)
        : data(detail::make_default_value_iterator(f, x),
               detail::make_default_value_iterator(l, value_type()))
    { }

    inline value_type& operator[](key_type const& k)
    { return data[k.get()]; }

    container_type data;
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

#endif
