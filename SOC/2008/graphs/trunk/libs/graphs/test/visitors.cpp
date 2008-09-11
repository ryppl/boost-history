
#include <iostream>

#include <boost/function.hpp>

#include <boost/graphs/adjacency_list/undirected_graph.hpp>
#include <boost/graphs/algorithms/search.hpp>

#include "typestr.hpp"

using namespace std;


struct v1 : default_search_visitor
{
    int n;
    v1() : n(0) { }
    v1(int n) : n(n) { }

    template <typename G, typename V>
    void start_vertex(G const&, V)
    { cout << "v1 - " << n << endl; }
};

struct v2 : default_search_visitor
{
    int n;
    v2() : n(0) { }
    v2(int n) : n(n) { }

    template <typename G, typename V>
    void start_vertex(G const&, V)
    { cout << "v2 - " << n << endl; }
};

struct v3 : default_search_visitor
{
    int n;
    v3() : n(0) { }
    v3(int n) : n(n) { }

    template <typename G, typename V>
    void start_vertex(G const&, V)
    { cout << "v3 - " << n << endl; }
};

struct my_start_visitor
{
    template <typename G, typename V>
    void operator()(G const&, V)
    { cout << "visiting function object" << endl; }
};

void test(int, int)
{ cout << "visiting free function" << endl; }


void another_test(int, int)
{ cout << "visiting wrapped function" << endl; }


int main()
{
    v1 a(1);
    v2 b(2);
    v3 c(3);

    search_visitor<v1, v2, v3> p;
    p.start_vertex(0, 0);
    cout << endl;

    search_visitor<v1, v2, v3> q(a, b, c);
    q.start_vertex(0, 0);
    cout << endl;

    search_visitor<start_vertex_visitor<void (*)(int, int)>, v1> r(test, a);
    r.start_vertex(0, 0);
    cout << endl;

    // Make a fairly complex search visitor.
    function<void (int, int)> func = another_test;
    make_search_visitor(
            on_start_vertex(test),
            on_start_vertex(my_start_visitor()),
            on_start_vertex(func),
            v2(1),
            v3(2)
        ).start_vertex(0, 0);
    cout << endl;

    return 0;
}
