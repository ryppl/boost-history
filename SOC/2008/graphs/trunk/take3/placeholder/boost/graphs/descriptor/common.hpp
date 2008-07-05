
#ifndef NODE_DESCRIPTOR_HPP
#define NODE_DESCRIPTOR_HPP

/**
 * The default implementation of descriptor implementations satisfies the
 * requirements for most node-based containers such as lists, sets, and maps.
 * These requirements are satisfied for node-based containers because they
 * are relatively stable.
 *
 * Note that maps store key/value pairs, so the returned value is actually
 * to the start of the pair rather than the intended mapped value.
 */
template <typename Container>
struct descriptor_impl
{
    typedef Container container_type;
    typedef typename container_type::iterator iterator;
    typedef typename container_type::value_type value_type;
    typedef typename iterator::pointer index_type;
    
    inline descriptor_impl()
        : cont(0), iter()
    { }
    
    inline descriptor_impl(container_type& c, iterator i)
        : cont(&c), iter(i)
    { }
    
    inline bool valid() const
    { return cont != 0; }
    
    inline index_type index() const
    { return &*iter; }
    
    inline value_type& value()
    { return *iter; }
    
    void reset()
    {
        cont = 0; 
        iter = iterator(); 
    }
    
    inline bool operator==(descriptor_impl const& x) const
    { return (cont == x.cont) && (index() == x.index()); }
    
    inline bool operator!=(descriptor_impl const& x) const
    { return !operator==(x); }
    
    inline bool operator<(descriptor_impl const& x) const
    { return std::make_pair(cont, index()) < std::make_pair(x.cont, x.index()); }
    
    inline bool operator>(descriptor_impl const& x) const
    { return x.operator<(*this); }
    
    inline bool operator<=(descriptor_impl const& x) const
    { return !x.operator<(*this); }
    
    inline bool operator>=(descriptor_impl const& x) const
    { return !operator<(x); }
    
    container_type* cont;
    iterator        iter;
};

#endif

