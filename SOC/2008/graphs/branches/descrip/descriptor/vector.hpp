
#ifndef VECTOR_HPP
#define VECTOR_HPP

/** Generate a descriptor for any vector with the given properties. */
template <typename T, typename Alloc = std::allocator<T>>
struct vector_descriptor
{
    typedef typename std::vector<T, Alloc>::size_type index_type;
    typedef index_descriptor<index_type> type;
};

template <typename T, typename Alloc>
inline typename vector_descriptor<T, Alloc>::type
get_descriptor(std::vector<T, Alloc>& c, typename std::vector<T, Alloc>::iterator i)
{
    typedef typename vector_descriptor<T, Alloc>::type result_type;
    return result_type(std::distance(c.begin(), i));
}

template <typename T, typename Alloc>
inline typename std::vector<T, Alloc>::iterator
get_iterator(std::vector<T, Alloc>& c, typename vector_descriptor<T, Alloc>::type d)
{
    return std::next(c.begin(), d.value);
}

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
