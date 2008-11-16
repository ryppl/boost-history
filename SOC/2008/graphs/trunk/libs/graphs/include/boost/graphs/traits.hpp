
#ifndef BOOST_GRAPHS_TRAITS_HPP
#define BOOST_GRAPHS_TRAITS_HPP

#include <boost/utility.hpp>
#include <boost/mpl/if.hpp>

#include "undirected_graph.hpp"
#include "directed_graph.hpp"

namespace boost { namespace graphs {

namespace detail
{
    template <typename Store>
    struct has_unique_elems
    { BOOST_STATIC_CONSTANT(bool, value = false); };

    template <typename E, typename C, typename A>
    struct has_unique_elems< incidence_set<E,C,A> >
    { BOOST_STATIC_CONSTANT(bool, value = true); };

    template <typename Graph>
    struct directed_parallel
    {
        BOOST_STATIC_CONSTANT(bool, value = false);
    };

    template <typename Graph>
    struct undirected_parallel
    {
        BOOST_STATIC_CONSTANT(
            bool,
            value = has_unique_elems<typename Graph::incidence_store>::value);
    };
}

/** @name Is Directed Trait
 * Determine if the graph is directed or undirected.
 */
//@{
template <typename Graph>
struct is_directed
{
    BOOST_STATIC_CONSTANT(bool, value = false);
};

template <typename VP, typename EP, typename VS, typename ES>
struct is_directed< directed_graph<VP,EP,VS,ES> >
{
    BOOST_STATIC_CONSTANT(bool, value = true);
};

template <typename VP, typename EP, typename VS, typename ES>
struct is_directed< undirected_graph<VP,EP,VS,ES> >
{
    BOOST_STATIC_CONSTANT(bool, value = false);
};

template <typename Graph>
inline bool is_directed_graph(Graph const& g)
{ return is_directed<Graph>::value; }

template <typename Graph>
inline bool is_undirected_graph(Graph const& g)
{ return !is_directed<Graph>::value; }
//@}

/** @name Has Parallels Edges Trait
 * Determine if the graph allows parallel edges. There are two ways that this
 * can be checked: either by the edge store or the policy.
 */
//@{
template <typename Graph>
struct has_parallel_edges
{
    typedef typename boost::mpl::if_<
            is_directed<Graph>,
            detail::directed_parallel<Graph>,
            detail::undirected_parallel<Graph>
        >::type test;

    BOOST_STATIC_CONSTANT(bool, value = test::value);
};

template <typename Graph>
inline bool allows_parallel_edges(const Graph& g)
{ return has_parallel_edges<Graph>::value; }
//@}

} } /* namespace boost::graphs */

#endif
