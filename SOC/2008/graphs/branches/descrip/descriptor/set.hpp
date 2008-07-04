
#ifndef SET_HPP
#define SET_HPP

/** Generate a set descriptor for a set with the given parameters. */
template <typename T, typename Compare = std::less<T>, typename Alloc = std::allocator<T>>
struct set_descriptor
{
    typedef blob<sizeof(typename std::set<T, Compare, Alloc>::iterator)> blob_type;
    typedef node_descriptor<blob_type> type;
};

template <typename T, typename Compare, typename Alloc>
inline typename set_descriptor<T, Compare, Alloc>::type
get_descriptor(std::set<T, Compare, Alloc>& c, typename std::set<T, Compare, Alloc>::iterator i)
{
    typedef typename set_descriptor<T, Compare, Alloc>::type result_type;
    return result_type(i);
}

template <typename T, typename Compare, typename Alloc>
inline typename std::set<T, Compare, Alloc>::iterator
get_iterator(std::set<T, Compare, Alloc>& c, typename set_descriptor<T, Compare, Alloc>::type d)
{
    typedef typename std::set<T, Compare, Alloc>::iterator result_type;
    return d.value.template get<result_type>();
}

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
