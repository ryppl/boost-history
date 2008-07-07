
#ifndef INCIDENCE_VECTOR
#define INCIDENCE_VECTOR

#include <vector>
#include <algorithm>

#include <boost/descriptors.hpp>

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
    typedef std::vector<Edge, Alloc> store_type;

    typedef typename Edge::first_type vertex_descriptor;
    typedef typename Edge::second_type property_descriptor;

    typedef typename store_type::iterator iterator;
    typedef typename store_type::size_type size_type;

    typedef typename descriptor_traits<store_type>::descriptor_type incidence_descriptor;

    // Constructors
    inline incidence_vector()
        : _edges()
    { }

    /**
     * Incidence vectors always allow the addition of edges, assuming that no
     * policy conflicts exist. The first element of the return is the end() of
     * the vector.
     * @complexity O(1)
     */
    std::pair<incidence_descriptor, bool> allow(vertex_descriptor) const
    { return make_pair(incidence_descriptor(), true); }

    /** Add the incidence pair to the vector. */
    incidence_descriptor add(vertex_descriptor v, property_descriptor p)
    {
        iterator i = _edges.insert(_edges.end(), std::make_pair(v, p));
        return make_descriptor(_edges, i);
    }

    /** Find the edge with the given vertex. */
    inline incidence_descriptor find(vertex_descriptor v) const
    {
        iterator i = std::find_if(_edges.begin(), _edges.end(), find_first(v));
        return make_descriptor(_edges, i);
    }

    /** Return the number of edges in this store. */
    inline size_type size() const
    { return _edges.size(); }

    /** @name Iterators */
    //@{
    inline iterator begin() const
    { return _edges.begin(); }

    inline iterator end() const
    { return _edges.end(); }
    //@}

private:
    mutable store_type _edges;
};

#endif
