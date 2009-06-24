/**
 * Testing a function that returns a boolean
 *
 * Functions used in test:
 *   source(e, g)
 *   target(e, g)
 *   edge(u, v, g)
 *     detail::bind_edge(r, u, v) - general type
 *     detail::bind_edge(r, u, v) - boolean
 *     detail::bind_edge(r, u, v) - optional<T>
 */

#include <iostream>
#include <boost/function.hpp>
#include <boost/optional.hpp>
#include <functional>
#include <algorithm>
#include <vector>
#include <utility>
#include "function_graph.hpp"
#include <cassert>
#include <cmath>

////////
// Boolean function
template <typename T>
struct less_than {
    bool operator() (T a, T b) { return a < b; }
};

////////
// Normal function
template <typename T>
struct sum {
    int operator() (T a, T b) { return a + b; }
};

////////
// optional<T> function - returns difference if a and b differ by less than 5
struct difference_within_five {
    boost::optional<int> operator() (int a, int b) {
        int diff_check = a - b;
        
        boost::optional<int> difference;
        if(diff_check < 5) difference = diff_check;
        //std::cerr << a << ":" << b << ":" << diff_check << ":" << difference << "\n";
        return difference;
    }
};


int main()
{
    ////////
    // Create typedefs for functions and function graphs
    typedef boost::function<bool(int,int)> function_boolean;
    typedef boost::function_graph<function_boolean> graph_boolean;

    typedef boost::function<int(int,int)> function_normal;
    typedef boost::function_graph<function_normal> graph_normal;

    typedef boost::function<boost::optional<int>(int,int)> function_optional;
    typedef boost::function_graph<function_optional> graph_optional;


    ////////
    // Create functions, graphs and edges
    function_boolean f = less_than<int>();
    graph_boolean funcGraph_boolean(f);
    graph_boolean::vertex_descriptor x = 1;
    graph_boolean::vertex_descriptor y = 2;

    function_normal g = sum<int>();
    graph_normal funcGraph_normal(g);
    graph_normal::vertex_descriptor u = 1;
    graph_normal::vertex_descriptor v = 2;

    function_optional h = difference_within_five();
    graph_optional funcGraph_optional(h);
    graph_optional::vertex_descriptor r = 1;
    graph_optional::vertex_descriptor s = 2;


    ////////
    // Assert all edges
    graph_boolean::edge_descriptor e1 = edge(x, y, funcGraph_boolean).first;
    assert(source(e1, funcGraph_boolean) == x);
    assert(target(e1, funcGraph_boolean) == y);
    assert(e1.result_ == (1 < 2));

    graph_normal::edge_descriptor e2 = edge(u, v, funcGraph_normal).first;
    assert(source(e2, funcGraph_normal) == u);
    assert(target(e2, funcGraph_normal) == v);
    assert(e2.result_ == (u + v));

    graph_optional::edge_descriptor e3 = edge(r, s, funcGraph_optional).first;
    assert(source(e3, funcGraph_optional) == r);
    assert(target(e3, funcGraph_optional) == s);
    assert(e2.result_ == -1);

    return 0;
}
