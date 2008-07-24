
#ifndef ALGORITHM_CONCEPTS_HPP
#define ALGORITHM_CONCEPTS_HPP

#if BOOST_HAS_CONCEPTS

// Why are there concepts for algorithm objects? It seems unlikely that anybody
// would ever build generic algorithms over generic algorithm objects. When is
// the algorithm replaceable?
//
// One reason is to help describe requirements template arguments to the
// algorithm iterator (which pretty much just requires an Algorithm, but it's
// nice to know what else might work there).
//
// The other reason is simply to provide a taxonomy of graph algorithm styles
// and what interfaces they /should/ support in order to be generically useful.\
// This is more a way of telling programmers: if you're using this algorithm,
// you have these options.

/**
 * The Algorithm concept describes common requirements on all algorithm objects.
 * Specifically, an algorithm object must provide a next() function that puts
 * the algorithm in the next logical state.
 *
 * The status() function returns an  object that encapsulates the current status
 * of the algorithm, but not necessarily the entire state of the algorithm.
 */
concept Algorithm<typename X>
{
    typename status_type;
    void X::next();
    status_type X::status() const;
};

/**
 * The vertex state concept requires that algorithms modeling this concept
 * provide access to the current vertex.
 */
concept VertexState<typename X>
{
    typename vertex_descriptor;
    vertex_descriptor X::current();
};

/**
 * The edge state concept requires that algorithms modeling this concept provide
 * access to the current edge.
 */
concept EdgeState<typename X>
{
    typename edge_descriptor;
    edge_descriptor X::current() const;
}

/**
 * The walk state is a refinemnet of edge state that denotes the movement across
 * the current edge, imposing source/target semantics on the edge, even when
 * none may exist. Algorithms that walk across edge states require that the
 * the edge descriptor be directional.
 */
concept WalkState<typename X> : EdgeState<X>
{
    typename vertex_descriptor;
    requires<DirectionalEdge<edge_descriptor>;
    vertex_descriptor Algorithm::source() const { source(this->current(); }
    vertex_descriptor Algorithm::target() const { target(this->current(); }
}

/**
 * A VertexAlgorithm is any algorithm that provides access to the current
 * vertex, and the status returned by this algorithm also provides access to the
 * current vertex.
 */
concept VertexAlgorithm<typename A> : Algorithm<A>, VertexState<A>
{
    requires VertexState<status_type>;
};

/**
 * An EdgeAlgorithm is any algorithm that provides access to the current
 * edge, and the status returned by this algorithm also provides access to the
 * current vertex.
 */
concept EdgeAlgorithm<typename A> : Algorithm<A>, EdgeState<A>
{
    requires EdgeState<status_type>;
};

/**
 * A WalkAlgorithm is any algorithm that provides access to the current state
 * of the walk (i.e., the current edge and its source and target vertices).
 */
concept WalkAlgorithm<typename A> : Algorithm<A>, WalkState<A>
{
    requires WalkState<status_type>;
};

#endif /* BOOST_HAS_CONCEPTS */

#endif
