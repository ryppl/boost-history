/**
 * Copyright (C) 2009
 * Michael Lopez
 *
 * Issued under Boost Software License - Version 1.0
 * http://www.boost.org/LICENSE_1_0.txt
 *
 */

/** @todo Rethink the edge handling mechanism. The need to call a version of
 * bind edge or edge(u,v,g)
 */

#ifndef FUNCTION_GRAPH_HPP_
#define FUNCTION_GRAPH_HPP_

#include <boost/graph/graph_traits.hpp>
#include <boost/function.hpp>
#include <boost/function_types/function_arity.hpp>
#include <utility>
#include <boost/optional/optional_fwd.hpp>
#include <boost/range/iterator.hpp>
#include <iterator>

namespace boost {

/** @name Domain Tags
 * @description Traits that identify the function_graph as either having a
 * finite domain, a range, or having an infinite domain, no range.
 */

struct finite_domain_tag { };
struct infinite_domain_tag { };



/** @name Edge Descriptor */

namespace detail {

template <typename Result, typename Vertex>
struct func_graph_edge {
    typedef Result result_type;
    typedef Vertex vertex_descriptor;

    func_graph_edge(result_type result,
                    vertex_descriptor source,
                    vertex_descriptor target)
        : result_(result), source_(source), target_(target)
    { }

    result_type result_;
    vertex_descriptor source_;
    vertex_descriptor target_;
};

}   // detail namespace



/** @name Function Graph Base
 * @description function_graph_base handles the edge function. A user can
 * define the function as a boost::function, but the user may only have access
 * to this function through the function graph via edge().
 */

//@{
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

template <typename Func, typename Rng>
struct function_graph_base_range {

    typedef Func function_type;
    typedef typename function_type::first_argument_type vertex_type;
    typedef typename function_type::result_type result_type;
    typedef typename detail::func_graph_edge<result_type,
                                             vertex_type> edge_type;
    typedef Rng vertex_iterator_range;

    /** Constructors to allow for initialization of edge */
    function_graph_base_range(function_type const& f,
                              vertex_iterator_range const& r)
        : edge_(f), range_(r)
    { }

    // Allow access to the function edge_ holds, not edge_ itself.
    result_type edge (vertex_type v1, vertex_type v2) const
    { return edge_(v1, v2); }

    function_type edge_;
    vertex_iterator_range range_;
};
//@}



/** @name In-Edge Iterator
 * @description Iterates through the in edges of a vertex.
 */

template<typename Function, typename Edge, typename Range>
struct function_graph_in_edge_iterator {
private:
    typedef function_graph_in_edge_iterator<Function, Edge, Range> This;

public:
    typedef Range vertex_iterator_range;
    typedef typename range_iterator<vertex_iterator_range>::type
            vertex_iterator;
    typedef Edge edge_descriptor;
    typedef Function function_type;

    /** Iterator traits */
    typedef std::input_iterator_tag iterator_category;
    typedef edge_descriptor value_type;
    typedef std::size_t different_type;
    typedef value_type* pointer;
    typedef value_type& reference;

    /** @internal Constructors */
    //@{
    function_graph_in_edge_iterator(function_type const& f,
                                    vertex_iterator const& v,
                                    vertex_iterator const& v_begin,
                                    vertex_iterator const& v_end)
        : edge_(f),
          vertex_(v),
          vertex_begin_(v_begin),
          vertex_end_(v_end)
    { };

    // Copy constructor
    function_graph_in_edge_iterator(This const& cp)
        : edge_(cp.edge_),
          vertex_(cp.vertex_),
          vertex_begin_(cp.vertex_begin_),
          vertex_end_(cp.vertex_end_)
    { };
    //@}

    /** Input Iterator operator overloads */
    function_graph_in_edge_iterator& operator++()
    {
        // Cycle through the range until an edge is found,
        // or the end of the list is found
        do {
            ++vertex_begin_;
        } while((vertex_begin_ != vertex_end_) &&
              !edge_(*vertex_begin_, *vertex_));

        return *this;
    }

    edge_descriptor operator*()
    {
        return edge_descriptor(edge_(*vertex_begin_, *vertex_),
                               *vertex_begin_,
                               *vertex_);
    }

    function_type edge_;
    vertex_iterator vertex_;
    vertex_iterator vertex_begin_;
    vertex_iterator vertex_end_;
};



/** @name Out-Edge Iterator
 * @description Iterates through the in edges of a vertex.
 */

template<typename Function, typename Edge, typename Range>
struct function_graph_out_edge_iterator {
private:
    typedef function_graph_out_edge_iterator<Function, Edge, Range> This;

public:
    typedef Range vertex_iterator_range;
    typedef typename range_iterator<vertex_iterator_range>::type
            vertex_iterator;
    typedef Edge edge_descriptor;
    typedef Function function_type;

    /** Iterator traits */
    typedef std::input_iterator_tag iterator_category;
    typedef edge_descriptor value_type;
    typedef std::size_t different_type;
    typedef value_type* pointer;
    typedef value_type& reference;

    /** @internal Constructors */
    //@{
    function_graph_out_edge_iterator(function_type const& f,
                                     vertex_iterator const& v,
                                     vertex_iterator const& v_begin,
                                     vertex_iterator const& v_end)
        : edge_(f),
          vertex_(v),
          vertex_begin_(v_begin),
          vertex_end_(v_end)
    { };

    // Copy constructor
    function_graph_out_edge_iterator(This const& cp)
        : edge_(cp.edge_),
          vertex_(cp.vertex_),
          vertex_begin_(cp.vertex_begin_),
          vertex_end_(cp.vertex_end_)
    { };
    //@}

    /** Input Iterator operator overloads */
    function_graph_out_edge_iterator& operator++()
    {
        // Cycle through the range until an edge is found,
        // or the end of the list is found
        do {
            ++vertex_begin_;
        } while((vertex_begin_ != vertex_end_) &&
              !edge_(*vertex_, *vertex_begin_));

        return *this;
    }

    edge_descriptor operator*()
    {
        return edge_descriptor(edge_(*vertex_, *vertex_begin_),
                               *vertex_begin_,
                               *vertex_);
    }

    function_type edge_;
    vertex_iterator vertex_;
    vertex_iterator vertex_begin_;
    vertex_iterator vertex_end_;
};


/** @name Function Graph
 * @description function_graph is a data structure that implements implicit
 * graphs and more.
 * @note This is a trap for poor template parameters
 */

template <typename Function, typename Range = infinite_domain_tag>
struct function_graph;



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
    : private function_graph_base_range<function<Result(Vertex, Vertex)>, Range>
{
private:
    typedef function_graph_base_range<
                function<Result(Vertex, Vertex)>,
                Range
            > Base;
    typedef function_graph<function<Result(Vertex, Vertex)>, Range > This;

public:
    typedef typename Base::function_type function_type;
    typedef typename Base::vertex_type vertex_descriptor;
    typedef typename Base::edge_type edge_descriptor;
    typedef typename Base::result_type result_type;
    typedef std::size_t degree_size_type;
    typedef std::size_t vertices_size_type;
    typedef directed_tag directed_category;
    typedef disallow_parallel_edge_tag edge_parallel_category;
    typedef adjacency_matrix_tag traversal_category;
    typedef Range vertex_iterator_range;
    typedef typename range_iterator<vertex_iterator_range>::type
                         vertex_iterator;
    typedef function_graph_in_edge_iterator<
                function_type,
                edge_descriptor,
                vertex_iterator_range
            > in_edge_iterator;
    typedef function_graph_out_edge_iterator<
                function_type,
                edge_descriptor,
                vertex_iterator_range
            > out_edge_iterator;
    typedef finite_domain_tag domain_category;

    /** Constructor: takes a functor and range */
    function_graph(function_type const& f, vertex_iterator_range const& r)
        : Base(f, r)
    { }

    using Base::edge_;
    using Base::range_;
};

/** @internal Allow a template function parameter without wrapping it with
 * boost::function.
 */
template <typename Result, typename Vertex, typename Range>
struct function_graph<Result(Vertex, Vertex), Range>
    : public function_graph<function<Result(Vertex, Vertex)>, Range>
{ };

// Specialization of function_graph without range
template <typename Result, typename Vertex>
struct function_graph<function<Result(Vertex, Vertex)>, infinite_domain_tag>
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
    typedef std::size_t degree_size_type;
    typedef std::size_t vertices_size_type;
    typedef directed_tag directed_category;
    typedef disallow_parallel_edge_tag edge_parallel_category;
    typedef adjacency_matrix_tag traversal_category;
    typedef infinite_domain_tag domain_category;

    /** Constructor: takes a boost::function or functor */
    function_graph(function_type const& f)
        : Base(f)
    { }
};


template <typename Result, typename Vertex>
struct function_graph<Result(Vertex, Vertex), infinite_domain_tag>
    : public function_graph<function<Result(Vertex, Vertex)> >
{ };




/** source(e, g) and target(e, g) are part of the incedence graph concept. */

template <typename Result, typename Vertex, typename Range>
Vertex source(detail::func_graph_edge<Result, Vertex> const& e,
              function_graph<function<Result(Vertex, Vertex)>, Range > const& g)
{ return e.source_; }

template <typename Result, typename Vertex, typename Range>
Vertex target(detail::func_graph_edge<Result, Vertex> const& e,
              function_graph<function<Result(Vertex, Vertex)>, Range > const& g)
{ return e.target_; }



/** in_edges(v, g) and out_edges(u, g)
 * @note This is a rough draft for testing purposes and readability. There will
 * be improvements later.
 */

#define FUNC_GRAPH \
    function_graph<function<Result(Vertex, Vertex)>, Range>

template <typename Result, typename Vertex, typename Range>
std::pair<
    typename FUNC_GRAPH::in_edge_iterator,
    typename FUNC_GRAPH::in_edge_iterator
>
in_edges(typename FUNC_GRAPH::vertex_descriptor const& v, FUNC_GRAPH const& g)
{
    typedef function_graph<function<Result(Vertex, Vertex)>, Range> Graph;
    typedef typename Graph::in_edge_iterator in_edge_iterator;
    typedef typename Graph::vertex_iterator vertex_iterator;
    typedef std::pair<in_edge_iterator, in_edge_iterator> iter_range;

    vertex_iterator first_vertex_pair = begin(g.range_);
    vertex_iterator vertex_end = end(g.range_);
    while((first_vertex_pair != vertex_end) || !g.edge_(first_vertex_pair, v))
    { ++first_vertex_pair; }
    in_edge_iterator in_edge_begin(first_vertex_pair, v);
    in_edge_iterator in_edge_end(vertex_end, v);

    return std::make_pair(in_edge_begin, in_edge_end);
}

template <typename Result, typename Vertex, typename Range>
std::pair<
    typename FUNC_GRAPH::out_edge_iterator,
    typename FUNC_GRAPH::out_edge_iterator
>
out_edges(typename FUNC_GRAPH::vertex_descriptor const& u, FUNC_GRAPH const& g)
{
    typedef FUNC_GRAPH Graph;
    typedef typename Graph::out_edge_iterator out_edge_iterator;
    typedef typename Graph::vertex_iterator vertex_iterator;
    typedef std::pair<out_edge_iterator, out_edge_iterator> iter_range;

    vertex_iterator first_vertex_pair = begin(g.range_);
    vertex_iterator vertex_end = end(g.range_);
    while((first_vertex_pair != vertex_end) || !g.edge_(first_vertex_pair, u))
    { ++first_vertex_pair; }
    out_edge_iterator out_edge_begin(first_vertex_pair, u);
    out_edge_iterator out_edge_end(vertex_end, u);

    return std::make_pair(out_edge_begin, out_edge_end);
}



/** Degree functions */

template<typename Result, typename Vertex, typename Range>
typename FUNC_GRAPH::degree_size_type
out_degree(typename FUNC_GRAPH::vertex_descriptor const& u, FUNC_GRAPH const& g)
{
    typedef FUNC_GRAPH Graph;
    typedef typename Graph::vertex_iterator vertex_iterator;
    typedef typename FUNC_GRAPH::degree_size_type degree_size_type;

    degree_size_type out_edges();
    vertex_iterator vertex_at(begin(g.range_));
    vertex_iterator vertex_end(end(g.range_));
    for(;vertex_at != vertex_end; ++vertex_at)
    {
        if(g.edge_(u, *vertex_at)) ++out_edges;
    }

    return out_edges;
}

template<typename Result, typename Vertex, typename Range>
typename FUNC_GRAPH::degree_size_type
in_degree(typename FUNC_GRAPH::vertex_descriptor const& v, FUNC_GRAPH const& g)
{
    typedef FUNC_GRAPH Graph;
    typedef typename Graph::vertex_iterator vertex_iterator;
    typedef typename FUNC_GRAPH::degree_size_type degree_size_type;

    degree_size_type in_edges();
    vertex_iterator vertex_at(begin(g.range_));
    vertex_iterator vertex_end(end(g.range_));
    for(;vertex_at != vertex_end; ++vertex_at)
    {
        if(g.edge_(*vertex_at, v)) ++in_edges;
    }

    return in_edges;
}

template<typename Result, typename Vertex, typename Range>
typename FUNC_GRAPH::degree_size_type
degree(typename FUNC_GRAPH::vertex_descriptor const& v, FUNC_GRAPH const& g)
{ return in_degree(v, g) + out_degree(v, g); }



/** vertices(g) is part of the vertex list concept. */

template <typename Result, typename Vertex, typename Range>
std::pair<
    typename FUNC_GRAPH::vertex_iterator,
    typename FUNC_GRAPH::vertex_iterator
>
vertices(function_graph<function<Result(Vertex, Vertex)>, Range> const& g)
{ return std::make_pair(begin(g.range_), end(g.range_)); }



/** num_vertices(g) is part of the vertex list concept.
 * @note Uses boost::size() from the iterator_range concept.
 */
template<typename Result, typename Vertex, typename Range>
typename FUNC_GRAPH::vertices_size_type
num_vertices(FUNC_GRAPH const& g)
{
    return size(g.range_);
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
bind_edge(Result const& r, Vertex u, Vertex v)
{
    return std::make_pair(detail::func_graph_edge<Result, Vertex>(r, u, v),
                          true);
}

template <typename Vertex>
std::pair<detail::func_graph_edge<bool, Vertex>, bool>
bind_edge(bool r, Vertex u, Vertex v)
{
    return std::make_pair(typename detail::func_graph_edge<
                              bool, Vertex
                          >(r, u, v),
                          r);
}

// This overload is specific to optional<T>
template <typename OptType, typename Vertex>
std::pair<detail::func_graph_edge<optional<OptType>, Vertex>, bool>
bind_edge(optional<OptType> const& r, Vertex u, Vertex v)
{
    return std::make_pair(detail::func_graph_edge<
                              optional<OptType>, Vertex
                          >(r, u, v),
                          (bool)r);
}

}   // detail namespace

template <typename Result, typename Vertex, typename Range>
std::pair<typename FUNC_GRAPH::edge_descriptor, bool>
edge(typename FUNC_GRAPH::vertex_descriptor u,
     typename FUNC_GRAPH::vertex_descriptor v,
     FUNC_GRAPH const& g)
{
    typedef FUNC_GRAPH graph_type;
    typedef typename FUNC_GRAPH::result_type result_type;
    result_type result = g.edge(u, v);
    return detail::bind_edge(result, u, v);
}

#undef FUNC_GRAPH
//@}

}   // boost namespace

#endif /*FUNCTION_GRAPH_HPP_*/

