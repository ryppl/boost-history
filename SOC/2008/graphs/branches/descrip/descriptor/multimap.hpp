
#ifndef DESCRIPTOR_MULTIMAP_HPP
#define DESCRIPTOR_MULTIMAP_HPP

/** Generate a multimap descriptor for a multimap with the given parameters. */
template <typename K, typename T, typename Compare = std::less<T>, typename Alloc = std::allocator<T>>
struct multimap_descriptor
{
    typedef blob<sizeof(typename std::multimap<T, K, Compare, Alloc>::iterator)> blob_type;
    typedef node_descriptor<blob_type> type;
};

/**
 * Extended container traits for multimaps.
 */
template <typename K, typename T, typename Compare, typename Alloc>
struct extended_container_traits<std::multimap<K, T, Compare, Alloc>>
{
    typedef typename multimap_descriptor<K, T, Compare, Alloc>::type descriptor_type;
    typedef stable_descriptor_tag descriptor_stability;
};

#endif
