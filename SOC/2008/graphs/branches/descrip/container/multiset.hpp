
#ifndef CONTAINER_MULTISET_HPP
#define CONTAINER_MULTISET_HPP

struct multiset_tag :
    virtual public sorted_associative_container_tag,
    virtual public simple_associative_container_tag,
    virtual public multiple_associative_container_tag
{ };

template <class T, class Compare, class Alloc>
struct container_traits<std::multiset<T, Compare, Alloc>>
{
    typedef multiset_tag category;
    typedef stable_iterator_tag iterator_stability;
};

#endif
