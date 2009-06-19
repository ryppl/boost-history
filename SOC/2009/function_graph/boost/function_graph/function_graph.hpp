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
#include <boost/range.hpp>

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

    /** Constructors to allow for initialization of edge */
    function_graph_base(function_type const& f)
        : edge_(f)
    { }

    // Allow access to the function edge_ holds, not edge_ itself.
    result_type edge (vertex_type v1, vertex_type v2) const
    { return edge_(v1, v2); }

    function_type edge_;
};

/** @name no_domain
 * A trait of function_graph used to declare a function_graph with no domain. 
 * @note is there somewhere else I could put this? Perhaps a traits file?
 */

struct no_domain { };



/** @name function_graph_in_edge_iterator
 * Iterates through the in edges of a vertex
 * @note The name of this data type is 31 characters long. I need some brevity.
 */

struct function_graph_in_edge_iterator { };



/**
 * Empty function graph prevents instantiations such as function_graph<int> and
 * function_graph<bool (int, int)>.
 */

template <typename T, typename U = no_domain> struct function_graph { };



/**
 * function_graph is a data structure that implements implicit graphs and more.
 * Better documentation and explanation of the data structure to come.
 *
 * @internal The typical user of function graph may not need to change edge
 * function during execution. However, since the code needed is trivial,
 * set_edge is part of the interface. Paired with it is the default constructor.
 */

template <typename Result, typename Vertex, typename Range>
struct function_graph<function<Result(Vertex, Vertex)>, Range>
    : public function_graph_base<function<Result(Vertex, Vertex)> >
{
private:
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
    typedef Range vertex_iterator_range_type;
    typedef typename range_iterator<vertex_iterator_range_type>::type
                         vertex_iterator;
    typedef function_graph_in_edge_iterator in_edge_iterator;

    /** Constructor: takes a functor and range */
    function_graph(function_type const& f, vertex_iterator_range_type const& r)
        : Base(f), range_(r)
    { }

private:
    Range range_;
};

// Specialization: function_graph without range
template <typename Result, typename Vertex>
struct function_graph<function<Result(Vertex, Vertex)>, no_domain>
    : public function_graph_base<function<Result(Vertex, Vertex)> >
{
private:
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

    /** Constructor: takes a boost::function or functor */
    function_graph(function_type const& f)
        : Base(f)
    { }
};



/** @name make_function_graph
 * Create and return a function_graph object deduced from the function arguments
 * passed to it.
 */

//@{
template <typename Result, typename Vertex>
function_graph<function<Result(Vertex, Vertex)>, no_domain>
    make_function_graph(function<Result(Vertex, Vertex)> const& f)
{
    return function_graph<function<Result(Vertex, Vertex)>, no_domain>(f);
}

template <typename Result, typename Vertex, typename Range>
function_graph<function<Result(Vertex, Vertex)>, Range>
    make_function_graph(function<Result(Vertex, Vertex)> const& f,
                        Range const& r)
{
    return function_graph<function<Result(Vertex, Vertex)>, Range>(f,r);
}
//@}

/**
 * @note This specialization will match any function of the form E(V,V) and
 * generates the graph over an adapted boost function. Note that functions of
 * the form E(U,V) will match the empty class causing compiler errors.
 */

template <typename Result, typename Vertex>
struct function_graph <Result(Vertex, Vertex)>
    : private function_graph_base <function<Result(Vertex, Vertex)> >
{ };



/** source(e, g) and target(e, g) are part of the incedence graph concept. */

template <typename Result, typename Vertex, typename Range>
Vertex source(detail::func_graph_edge<Result, Vertex> const& e,
              function_graph<function<Result(Vertex, Vertex)>, Range > const& g)
{
    return e.source;
}

template <typename Result, typename Vertex, typename Range>
Vertex target(detail::func_graph_edge<Result, Vertex> const& e,
              function_graph<function<Result(Vertex, Vertex)>, Range > const& g)
{
    return e.target;
}



/** in_edges(v, g) is part of the bidirectional graph concept. */

template <typename Result, typename Vertex, typename Range>
std::pair<typename function_graph<function<Result(Vertex, Vertex)>,
          Range>::in_edge_iterator,
          typename function_graph<function<Result(Vertex, Vertex)>,
          Range>::in_edge_iterator>
in_edges(typename function_graph<function<Result(Vertex, Vertex)>,
         Range>::vertex_descriptor const& v,
         function_graph<function<Result(Vertex, Vertex)>, Range> const& g)
{
    std::pair<typename function_graph<function<Result(Vertex, Vertex)>,
              Range>::in_edge_iterator,
              typename function_graph<function<Result(Vertex, Vertex)>,
              Range>::in_edge_iterator> iter_range;
    // Search for first and last in_edges
    return iter_range;
}



/** vertices(g) is part of the vertex list concept. */

template <typename Result, typename Vertex, typename Range>
std::pair<typename function_graph<function<Result(Vertex, Vertex)>,
          Range>::vertex_iterator,
          typename function_graph<function<Result(Vertex, Vertex)>,
          Range>::vertex_iterator>
vertices(function_graph<function<Result(Vertex, Vertex)>, Range> const& g) {
    return std::make_pair(begin(g.range_), end(g.range_));
}



/** @name bind_edge
 * edge(u, v, g) is part of the adjacency matrix concept called Direct Edge
 * Access. The function must account for edges that already return. There is
 * specialization to account for functions that use bool and optional<T>.
 */

//@{
namespace detail {

template <typename Result, typename Vertex>
std::pair<detail::func_graph_edge<Result, Vertex>, bool>
bind_edge(Result const& x, Vertex u, Vertex v)
{
    return std::make_pair(detail::func_graph_edge<Result, Vertex>(x, u, v),
                          true);
}

template <typename Vertex>
std::pair<detail::func_graph_edge<bool, Vertex>, bool>
bind_edge(bool x, Vertex u, Vertex v)
{
    return std::make_pair(typename detail::func_graph_edge
                              <bool, Vertex>(x, u, v),
                          x);
}

// This overload is specific to optional<T>
template <typename OptType, typename Vertex>
std::pair<detail::func_graph_edge<optional<OptType>, Vertex>, bool>
bind_edge(optional<OptType> const& x, Vertex u, Vertex v)
{
    return std::make_pair(detail::func_graph_edge
                              <optional<OptType>, Vertex>(x, u, v),
                          (bool)x);
}

}   // detail namespace

#define FUNC_GRAPH function_graph<function<Result(Vertex, Vertex)>, Range>

template <typename Result, typename Vertex, typename Range>
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

#undef FUNC_GRAPH
//@}

}   // boost namespace

#endif /*FUNCTION_GRAPH_HPP_*/

