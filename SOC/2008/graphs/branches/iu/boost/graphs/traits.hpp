
#ifndef TRAITS_HPP
#define TRAITS_HPP

namespace detail
{
    // This isn't quite so simple as it seems. I think we need to have the
    // catch-all push the query back to a policy object and that policy needs
    // to be explicit about whether or not it allows multiple edges.
    template <typename Store>
    struct is_simple
    { enum { value = false }; };

    template <typename E, typename C, typename A>
    struct is_simple< incidence_set<E,C,A> >
    { enum { value = true }; };

};

/**
 * This metafunction determines if the graph allows multiple edges.
 */
template <typename Graph>
struct is_simple
{ enum { value = detail::is_simple<typename Graph::incidence_store>::value }; };

/**
 * A helper function for the above.
 */
template <typename Graph>
bool is_simple_graph(const Graph& g)
{ return is_simple<Graph>::value; }

#endif
