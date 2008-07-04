
#ifndef DESCRIPTOR_LIST_HPP
#define DESCRIPTOR_LIST_HPP

/** Generate a descriptor for a list wtih the given properties. */
template <typename T, typename Alloc = std::allocator<T>>
struct list_descriptor
{
    typedef blob<sizeof(typename std::list<T, Alloc>::iterator)> blob_type;
    typedef node_descriptor<blob_type> type;
};

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
