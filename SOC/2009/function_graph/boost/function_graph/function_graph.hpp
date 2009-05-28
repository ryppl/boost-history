/**
 * Copyright (C) 2009
 * Michael Lopez
 *
 * Issued under Boost Software License - Version 1.0
 * http://www.boost.org/LICENSE_1_0.txt
 *
 */



#ifndef FUNCTION_GRAPH_HPP_
#define FUNCTION_GRAPH_HPP_

#include <boost/function.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/mpl/if.hpp>

namespace boost { namespace graph {

// [asutton] You're going about this in slightly the wrong way...

/**
 * function_graph_base handles the edge function. A user can define the function
 * through a boost::function or a functor, but the user must only have access to
 * this function through function graph via the function edge.
 *
 * @note [asutton] Don't try to deduce what might be passed to this function.
 * Just require that F be of the form boost::function<R,T,T>. Use the derived
 * classes to put the type in the right form.
 */
template <typename Func>
struct function_graph_base {
    // If the type F has an arity of 2, then it is the boost function syntax.
    // Otherwise, this function is either a functor or a boost function that is
    // already defined.
//     typedef typename mpl::if_c<function_types::function_arity<F>::value == 2,
//                                function<F>,
//                                F>::type func_type;

    typedef Func function_type;
    typedef typename Func::result_type edge_type;
    typedef typename Func::argument_type vertex_type;

    /** Edge function that defines the implicit graph */
    function_type func;
};

/**
 * @note [asutton] I'm defining an empty function graph so that we can create
 * specializations that force the user to provide T in certain forms. This
 * variant will be instantiated for meaningless types such as graph<int>.
 * Obviously, that should fail.
 */
template <typename T> struct functon_graph { };

/**
 * @note [asutton] This specialization will be instantiated whenever the
 * user tries to make something like graph< function<bool(int,int)> >. Note
 * the subtelty. If you write: graph<function<bool(int, double)> >, then
 * the compiler will match the version above, causing compiler errors.
 */
template <typename Edge, typename Vertex>
struct function_graph< function<Edge(Vertex, Vertex)> >
    : function_graph_base< function<Edge(Vertex, Vertex)> >
{ };

/**
 * @note This specialization will match any function of the form E(V,V) and
 * generates the graph over an adapted boost function. Note that functions of
 * the form E(U,V) will match the empty class causing compiler errors.
 *
template <typename Edge, typename Vertex>
struct fucntion_graph<Edge(Vertex, Vertex)>
    : function_graph_base< function<Edge(Vertex, Vertex)> >
{ };

}   // graph namespace
}   // boost namespace

#endif /*FUNCTION_GRAPH_HPP_*/

