
#ifndef OUT_VECTOR_HPP
#define OUT_VECTOR_HPP

#include <vector>

/**
 * The out vector implements vector-based, out-edge storage for directed graphs.
 * As an out-edge store, this type stores an edge property with the descriptor
 * referencing the adjacenct vertex. Vector-based stores support fast inserts, 
 * slow finds, and do not allow remove operations.
 *
 * Here, the edge is required to be a pair containing a vertex descriptor and a
 * edge property.
 */
template <typename Edge, typename Alloc>
class out_vector
{
    typedef std::vector<Edge, Alloc> store_type;
public:
    typedef Edge out_pair;
    typedef typename Edge::first_type vertex_descriptor;
    typedef typename Edge::second_type edge_properties;
    
    typedef typename store_type::const_iterator const_iterator;
    typedef typename store_type::size_type size_type;

    inline out_vector()
        : _store()
    { }

private:
    store_type _store;
};

#endif
