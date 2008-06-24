
#ifndef INCIDENCE_SET_HPP
#define INCICENCE_SET_HPP

#include <map>

#include <boost/next_prior.hpp>

/**
 * The incidence vector stores incident "edges" of a vertex. In actuality,
 * this stores pairs containing an adjacent vertex descriptor and a property
 * descriptor, that points to the edges global properties. This pair uniquely
 * identifies incident edges of the vertex.
 *
 * This type allows logarithmic time insertions, searches, and removals.
 */
template <typename IncEdge, typename Compare, typename Alloc>
class incidence_set
{
public:
    typedef IncEdge incidence_pair;
    typedef typename IncEdge::first_type vertex_descriptor;
    typedef typename IncEdge::second_type property_descriptor;
private:
    // Actually, the incident set, as it fundamentally implements a simple
    // graph is based on a map keyed on the adjacenct vertex and mapped to the
    // edge properties that describe it. We're basically undwinding and
    // rebuilding the edge pair for this map.
    typedef std::map<vertex_descriptor, property_descriptor, Compare, Alloc> store_type;
public:
    typedef typename store_type::iterator iterator;
    typedef typename store_type::size_type size_type;

    // Constructors
    inline incidence_set()
        : _edges()
    { }

    /**
     * Deteremine whether or not the edge exists or is even allowed to be added.
     * This returns a pair containing an iterator indicating the position of the
     * edge if it already exists and a bool value indicating whether or not the
     * addition would even be allowed by policy.
     * @complexity O(lg(deg(v)))
     */
    inline std::pair<iterator, bool> allow(vertex_descriptor v) const
    { return make_pair(_edges.find(v), true); }

    /**
     * Add the incidence pair to the vertex.
     * @complexity O(lg(deg(v)))
     */
    inline void add(incidence_pair p)
    { _edges.insert(p); }

    /**
     * Find the incidence pair.
     * @complexity O(lg(deg(v)))
     */
    inline iterator find(incidence_pair p) const
    { return _edges.find(p.first); }

    /**
     * Remove the incidence pair from the set.
     * @complexity O(lg(deg(v)))
     */
    inline void remove(incidence_pair p)
    { _edges.erase(find(p)); }

    /**
     * Remove the iterator to the given incidence pair, returning an iterator
     * to the next object in the sequence.
     * @complexity O(lg(deg(v)))
     */
    inline iterator remove(iterator x)
    {
        iterator ret = boost::next(x);
        _edges.erase(x);
        return ret;
    }

    // Remove edges.
    inline void clear()
    { _edges.clear(); }

    inline size_type size() const
    { return _edges.size(); }

    inline iterator begin() const
    { return _edges.begin(); }

    inline iterator end() const
    { return _edges.end(); }

private:
    mutable store_type _edges;
};

#endif
