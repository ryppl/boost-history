
#ifndef DESCRIPTOR_VECTOR_HPP
#define DESCRIPTOR_VECTOR_HPP

/** Generate a descriptor for any vector with the given properties. */
template <typename T, typename Alloc = std::allocator<T>>
struct vector_descriptor
{
    typedef typename std::vector<T, Alloc>::size_type index_type;
    typedef index_descriptor<index_type> type;
};

/**
 * Extended container traits for vectors.
 */
template <typename T, typename Alloc>
struct extended_container_traits<std::vector<T, Alloc>>
{
    typedef typename vector_descriptor<T, Alloc>::type descriptor_type;
    typedef unstable_remove_tag descriptor_stability;
};


#endif
