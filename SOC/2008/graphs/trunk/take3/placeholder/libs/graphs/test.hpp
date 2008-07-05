
#ifndef TEST_HPP
#define TEST_HPP

#include <list>
#include <set>

template <typename Graph>
void test_add_vertices()
{
    Graph g;
    std::list<typename Graph::vertex_descriptor> V;
    for(int i = 0; i < 5; ++i) {
        V.push_back(g.add_vertex(i));
    }
}

template <typename Graph>
void test_add_remove_vertices()
{
    Graph g;
    std::list<typename Graph::vertex_descriptor> V;
    for(int i = 0; i < 5; ++i) {
        V.push_back(g.add_vertex(i));
    }
    BOOST_ASSERT(g.num_vertices() == 5);
    while(!V.empty()) {
        g.remove_vertex(V.front());
        V.pop_front();
    }
    BOOST_ASSERT(g.num_vertices() == 0);
}

template <typename Graph>
void test_make_simple_triangle()
{
    Graph g;
    std::vector<typename Graph::vertex_descriptor> V;
    for(int i = 0; i < 3; ++i) {
        V.push_back(g.add_vertex(i));
    }
    g.add_edge(V[0], V[1]);
    g.add_edge(V[1], V[2]);
    g.add_edge(V[2], V[0]);
    BOOST_ASSERT(g.num_vertices() == 3);
    BOOST_ASSERT(g.num_edges() == 3);
}

#endif
