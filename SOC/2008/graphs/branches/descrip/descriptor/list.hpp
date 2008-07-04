
#ifndef LIST_HPP
#define LIST_HPP

/** Generate a descriptor for a list wtih the given properties. */
template <typename T, typename Alloc = std::allocator<T>>
struct list_descriptor
{
    typedef blob<sizeof(typename std::list<T, Alloc>::iterator)> blob_type;
    typedef node_descriptor<blob_type> type;
};

template <typename T, typename Alloc>
inline typename list_descriptor<T, Alloc>::type
get_descriptor(std::list<T, Alloc>& c, typename std::list<T, Alloc>::iterator i)
{
    typedef typename list_descriptor<T, Alloc>::type result_type;
    return result_type(i);
}

template <typename T, typename Alloc>
inline typename std::list<T, Alloc>::iterator
get_iterator(std::list<T, Alloc>& c, typename list_descriptor<T, Alloc>::type d)
{
    typedef typename std::list<T, Alloc>::iterator result_type;
    return d.value.template get<result_type>();
}

/**
 * Extended container traits for lists.
 */
template <typename T, typename Alloc>
struct extended_container_traits<std::list<T, Alloc>>
{
    typedef typename list_descriptor<T, Alloc>::type descriptor_type;
    typedef stable_descriptor_tag descriptor_stability;
};


#endif
