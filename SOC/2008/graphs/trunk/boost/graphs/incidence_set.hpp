
#ifndef INCIDENCE_SET_HPP
#define INCICENCE_SET_HPP

#include <map>

#include <boost/descriptors.hpp>

/**
 * The incidence vector stores incident "edges" of a vertex. In actuality,
 * this stores pairs containing an adjacent vertex descriptor and a property
 * descriptor, that points to the edges global properties. This pair uniquely
 * identifies incident edges of the vertex.
 *
 * This type allows logarithmic time insertions, searches, and removals.
 */
template <typename Edge, typename Compare, typename Alloc>
class incidence_set
{
public:
    typedef typename Edge::first_type vertex_descriptor;
    typedef typename Edge::second_type property_descriptor;

    // Actually, the incident set, as it fundamentally implements a simple
    // graph is based on a map keyed on the adjacenct vertex and mapped to the
    // edge properties that describe it. We're basically undwinding and
    // rebuilding the edge pair for this map.
    // NOTE: This can impose some difficulties since the vertex (key type) will
    // be made const in this map. That means we may have to cast out the const
    // aspect of the key at times, but changing that value would be absolutely
    // catastrophic.
    typedef std::map<vertex_descriptor, property_descriptor, Compare, Alloc> store_type;

    typedef typename store_type::iterator iterator;
    typedef typename store_type::size_type size_type;

    typedef typename descriptor_traits<store_type>::descriptor_type incidence_descriptor;

    // Constructors
    inline incidence_set()
        : _edges()
    { }

    /**
     * Deteremine whether or not the edge exists or is even allowed to be added.
     * This returns a pair containing an iterator indicating the position of the
     * edge if it already exists and a bool value indicating whether or not the
     * addition would even be allowed by policy.
     * @complexity O(lg(dd))
     */
    inline std::pair<incidence_descriptor, bool> allow(vertex_descriptor v) const
    { return make_pair(make_descriptor(_edges, _edges.find(v)), true); }

    /**
     * Add the incidence pair to the vertex.
     * @complexity O(lg(d))
     */
    inline incidence_descriptor add(vertex_descriptor v, property_descriptor p)
    {
        std::pair<iterator, bool> i = _edges.insert(make_pair(v, p));
        return make_descriptor(_edges, i.first);
    }

    /**
     * Find the incident edge whose opposite end is v.
     * @complexity O(lg(d))
     */
    inline incidence_descriptor find(vertex_descriptor v) const
    { return make_descriptor(_edges, _edges.find(v)); }

    /**
     * Remove the edge whose opposite end is v.
     * @complexity O(lg(d))
     */
    inline void remove(incidence_descriptor d)
    { _edges.erase(make_iterator(_edges, d)); }

    /** Remove all edges incident to this vertex. */
    inline void clear()
    { _edges.clear(); }

    /** Return the number of edges incident to this vertex. */
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
