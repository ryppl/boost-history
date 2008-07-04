
#ifndef CONTAINER_SET_HPP
#define CONTAINER_SET_HPP

struct set_tag :
    virtual public sorted_associative_container_tag,
    virtual public simple_associative_container_tag,
    virtual public unique_associative_container_tag
{ };


template <class Key, class Cmp, class Alloc>
struct container_traits<std::set<Key,Cmp,Alloc>>
{
    typedef set_tag category;
    typedef stable_iterator_tag iterator_stability;
};

#endif
