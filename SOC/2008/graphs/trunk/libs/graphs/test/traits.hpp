
#ifndef TRAITS_HPP
#define TRAITS_HPP

#include <boost/mpl/bool.hpp>

// Forward declare our common label types.
class node;
class arc;

/** @name Has Mapped Vertices */
//@{
template <typename Graph>
struct _has_mapped_vertices
{ typedef boost::mpl::false_ type; };

template <
    typename VL, typename EL,
    typename K, template <typename> class C, template <typename> class A,
    typename ES, typename IS>
struct _has_mapped_vertices<
    boost::graphs::adjacency_list::undirected_graph<
        VL, EL, boost::graphs::adjacency_list::vertex_map<K,C,A>, ES, IS
    >
>
{ typedef boost::mpl::true_ type; };

template <typename Graph>
typename _has_mapped_vertices<Graph>::type
has_mapped_vertices(Graph const& g)
{ return typename _has_mapped_vertices<Graph>::type(); }
//@}

/** @name Can Remove Vertices */
//@{
template <typename Graph>
struct _can_remove_vertices
{ typedef boost::mpl::true_ type; };

template <
    typename VL, typename EL,
    template <typename> class A,
    typename ES, typename IS>
struct _can_remove_vertices<
    boost::graphs::adjacency_list::undirected_graph<
        VL, EL, boost::graphs::adjacency_list::vertex_vector<A>, ES, IS
    >
>
{ typedef boost::mpl::false_ type; };

template <typename Graph>
typename _can_remove_vertices<Graph>::type
can_remove_vertices(Graph const& g)
{ return typename _can_remove_vertices<Graph>::type(); }
//@}


// Helper functions
namespace detail {
    template <typename Graph>
    typename Graph::vertex_descriptor
    do_make_vertex(Graph& g, int i, boost::mpl::false_)
    { return add_vertex(g, node(i)); }

    template <typename Graph>
    typename Graph::vertex_descriptor
    do_make_vertex(Graph& g, int i, boost::mpl::true_)
    { return add_vertex(g, i, node(i)); }

    template <typename Graph>
    typename Graph::vertex_descriptor
    make_vertex(Graph& g, int i)
    { return do_make_vertex(g, i, has_mapped_vertices(g)); }
}

#endif
