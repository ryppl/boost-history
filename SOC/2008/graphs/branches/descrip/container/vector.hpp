
#ifndef CONTAINER_VECTOR_HPP
#define CONTAINER_VECTOR_HPP

struct vector_tag :
    virtual public random_access_container_tag,
    virtual public back_insertion_sequence_tag
{ };

template <class T, class Alloc>
struct container_traits<std::vector<T,Alloc>>
{
    typedef vector_tag category;
    typedef unstable_iterator_tag iterator_stability;
};

#endif
