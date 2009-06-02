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

namespace boost {

/**
 * function_graph_base handles the edge function. A user can define the function
 * as a boost::function, but the user may only have access to this function
 * through the function graph via edge().
 */
template <typename Func>
struct function_graph_base {
    
    typedef Func function_type;
    typedef typename function_type::result_type edge_type;
    typedef typename function_type::first_argument_type vertex_type;
    
    /** Constructor - Default */
    function_graph_base()
    { }
    
    /** Constructors to allow for initialization of edge */
    function_graph_base(function_type const& f)
        : edge_(f)
    { }
    
    // Allow access to the function edge_ holds, not edge_ itself.
    edge_type edge(vertex_type v1, vertex_type v2)
    { return edge_(v1, v2); }
    
    // Set the edge_ function
    void set_function(function_type edge_function)
    { edge_ = edge_function; }
    
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
 * function during execution. However, since the code needed is minute, set_edge
 * is part of the interface. Paired with it is the default constructor.
 */
template <typename Edge, typename Vertex>
struct function_graph <function <Edge(Vertex, Vertex)> >
    : public function_graph_base <function <Edge(Vertex, Vertex)> >
{
    typedef function_graph_base <function <Edge(Vertex, Vertex)> > Base;
public:
    
    typedef typename Base::function_type function_type;
    typedef typename Base::vertex_type vertex_descriptor;
    typedef typename Base::edge_type edge_descriptor;
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
    
    /** Set the function of the implicit graph */    
    using Base::set_function;
    
private:
    /** Edge function from Base */
    using Base::edge;
};

/**
 * @note This specialization will match any function of the form E(V,V) and
 * generates the graph over an adapted boost function. Note that functions of
 * the form E(U,V) will match the empty class causing compiler errors.
 */

template <typename Edge, typename Vertex>
struct function_graph <Edge(Vertex, Vertex)>
    : private function_graph_base <function<Edge(Vertex, Vertex)> >
{ };

/**
 * Direct Edge Access - Part of the adjacency matrix concept
 * http://www.boost.org/doc/libs/1_39_0/libs/graph/doc/AdjacencyMatrix.html
 */
template <typename Edge, typename Vertex>
std::pair<typename function_graph<function<Edge(Vertex, Vertex)> >
          ::edge_descriptor,    // It would look cleaner to use Edge and Vertex
          bool>
edge(typename function_graph<function<Edge(Vertex, Vertex)> >
     ::vertex_descriptor u,
     typename function_graph<function<Edge(Vertex, Vertex)> >
     ::vertex_descriptor v,
     function_graph<function<Edge(Vertex, Vertex)> > const& g)
{
    // Issue: Edge does not necessarily yield a false. The edge function needs
    // to account for this and should return the pair.
    //bool exists = function_graph<function<Edge(Vertex, Vertex)> >::edge(u, v)
    //return std::make_pair(/*  */, exists);
}

}   // boost namespace

#endif /*FUNCTION_GRAPH_HPP_*/

