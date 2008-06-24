
#ifndef INCIDENCE_VECTOR
#define INCIDENCE_VECTOR

#include <vector>

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
    typedef std::vector<Edge, Alloc> store_type;
public:
    typedef Edge incidence_pair;
    typedef typename Edge::first_type vertex_descriptor;
    typedef typename Edge::second_type property_descriptor;

    typedef typename store_type::iterator iterator;
    typedef typename store_type::size_type size_type;

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
    std::pair<iterator, bool> allow(vertex_descriptor) const
    { return make_pair(_edges.end(), true); }

    /** Add the incidence pair to the vector. */
    void add(incidence_pair p)
    { _edges.push_back(p); }

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
