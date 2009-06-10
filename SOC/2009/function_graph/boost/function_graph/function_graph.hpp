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

#include <boost/graph/graph_traits.hpp>
#include <boost/function.hpp>
#include <boost/function_types/function_arity.hpp>
#include <utility>
#include <boost/optional/optional_fwd.hpp>

namespace boost {

namespace detail {

/** Edge type */
template <typename Result, typename Vertex>
struct func_graph_edge {
    typedef Result result_type;
    typedef Vertex vertex_descriptor;

    func_graph_edge(result_type rslt,
                    vertex_descriptor src,
                    vertex_descriptor trg)
        : result(rslt), source(src), target(trg)
    { }

    result_type result;
    vertex_descriptor source;
    vertex_descriptor target;
};

}   // detail namespace

/**
 * function_graph_base handles the edge function. A user can define the function
 * as a boost::function, but the user may only have access to this function
 * through the function graph via edge().
 */
template <typename Func>
struct function_graph_base {

    typedef Func function_type;
    typedef typename function_type::first_argument_type vertex_type;
    typedef typename function_type::result_type result_type;
    typedef typename detail::func_graph_edge<result_type,
                                             vertex_type> edge_type;

    /** Constructor - Default */
    function_graph_base()
    { }

    /** Constructors to allow for initialization of edge */
    function_graph_base(function_type const& f)
        : edge_(f)
    { }

    // Allow access to the function edge_ holds, not edge_ itself.
    result_type edge (vertex_type v1, vertex_type v2) const
    { return edge_(v1, v2); }

    function_type edge_;
};



/**
 * Empty function graph prevents instantiations such as function_graph<int> and
 * function_graph<bool (int, int)>.
 */
template <typename T> struct function_graph { };

/**
 * function_graph is a data structure that implements implicit graphs and more.
 * Better documentation and explanation of the data structure to come.
 *
 * @internal The typical user of function graph may not need to change edge
 * function during execution. However, since the code needed is trivial,
 * set_edge is part of the interface. Paired with it is the default constructor.
 */
template <typename Result, typename Vertex>
struct function_graph<function<Result(Vertex, Vertex)> >
    : public function_graph_base<function<Result(Vertex, Vertex)> >
{
    typedef function_graph_base<function<Result(Vertex, Vertex)> > Base;
    typedef function_graph<function<Result(Vertex, Vertex)> > This;
public:

    typedef typename Base::function_type function_type;
    typedef typename Base::vertex_type vertex_descriptor;
    typedef typename Base::edge_type edge_descriptor;
    typedef typename Base::result_type result_type;
    typedef directed_tag directed_category;
    typedef disallow_parallel_edge_tag edge_parallel_category;
    typedef adjacency_matrix_tag traversal_category;

    /** Constructor: default */
    function_graph()
        : Base()
    { }

    /** Constructor: takes a boost::function or functor */
    function_graph(function_type const& f)
        : Base(f)
    { }

private:
    /** Edge function from Base */
    //using Base::edge;
};

/**
 * @note This specialization will match any function of the form E(V,V) and
 * generates the graph over an adapted boost function. Note that functions of
 * the form E(U,V) will match the empty class causing compiler errors.
 */

template <typename Result, typename Vertex>
struct function_graph <Result(Vertex, Vertex)>
    : private function_graph_base <function<Result(Vertex, Vertex)> >
{ };



/**
 * source(e, g) and target(e, g) are part of the incedence graph concept.
 */

template <typename Result, typename Vertex>
Vertex source(detail::func_graph_edge<Result, Vertex> const& e,
              function_graph<function<Result(Vertex, Vertex)> > const& g)
{
    return e.source;
}

template <typename Result, typename Vertex>
Vertex target(detail::func_graph_edge<Result, Vertex> const& e,
              function_graph<function<Result(Vertex, Vertex)> > const& g)
{
    return e.target;
}



/**
 * edge(u, v, g) is part of the adjacency matrix concept called Direct Edge
 * Access. The function must account for edges that already return. There is
 * specialization to account for functions that use bool and optional<T>.
 */

#define FG_EDGE_BOOL detail::func_graph_edge<Result, Vertex>

template <typename Vertex, typename Result>
std::pair<FG_EDGE_BOOL, bool> bind_edge(Result const& x, Vertex u, Vertex v)
{ return std::make_pair(FG_EDGE_BOOL(x, u, v), true); }

#undef FG_EDGE_BOOL
#define FG_EDGE_BOOL detail::func_graph_edge<bool, Vertex>

template <typename Vertex>
std::pair<FG_EDGE_BOOL, bool> bind_edge(bool x, Vertex u, Vertex v)
{ return std::make_pair(typename FG_EDGE_BOOL(x, u, v), x); }

#undef FG_EDGE_BOOL
#define FG_EDGE_BOOL detail::func_graph_edge<optional<OptType>, Vertex>

template <typename OptType, typename Vertex>
std::pair<FG_EDGE_BOOL, bool>
bind_edge(optional<OptType> const& x, Vertex u, Vertex v)
{ return std::make_pair(FG_EDGE_BOOL(x, u, v), (bool)x); }

#undef FG_EDGE_BOOL

#define FUNC_GRAPH function_graph<function<Result(Vertex, Vertex)> >

template <typename Result, typename Vertex>
std::pair<typename FUNC_GRAPH::edge_descriptor, bool>
edge(typename FUNC_GRAPH::vertex_descriptor u,
     typename FUNC_GRAPH::vertex_descriptor v,
     FUNC_GRAPH const& g)
{
    typedef FUNC_GRAPH graph_type;
    typedef typename FUNC_GRAPH::result_type result_type;
    result_type result = g.edge(u, v);
    return bind_edge(result, u, v);
}

}   // boost namespace

#endif /*FUNCTION_GRAPH_HPP_*/

