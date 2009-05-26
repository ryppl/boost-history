#ifndef FUNCTION_GRAPH_FUNCTION_HPP_
#define FUNCTION_GRAPH_FUNCTION_HPP_

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <boost/function.hpp>

namespace boost { namespace graph {

/**
 * function_graph - function version
 */
template<typename container, typename edge_return>
class function_graph {
    typedef container container_type;
    typedef typename container_type::value_type value_type;
    typedef edge_return edge_return_type;
    typedef boost::function<edge_return_type (value_type, value_type)>
        function_type;
public:
    /** Constructor - blank */
    function_graph() 
        : edge_(0), container_(0) { }
    
    /** Constructor - take a function */
    function_graph(function_type const& f)
        : edge_(f), container_(0) { }
        
    /** Constructor - take a function and a container */
    function_graph(function_type const& f, container_type const& x)
        : edge_(f), container_(&x) { }
    
    /** edge - does the edge exist */
    edge_return_type edge(value_type const& a, value_type const& b)
    { return edge_(a, b); }
    
private:
    function_type edge_;
    const container_type* container_;
    
};

}   // end graph namespace
}   // end boost naespace

#endif /*FUNCTION_GRAPH_FUNCTION_HPP_*/
