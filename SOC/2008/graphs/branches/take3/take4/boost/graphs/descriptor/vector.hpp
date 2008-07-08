
#ifndef VECTOR_DESCRIPTOR_HPP
#define VECTOR_DESCRIPTOR_HPP

/**
 * Vectors are some of the most troublesome data structures to work with because
 * their iterators can be invalidated so easily - especially on insertion. This
 * descriptor prevents invalidation on insertion, but any deletions will still
 * invalidate all descriptors whose indices are greater than than the one 
 * deleted.
 *
 * Vector descriptors retain a pointer to the container itself, and the offset
 * of the object being described.
 */
template <typename T, typename Alloc>
struct descriptor_impl< std::vector<T, Alloc> >
{
    typedef std::vector<T, Alloc> container_type;
    typedef typename container_type::iterator iterator;
    typedef typename container_type::size_type index_type;
    typedef typename container_type::value_type value_type;
    
    inline descriptor_impl()
        : cont(0), off(index_type(-1))
    { }
    
    inline descriptor_impl(container_type& c, iterator i)
        : cont(&c), off(std::distance(c.begin(), i))
    { }
    
    inline bool valid() const
    { return cont != 0; }
    
    inline index_type index() const
    { return off; }

    inline value_type& value()
    { return (*cont)[off]; }

    void reset()
    {
        cont = 0;
        off = index_type(-1);
    }
    
    inline bool operator==(descriptor_impl const& x) const
    { return (cont == x.cont) && (off == x.off); }
    
    inline bool operator!=(descriptor_impl const& x) const
    { return !operator==(x); }
    
    inline bool operator<(descriptor_impl const& x) const
    { return std::make_pair(cont, off) < std::make_pair(x.cont, x.off); }
    
    inline bool operator>(descriptor_impl const& x) const
    { return x.operator<(*this); }
    
    inline bool operator<=(descriptor_impl const& x) const
    { return !x.operator<(*this); }
    
    inline bool operator>=(descriptor_impl const& x) const
    { return !operator<(x); }
    
    container_type* cont;
    index_type      off;
};

#endif

