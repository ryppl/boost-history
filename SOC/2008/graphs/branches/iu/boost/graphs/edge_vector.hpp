
#ifndef EDGE_VECTOR_HPP
#define EDGE_VECTOR_HPP

#include "property_vector.hpp"
#include "incidence_vector.hpp"

// What's in an edge vector? Basically, an edge vector has to specify
// the type of property storage and the type of incidence storage. What
// are the possible parameters to edge storage?
// 1. The edge properties
// 2. Allocator for the property store
// 3. Allocator for the incidence store

template <
    template <typename> class IncAlloc,
    template <typename> class PropAlloc>
struct basic_edge_vector
{
    template <typename EdgeProps>
    struct property_store
    {
        typedef property_vector<EdgeProps, PropAlloc<EdgeProps> > type;
    };

    template <typename VertexDesc, typename PropDesc>
    struct incidence_store
    {
        typedef std::pair<VertexDesc, PropDesc> incidence_pair;
        typedef incidence_vector<incidence_pair, IncAlloc<incidence_pair> > type;
    };
};

struct edge_vector : basic_edge_vector<std::allocator, std::allocator> { };

#endif

