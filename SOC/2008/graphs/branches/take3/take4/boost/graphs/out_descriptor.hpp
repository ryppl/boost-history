
#ifndef OUT_DESCRIPTOR_HPP
#define OUT_DESCRIPTOR_HPP

/**
 * The out descriptor wraps an iterator into the out vector. This is primarily
 * provided to help decouple the directed edge from the underlying reference
 * to the out edge list.
 *
 * @param OutIter The pair of vertex descriptor and edge property being
 * described.
 */
template <typename OutIter>
struct basic_out_descriptor
{
    typedef typename OutIter::value_type out_pair;
    typedef typename out_pair::first_type vertex_descriptor;
    typedef typename out_pair::second_type edge_properties;

    /** @name Constructors */
    //@{
    inline basic_out_descriptor()
        : iter()
    { }

    inline basic_out_descriptor(basic_out_descriptor const& x)
        : iter(x.iter)
    { }

    inline basic_out_descriptor(OutIter const& iter)
        : iter(iter)
    { }
    //@}

    inline vertex_descriptor target() const
    { return iter->first; }

    inline edge_properties& properties()
    { return iter->second; }

    inline edge_properties const& properties() const
    { return iter->second; }

    OutIter iter;
};

#endif
