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

namespace boost { namespace graph {

/**
 * function_graph_base handles the edge function. A user can define the function
 * as a boost::function, but the user may only have access to this function
 * through the function graph via edge().
 * 
 * @note This version compiles. To revert to the single parameter catch,
 * uncomment lines 55, 66 and 107 - 110. This is not to take the interface
 * towards a different direction. This is only to test the function graph
 * constructors and a set_edge_function for now.
 */
template <typename Func>
struct function_graph_base {
    
    typedef Func function_type;
    typedef typename function_type::result_type edge_type;
    typedef typename function_type::first_argument_type vertex_type;
    //typedef typename function_type::second_argument_type second_vertex_type;
    
    // Allow access to the function edge_ holds, not edge_ itself.
    edge_type edge(vertex_type v1, vertex_type v2)
    { return edge_(v1, v2); }
    
    // Set the edge_ function
    void set_edge_function(function_type edge_function)
    { edge_ = edge_function; }
    function_type edge_;
};

/**
 * Empty function graph prevents instantiations such as function_graph<int> and
 * function_graph<bool (int, int)>.
 * 
 * @note [asutton] I'm defining an empty function graph so that we can create
 * specializations that force the user to provide T in certain forms. This
 * variant will be instantiated for meaningless types such as graph<int>.
 * Obviously, that should fail.
 */
//template <typename T> struct function_graph { };

/**
 * function_graph is a class designed to handle implicit graphs and more.
 * Better documentation and explanation of the data structure to come.
 * 
 * @internal The typical user of function graph may not need to change edge 
 * function during execution. However, since the code needed is minute, set_edge
 * is part of the interface. Paired with it is the default constructor.
 */
template <typename Edge, typename Vertex>
struct function_graph //<function<Edge(Vertex, Vertex)> >
    : private function_graph_base <function <Edge(Vertex, Vertex)> >
{
    typedef function_graph_base <function<Edge(Vertex, Vertex)> > Base;
    
public:

    typedef typename Base::function_type function_type;
    typedef typename Base::edge_type edge_type;
    typedef typename Base::vertex_type vertex_type;
    //typedef Base::second_vertex_type second_vertex_type;

    /** 
     * Constructor: default 
     * @note Exists only when set_edge is used
     */
    function_graph()
    { }
    
    /** Constructor: takes a boost::function or functor */
    function_graph(function_type const& f)
        //: edge_(f)
    { edge_ = f;}
    
    
    /** Set the function of the implicit graph */    
    using Base::set_edge_function;
    
    /** Edge function from Base */
    using Base::edge;

private:
    Base::edge_;

private:
};

/**
 * @note This specialization will match any function of the form E(V,V) and
 * generates the graph over an adapted boost function. Note that functions of
 * the form E(U,V) will match the empty class causing compiler errors.
 */

/*template <typename Edge, typename Vertex>
struct function_graph<Edge(Vertex, Vertex)>
    : private function_graph_base< function<Edge(Vertex, Vertex)> >
{ };*/

}   // graph namespace
}   // boost namespace

#endif /*FUNCTION_GRAPH_HPP_*/
