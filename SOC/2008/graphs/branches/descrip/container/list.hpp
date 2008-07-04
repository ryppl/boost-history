
#ifndef CONTAINER_LIST_HPP
#define CONTAINER_LIST_HPP

struct list_tag :
    virtual public reversible_container_tag,
    virtual public back_insertion_sequence_tag,
    virtual public front_insertion_sequence_tag
{ };

template <class T, class Alloc>
struct container_traits<std::list<T,Alloc>>
{
    typedef list_tag category;
    typedef stable_iterator_tag iterator_stability;
};

#endif
