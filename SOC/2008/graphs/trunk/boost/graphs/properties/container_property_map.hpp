
#ifndef CONTAINER_PROPERTY_MAP_HPP
#define CONTAINER_PROPERTY_MAP_HPP

template <typename Container>
struct container_property_map
{
    typedef typename Container::key_type key_type;
    typedef typename Container::value_type value_type;
    typedef value_type& reference;

    container_property_map(Container& cont)
        : container(cont)
    { }

    value_type& operator()(key_type const& key)
    { return container[key]; }

    value_type const& operator()(key_type const& key) const
    { return container[key]; }

    Container& container;
};

#endif
