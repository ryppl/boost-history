
#ifndef UNDIRECTED_VERTEX_HPP
#define UNDIRECTED_VERTEX_HPP

#include <boost/graphs/adjacency_list/incidence_iterator.hpp>

/**
 * A vertex, at least for an undirected graph, is simply an repository for the
 * vertex's label and an interface for the its incidence list.
 *
 * For directed graphs, it's basically the same, but has an out edge list and/or
 * an in edge list, and the edge label are stored on the out edge list. The
 * default comparison of vertices always delegates the comparison to the stored
 * vertex label. This allows developers to express custom comparitors with
 * respect to the label and have the vertex sets or other vertex ordering
 * operations work as they might expect.
 *
 * @requires LessThanComparable<vertex_label>.
 */
template <typename VertexProps, typename IncStore>
class undirected_vertex
{
public:
    typedef VertexProps vertex_label;

    typedef IncStore incidence_store;
    typedef typename incidence_store::incidence_descriptor incidence_descriptor;
    typedef typename incidence_store::vertex_descriptor vertex_descriptor;
    typedef typename incidence_store::label_descriptor label_descriptor;
    typedef typename incidence_store::iterator iterator;
    typedef typename incidence_store::size_type size_type;

    inline undirected_vertex()
        : _label(), _edges()
    { }

    inline undirected_vertex(vertex_label const& vp)
        : _label(vp), _edges()
    { }

    /** @name Edge Connection and Disconnection
     * These functions control how edges are added to and removed from the
     * vertex.
     */
    //@{
    inline insertion_result<incidence_descriptor> connect(vertex_descriptor v)
    { return _edges.add(v); }

    inline void bind(incidence_descriptor i, label_descriptor p)
    { _edges.bind(i, p); }

    inline void disconnect(incidence_descriptor i)
    { _edges.remove(i); }

    /** Find an incident edge connecting this to the given vertex. */
    inline incidence_descriptor find(vertex_descriptor v) const
    { return _edges.find(v); }
    //@}

    /** Return the label of the given incident edge. */
    inline label_descriptor label(incidence_descriptor i) const
    { return _edges.label(i); }

    inline vertex_descriptor opposite(incidence_descriptor i) const
    { return _edges.vertex(i) ;}


    /** @name Property Accessors */
    //@{
    inline vertex_label& label()
    { return _label; }

    inline vertex_label const& label() const
    { return _label; }
    //@}

    /** @name Iterators */
    //@{
    inline iterator begin() const
    { return _edges.begin(); }

    inline iterator end() const
    { return _edges.end(); }
    //@}

    inline void clear()
    { _edges.clear(); }

    inline size_type degree() const
    { return _edges.size(); }

    inline bool operator==(undirected_vertex const& x) const
    { return _label == x._label; }

    inline bool operator!=(undirected_vertex const& x) const
    { return !this->operator==(x); }

    inline bool operator<(undirected_vertex const& x) const
    { return _label < x._label; }

private:
    vertex_label   _label;
    incidence_store     _edges;
};

#endif
