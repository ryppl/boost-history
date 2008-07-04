
#ifndef DESCRIPTOR_SET_HPP
#define DESCRIPTOR_SET_HPP

/** Generate a set descriptor for a set with the given parameters. */
template <typename T, typename Compare = std::less<T>, typename Alloc = std::allocator<T>>
struct set_descriptor
{
    typedef blob<sizeof(typename std::set<T, Compare, Alloc>::iterator)> blob_type;
    typedef node_descriptor<blob_type> type;
};

/**
 * Extended container traits for sets.
 */
template <typename T, typename Compare, typename Alloc>
struct extended_container_traits<std::set<T, Compare, Alloc>>
{
    typedef typename set_descriptor<T, Compare, Alloc>::type descriptor_type;
    typedef stable_descriptor_tag descriptor_stability;
};


#endif
