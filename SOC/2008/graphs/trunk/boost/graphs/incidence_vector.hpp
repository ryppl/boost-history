
#ifndef INCIDENCE_VECTOR
#define INCIDENCE_VECTOR

#include <vector>
#include <algorithm>

#include <boost/descriptors.hpp>
#include <boost/graphs/utility.hpp>

/**
 * The incidence vector stores incident "edges" of a vertex. In actuality,
 * this stores pairs containing an adjacent vertex descriptor and a property
 * descriptor, that points to the edges global properties. This pair uniquely
 * identifies incident edges of the vertex.
 *
 * This type allows constant-time edge addition and a linear search. Removal
 * is not supported.
 */
template <typename Edge, typename Alloc>
class incidence_vector
{
public:
    typedef typename Edge::first_type vertex_descriptor;
    typedef typename Edge::second_type property_descriptor;

    typedef std::vector<Edge, Alloc> store_type;
    typedef typename store_type::iterator iterator;
    typedef typename store_type::size_type size_type;

    typedef typename descriptor_traits<store_type>::descriptor_type incidence_descriptor;

    // Constructors
    inline incidence_vector()
        : _edges()
    { }

    /** @name Add Edge
     * Add the incidence pair to the vector. The first version is responsible
     * for adding a "stub" that is completed later by binding a property
     * descriptor into the edge.
     */
    //@{
    inline insertion_result<incidence_descriptor> add(vertex_descriptor v)
    { return add(v, property_descriptor()); }

    insertion_result<incidence_descriptor> add(vertex_descriptor v, property_descriptor p)
    {
        iterator i = _edges.insert(_edges.end(), std::make_pair(v, p));
        return make_result(make_descriptor(_edges, i));
    }
    //@}

    /** Find the edge with the given vertex. */
    incidence_descriptor find(vertex_descriptor v) const
    {
        iterator i = std::find_if(_edges.begin(), _edges.end(), find_first(v));
        return make_descriptor(_edges, i);
    }

    /** Return the number of edges in this store. */
    inline size_type size() const
    { return _edges.size(); }

    /** Return true if this is empty. */
    inline bool empty() const
    { return _edges.empty(); }

    /** @name Iterators */
    //@{
    inline iterator begin() const
    { return _edges.begin(); }

    inline iterator end() const
    { return _edges.end(); }
    //@}

    /** @name Property Access
     * These functions provide a descriptor-based interface to the property
     * descriptors contained within the edge record. Binding binds the property
     * to an edge stub, and the properties function returns the bound property
     * descriptor.
     */
    //@{
    inline void bind(incidence_descriptor d, property_descriptor p)
    { make_iterator(_edges, d)->second = p; }

    inline property_descriptor properties(incidence_descriptor d) const
    { return make_iterator(_edges, d)->second; }
    //@}


private:
    mutable store_type _edges;
};

#endif
