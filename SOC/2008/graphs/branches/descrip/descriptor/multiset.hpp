
#ifndef DESCRIPTOR_MULTISET_HPP
#define DESCRIPTOR_MULTISET_HPP

/** Generate a multiset descriptor for a multiset with the given parameters. */
template <typename T, typename Compare = std::less<T>, typename Alloc = std::allocator<T>>
struct multiset_descriptor
{
    typedef blob<sizeof(typename std::multiset<T, Compare, Alloc>::iterator)> blob_type;
    typedef node_descriptor<blob_type> type;
};

/**
 * Extended container traits for multisets.
 */
template <typename T, typename Compare, typename Alloc>
struct extended_container_traits<std::multiset<T, Compare, Alloc>>
{
    typedef typename multiset_descriptor<T, Compare, Alloc>::type descriptor_type;
    typedef stable_descriptor_tag descriptor_stability;
};


#endif
