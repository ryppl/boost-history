
#ifndef OUT_DESCRIPTOR_HPP
#define OUT_DESCRIPTOR_HPP

/**
 * The out descriptor wraps an opaque reference to an out edge pair and
 * provides some convenience functions for translating between the elements
 * of the referenced pair and their "descriptors".
 *
 * @param OutPair The pair of vertex descriptor and edge property being
 * described.
 */
template <typename OutPair>
struct basic_out_descriptor
{
    typedef typename OutPair::first_type vertex_descriptor;
    typedef typename OutPair::second_type edge_properties;

    /** @name Constructors */
    //@{
    inline basic_out_descriptor()
        : _d(0)
    { }

    inline basic_out_descriptor(basic_out_descriptor const& x)
        : _d(x._d)
    { }

    inline basic_out_descriptor(OutPair& o)
        : _d(&o)
    { }

    inline basic_out_descriptor(OutPair const& o)
        : _d(&const_cast<OutPair&>(o))
    { }

    //@{

    OutPair& pair() const
    { return reinterpret_cast<OutPair*>(_d); }

    vertex_descriptor target() const
    { return pair().first; }

    edge_properties& properties()
    { return pair().second; }

    edge_properties const& properties() const
    { return pair().second; }

    mutable void* _d;
};

#endif
