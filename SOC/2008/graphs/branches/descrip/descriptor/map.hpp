
#ifndef DESCRIPTOR_MAP_HPP
#define DESCRIPTOR_MAP_HPP

/** Generate a map descriptor for a map with the given parameters. */
template <typename K, typename T, typename Compare = std::less<T>, typename Alloc = std::allocator<T>>
struct map_descriptor
{
    typedef blob<sizeof(typename std::map<T, K, Compare, Alloc>::iterator)> blob_type;
    typedef node_descriptor<blob_type> type;
};

/**
 * Extended container traits for maps.
 */
template <typename K, typename T, typename Compare, typename Alloc>
struct extended_container_traits<std::map<K, T, Compare, Alloc>>
{
    typedef typename map_descriptor<K, T, Compare, Alloc>::type descriptor_type;
    typedef stable_descriptor_tag descriptor_stability;
};

#endif
