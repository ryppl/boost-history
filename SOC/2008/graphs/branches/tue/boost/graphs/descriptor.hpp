
#ifndef DESCRIPTOR_HPP
#define DESCRIPTOR_HPP

#include "unordered_pair.hpp"

// Important notes about descriptors
//
// A descriptor is basically an opaque reference to an object. It's kind of
// like an iterator that can't be moved or dereferenced. It's kind of like a
// flyweight, but you can't implicitly cast it as the shared object. In short,
// you can't use descriptors without their graph. In general, we'd like the
// descriptor types to be as trivial as possible. One of the most important
// facts about descriptors is that, by themselves, they have absolutely no
// semantics. They cannot be used (in general) be used to access the vertices
// or edges that they represent.
//
// The most important thing to understand about descriptors is that they attempt
// to model the most "consistent" reference into a storage mechanism. By the
// term "consistent", we mean a means of accessing elements that allow us to
// actually build a graph without invalidating memory or iterators. For example,
// we can't use pointers as descriptors into vectors since vectors occasionally
// reallocate all their memory (oops), but indices work just fine.
//
// Descriptors must be completely independent of the actual type of vertices
// and edges. Two problems arise if we don't do this. First, we end up with
// weird cyclic type dependencies that can probably be unrolled using some
// funky lazy template evaluation, but that's generally beyond me. Second,
// this would

// Build trivial wrappers around the underlying descriptor types. This allows
// us to differentiate descriptors based on these types rather than their
// simple descriptor types.

template <typename D>
class basic_vertex_descriptor
{
public:
    inline basic_vertex_descriptor()
        : desc()
    { }

    inline basic_vertex_descriptor(D d)
        : desc(d)
    { }

    // Assignment copy.
    inline basic_vertex_descriptor& operator=(basic_vertex_descriptor const& d)
    {
        desc = d.desc;
        return *this;
    }

    // value-type assignment.
    inline basic_vertex_descriptor& operator=(D d)
    {
        desc = d;
        return *this;
    }

    // Just to make the access a little prettier.
    inline D get() const
    { return desc; }

    // Provides an implicit cast to bool. Returns true if the basic_vertex_descriptor
    // is not the same as its default value.
    inline bool is_valid() const
    { return desc != basic_vertex_descriptor().desc; }

    inline bool operator<(basic_vertex_descriptor const& d) const
    { return desc < d.desc; }

    inline bool operator==(basic_vertex_descriptor const& d) const
    { return desc == d.desc; }

    inline bool operator!=(basic_vertex_descriptor const& d) const
    { return desc != d.desc; }

    D desc;
};

// We actually need a specialized undirected vertex descriptor that is capable of
// representing an edge, given a source vertex, a target vertex and a reference
// to the edge properties.
template <typename VertexDesc, typename PropDesc>
class undirected_edge_descriptor
{
public:
    undirected_edge_descriptor()
        : edge()
        , prop()
    { }

    undirected_edge_descriptor(VertexDesc u, VertexDesc v, PropDesc p)
        : edge(u, v)
        , prop(p)
    { }

    bool operator<(undirected_edge_descriptor const& x)
    {
        return edge < x.edge || (!(x.edge < edge) && prop < x.prop);
    }

private:
    unordered_pair<VertexDesc> edge;
    PropDesc prop;
};

#endif

