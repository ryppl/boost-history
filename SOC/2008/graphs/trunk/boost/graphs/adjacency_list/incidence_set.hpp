
#ifndef BOOST_GRAPHS_ADJLIST_INCIDENCE_SET_HPP
#define BOOST_GRAPHS_ADJLIST_INCIDENCE_SET_HPP

#include <map>

#include <boost/descriptors.hpp>

namespace boost { namespace graphs { namespace adjacency_list {

/**
 * The incidence vector stores incident "edges" of a vertex. In actuality,
 * this stores pairs containing an adjacent vertex descriptor and a property
 * descriptor, that points to the edges global label. This pair uniquely
 * identifies incident edges of the vertex.
 *
 * This type allows logarithmic time insertions, searches, and removals.
 */
template <typename Edge, typename Compare, typename Alloc>
class incidence_set
{
public:
    typedef typename Edge::first_type vertex_descriptor;
    typedef typename Edge::second_type label_descriptor;

    // Actually, the incident set, as it fundamentally implements a simple
    // graph is based on a map keyed on the adjacenct vertex and mapped to the
    // edge label that describe it. We're basically undwinding and
    // rebuilding the edge pair for this map.
    // NOTE: This can impose some difficulties since the vertex (key type) will
    // be made const in this map. That means we may have to cast out the const
    // aspect of the key at times, but changing that value would be absolutely
    // catastrophic.
    typedef std::map<vertex_descriptor, label_descriptor, Compare, Alloc> store_type;
    typedef typename store_type::iterator iterator;
    typedef typename store_type::size_type size_type;

    typedef typename descriptor_traits<store_type>::descriptor_type incidence_descriptor;

    // Constructors
    inline incidence_set()
        : _edges()
    { }

    /** @name Add Incident Edge.
     * Try adding the incident edge to the vertex. The first version is used in
     * a two-step edge addition where the property descriptor is bound in the
     * second phase of the insertion.
     * @complexity O(lg(d))
     */
    //@{
    inline insertion_result<incidence_descriptor> add(vertex_descriptor v)
    { return add(v, label_descriptor()); }

    insertion_result<incidence_descriptor> add(vertex_descriptor v, label_descriptor p)
    {
        std::pair<iterator, bool> i = _edges.insert(make_pair(v, p));
        return make_result(_edges, i);
    }
    //@}

    /**
     * Find the incident edge whose opposite end is v.
     * @complexity O(lg(d))
     */
    incidence_descriptor find(vertex_descriptor v) const
    { return make_descriptor(_edges, _edges.find(v)); }

    /**
     * Remove the edge whose opposite end is v.
     * @complexity O(lg(d))
     */
    void remove(incidence_descriptor d)
    { _edges.erase(make_iterator(_edges, d)); }

    /** Remove all edges incident to this vertex. */
    void clear()
    { _edges.clear(); }

    /** Return the number of edges incident to this vertex. */
    inline size_type size() const
    { return _edges.size(); }

    /** Return true if this is empty. */
    inline bool empty() const
    { return _edges.empty(); }

    inline bool valid(incidence_descriptor d)
    { return make_iterator(_edges, d) != _edges.end(); }

    /** @name Iterators */
    //@{
    inline iterator begin() const
    { return _edges.begin(); }

    inline iterator end() const
    { return _edges.end(); }
    //@}

    /** Bind this incident edge to the given property. */
    inline void bind(incidence_descriptor d, label_descriptor p)
    { make_iterator(_edges, d)->second = p; }

    /** Return the vertex connected by this edge. */
    inline vertex_descriptor vertex(incidence_descriptor d) const
    { return make_iterator(_edges, d)->first; }

    /** Return the label connected by this edge. */
    inline label_descriptor label(incidence_descriptor d) const
    { return make_iterator(_edges, d)->second; }

private:
    mutable store_type _edges;
};

} } } /* namespace boost::graphs::adjacency_list */

#endif
