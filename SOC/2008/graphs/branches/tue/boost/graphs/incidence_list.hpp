
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_INCIDENCE_LIST_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_INCIDENCE_LIST_HPP

#include <list>
#include <algorithm>

template <typename IncEdge, typename Alloc>
class incidence_list
{
    typedef std::list<IncEdge, Alloc> store_type;
public:
    typedef IncEdge incidence_pair;
    typedef typename IncEdge::first_type vertex_descriptor;
    typedef typename IncEdge::second_type property_descriptor;

    typedef typename store_type::iterator iterator;
    typedef typename store_type::const_iterator const_iterator;
    typedef typename store_type::size_type size_type;

    // Constructors
    incidence_list();

    void add(incidence_pair);

    iterator find(incidence_pair);
    const_iterator find(incidence_pair) const;

    void remove(incidence_pair);
    void clear();

    size_type size() const;

private:
    store_type _edges;
};

#define BOOST_GRAPHS_IL_PARAMS \
    typename IE, typename A

template <BOOST_GRAPHS_IL_PARAMS>
incidence_list<IE,A>::incidence_list()
    : _edges()
{ }

#if 0

// Functions

template <typename E>
vertex_edge_list<E>::vertex_edge_list()
    : base_type()
{ }

template <typename E>
void
vertex_edge_list<E>::add(edge_descriptor e)
{
    this->_store.push_back(e);
}

template <typename E>
typename vertex_edge_list<E>::iterator
vertex_edge_list<E>::find(edge_descriptor e)
{
    return std::find(this->_store.begin(), this->_store.end(), e);
}

template <typename E>
typename vertex_edge_list<E>::const_iterator
vertex_edge_list<E>::find(edge_descriptor e) const
{
    return std::find(this->_store.begin(), this->_store.end(), e);
}

template <typename E>
void
vertex_edge_list<E>::remove(edge_descriptor e)
{
    this->_store.erase(find(e));
}

template <typename E>
void
vertex_edge_list<E>::clear()
{
    this->_store.clear();
}

template <typename E>
typename vertex_edge_list<E>::size_type
vertex_edge_list<E>::size() const
{
    return this->_store.size();
}

#endif

#endif
