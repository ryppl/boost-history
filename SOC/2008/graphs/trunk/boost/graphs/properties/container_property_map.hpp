
#ifndef CONTAINER_PROPERTY_MAP_HPP
#define CONTAINER_PROPERTY_MAP_HPP

#include <algorithm>

template <typename Container>
struct container_property_map
{
    typedef typename Container::key_type key_type;
    typedef typename Container::value_type value_type;
    typedef value_type& reference;

    inline container_property_map()
        : container(0)
    { }

    inline container_property_map(Container& cont)
        : container(&cont)
    { }

    inline value_type& operator()(key_type const& key)
    { return (*container)[key]; }

    inline value_type const& operator()(key_type const& key) const
    { return (*container)[key]; }

    inline void swap(container_property_map& x)
    { std::swap(container, x.container); }

    Container* container;
};

#endif
