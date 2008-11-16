
#ifndef BOOST_GRAPHS_ADJLIST_PROPERTY_LIST_HPP
#define BOOST_GRAPHS_ADJLIST_PROPERTY_LIST_HPP

#include <list>
#include <algorithm>

#include <boost/descriptors.hpp>
#include <boost/graphs/utility.hpp>

namespace boost { namespace graphs { namespace adjacency_list {

/**
 * The property list implements global list of label for node-based edge
 * storage. Note that we can get away with only a list because the edge
 * addition logic is implemented by the incidence list.
 *
 * The property store actually maintains the number of elements internally.
 * Because this store is built over a list, but only allows the insertion and
 * removal of one element at a time, we do this to optimize calls to the size()
 * function (which is used for the number of edges).
 *
 * Note that the edge pair is actually a set of descriptors into the incidence
 * lists of the vertices that reference this edge property.
 */
template <typename Edge, typename Alloc>
class property_list
{
public:
    typedef std::list<Edge, Alloc> store_type;

    typedef Edge edge_type;
    typedef typename Edge::first_type edge_label;
    typedef typename Edge::second_type end_pair;
    typedef typename end_pair::first_type end_type; // same as second_type
    typedef typename end_type::first_type vertex_descriptor;
    typedef typename end_type::second_type incidence_descriptor;

    typedef typename store_type::iterator iterator;
    typedef typename store_type::size_type size_type;

    typedef typename descriptor_traits<store_type>::descriptor_type label_descriptor;

    // Constructors.
    inline property_list()
        : _props(), _size(0)
    { }

    /** @name Add Property
     * Add a property tot the global set, leaving the incidence descriptors
     * empty for the time being.
     */
    //@{
    inline label_descriptor add()
    { return add(edge_label()); }

    inline label_descriptor add(edge_label const& ep)
    {
        ++_size;
        iterator i = _props.insert(_props.end(), make_pair(ep, end_pair()));
        return make_descriptor(_props, i);
    }
    //@}

    /**
     * Find the edge with the given label. Finding an edge by its
     * label is guaranteed to be O(E).
     */
    inline label_descriptor find(edge_label const& ep) const
    {
        iterator i = std::find_if(_props.begin(), _props.end(), find_first(ep));
        return make_descriptor(_props, i);
    }

    /** Remove the described property from the property set. */
    inline void remove(label_descriptor d)
    {
        _props.erase(make_iterator(_props, d));
        --_size;
    }

    /** Return the number of label. */
    inline size_type size() const
    { return _size; }

    /** Return true if this is empty. */
    inline bool empty() const
    { return _props.empty(); }

    /** @name Iterators */
    //@{
    inline iterator begin() const
    { return _props.begin(); }

    inline iterator end() const
    { return _props.end(); }
    //@}

    /**
     * Bind vertex descriptors into the incidence lists into the global
     * property. This is the last step of edge creation for undirected graphs.
     */
    void bind(label_descriptor d, end_pair const& p)
    { make_iterator(_props, d)->second = p; }

    /** Return the incidence descriptors bound to the property. */
    inline end_pair const& ends(label_descriptor d) const
    { return make_iterator(_props, d)->second; }

    /** Return the label referenced by the given descriptor. */
    inline edge_label& label(label_descriptor d)
    { return make_iterator(_props, d)->first; }

    /** Return the first vertex of the edge. */
    inline vertex_descriptor first_vertex(label_descriptor d) const
    { return make_iterator(_props, d)->second.first.first; }

    /** Return the second vertex of the edge. */
    inline vertex_descriptor second_vertex(label_descriptor d) const
    { return make_iterator(_props, d)->second.second.first; }

    /** Return the first incidence descriptor of the edge. */
    inline incidence_descriptor first_edge(label_descriptor d) const
    { return make_iterator(_props, d)->second.first.second; }

    /** Return the second incidence descriptor of the edge. */
    inline incidence_descriptor second_edge(label_descriptor d) const
    { return make_iterator(_props, d)->second.second.second; }


    /** Return a descriptor for the iterator. */
    inline label_descriptor describe(iterator i) const
    { return make_descriptor(_props, i); }

private:
    mutable store_type  _props;
    size_type           _size;
};

} } } /* namespace boost::graphs::adjacency_list */

#endif

