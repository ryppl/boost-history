
#ifndef CONTAINER_MAP_HPP
#define CONTAINER_MAP_HPP

struct map_tag :
    virtual public sorted_associative_container_tag,
    virtual public pair_associative_container_tag,
    virtual public unique_associative_container_tag
{ };

template <class Key, class T, class Compare, class Alloc>
struct container_traits<std::map<Key, T, Compare, Alloc>>
{
    typedef map_tag category;
    typedef stable_iterator_tag iterator_stability;
};

#endif
