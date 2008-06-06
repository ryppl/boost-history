
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

    typedef none incidence_iterator;

    // Constructors
    incidence_vector();

    void add(incidence_pair);

    iterator find(incidence_pair);
    const_iterator find(incidence_pair) const;

    size_type size() const;

private:
    store_type _edges;
};

#define BOOST_GRAPHS_IV_PARAMS \
    typename IE, typename A

template <BOOST_GRAPHS_IV_PARAMS>
incidence_vector<IE,A>::incidence_vector()
    : _edges()
{ }

template <BOOST_GRAPHS_IV_PARAMS>
void
incidence_vector<IE,A>::add(incidence_pair e)
{
    _edges.push_back(e);
}

template <BOOST_GRAPHS_IV_PARAMS>
typename incidence_vector<IE,A>::size_type
incidence_vector<IE,A>::size() const
{
    return _edges.size();
}

#undef BOOST_GRAPHS_IV_PARAMS

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
