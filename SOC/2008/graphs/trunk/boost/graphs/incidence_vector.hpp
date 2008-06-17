
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
template <typename IncEdge, typename Alloc>
class incidence_vector
{
    typedef std::vector<IncEdge, Alloc> store_type;
public:
    typedef IncEdge incidence_pair;
    typedef typename IncEdge::first_type vertex_descriptor;
    typedef typename IncEdge::second_type property_descriptor;

    typedef typename store_type::iterator iterator;
    typedef typename store_type::const_iterator const_iterator;
    typedef typename store_type::size_type size_type;

    // Constructors
    incidence_vector();

    void add(incidence_pair);

    std::pair<const_iterator, bool> allow(vertex_descriptor) const;
    iterator find(incidence_pair);
    const_iterator find(incidence_pair) const;

    inline size_type size() const
    { return _edges.size(); }

    inline const_iterator begin() const
    { return _edges.begin(); }

    inline const_iterator end() const
    { return _edges.end(); }

private:
    store_type _edges;
};

#define BOOST_GRAPH_IV_PARAMS \
    typename E, typename A

template <BOOST_GRAPH_IV_PARAMS>
incidence_vector<E,A>::incidence_vector()
    : _edges()
{ }

template <BOOST_GRAPH_IV_PARAMS>
void
incidence_vector<E,A>::add(incidence_pair e)
{
    _edges.push_back(e);
}

/**
 * Incidence vectors always allow the addition of edges, assuming that no
 * policy conflicts exist. The first element of the return is the end() of the
 * vector.
 *
 * @complexity O(1)
 */
template <BOOST_GRAPH_IV_PARAMS>
std::pair<typename incidence_vector<E,A>::const_iterator, bool>
incidence_vector<E,A>::allow(vertex_descriptor v) const
{
    // Since there's no policy, there we must be able to add the edge.
    return make_pair(_edges.end(), true);
}


#undef BOOST_GRAPH_IV_PARAMS

#if 0

// Functions

template <typename E>
vertex_edge_vector<E>::vertex_edge_vector()
    : base_type()
{ }

template <typename E>
void
vertex_edge_vector<E>::add(edge_descriptor e)
{
    this->_store.push_back(e);
}

template <typename E>
typename vertex_edge_vector<E>::iterator
vertex_edge_vector<E>::find(edge_descriptor e)
{
    return std::find(this->_store.begin(), this->_store.end(), e);
}

template <typename E>
typename vertex_edge_vector<E>::const_iterator
vertex_edge_vector<E>::find(edge_descriptor e) const
{
    return std::find(this->_store.begin(), this->_store.end(), e);
}

template <typename E>
void
vertex_edge_vector<E>::remove(edge_descriptor e)
{
    this->_store.erase(find(e));
}

template <typename E>
void
vertex_edge_vector<E>::clear()
{
    this->_store.clear();
}

template <typename E>
typename vertex_edge_vector<E>::size_type
vertex_edge_vector<E>::size() const
{
    return this->_store.size();
}

#endif

#endif
