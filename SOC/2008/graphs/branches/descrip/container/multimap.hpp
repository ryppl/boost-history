
#ifndef CONTAINER_MULTIMAP_HPP
#define CONTAINER_MULTIMAP_HPP

struct multimap_tag :
    virtual public sorted_associative_container_tag,
    virtual public pair_associative_container_tag,
    virtual public multiple_associative_container_tag
{ };

template <class Key, class T, class Compare, class Alloc>
struct container_traits<std::multimap<Key, T, Compare, Alloc>>
{
    typedef multimap_tag category;
    typedef stable_iterator_tag iterator_stability;
};

#endif
