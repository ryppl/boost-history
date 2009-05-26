#ifndef FUNCTION_GRAPH_FUNCTOR_HPP_
#define FUNCTION_GRAPH_FUNCTOR_HPP_

#include <boost/function.hpp>

namespace boost { namespace graph {

/**
 * beta of function_graph using functor
 * 
 * function_graph is a data structure that creates an implicit graph based on a
 * function or functor that is either user defined or from the stl or boost.
 * 
 * @internal
 * as of current, this data structure acts over a container. The type of the
 * container is immaterial, so long as it uses T least an input iterator.
 * 
 * @todo
 * implement a pointer that will set itself to null if the object it points to
 *     is destroyed
 * have type deduced from container
 * implement domains and ranges (given a type and function, but no container)
 * implement an edge iterator
 * implement a function_graph that acts over two sets that may be different
 *     types
 */
 
 /**
  * function_graph - functor version
  */
template<typename container, template <typename> class functor>
class function_graph
{
    typedef typename container::value_type value_type;
    // functor type is deduced from the container
    // could use better method of deduction
    typedef functor<value_type> functor_type;
    
public:

    /** Constructor - blank */
    function_graph()
        : container_(0) { }
    
    /** Constructor - take the container */
    function_graph(container const& x)
        : container_(&x) { }
    
    /** edge functor - allows it to act as edge(...) */
public:
    functor_type edge;
    
private:
    const container* container_;
};

}   // graph namespace
}   // boost namespace

#endif /*FUNCTION_GRAPH_FUNCTOR_HPP_*/
