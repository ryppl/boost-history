
#include <iostream>

#include <boost/graphs/undirected_graph.hpp>

using namespace std;
using namespace boost;

/**
 * Dumb type that will cause common functions like std::less and std::greater
 * to call the custom operators.
 */
struct Vertex
{
    Vertex(int x)
        : value(x)
    { }

    int value;

    inline bool operator<(Vertex const& x) const
    {
        cout << "    op< " << value << " " << x.value << endl;
        return value < x.value;
    }

    inline bool operator>(Vertex const& x) const
    {
        cout << "    op> " << value << " " << x.value << endl;
        return value > x.value;
    }
};

template <typename Props>
struct custom
{
    bool operator()(Props const& a, Props const& b) const
    {
        cout << "    custom " << a.value << " " << b.value << endl;
        return a.value < b.value;
    }
};

template <typename>
struct non_template
{
    bool operator()(Vertex const& a, Vertex const& b) const
    {
        cout << "    custom " << a.value << " " << b.value << endl;
        return a.value < b.value;
    }
};

template <template <typename> class Compare>
void test()
{
    typedef undirected_graph<Vertex, int, vertex_set<Compare>, edge_set<> > Graph;
    Graph g;
    cout << "inserting 1" << endl;
    g.add_vertex(1);
    cout << "inserting 2" << endl;
    g.add_vertex(2);
}

void comps()
{
    cout << "LESS" << endl;
    test<less>();

    cout << "GREATER" << endl;
    test<greater>();

    cout << "CUSTOM" << endl;
    test<custom>();

    cout << "NON-TEMPLATE" << endl;
    test<non_template>();
}

int main()
{
    typedef undirected_graph<string, none, vertex_set<>, edge_set<> > Graph;
    Graph g;
    g.add_vertex("Jan");
    cout << g[g.find_vertex("Jan")] << endl;

    typedef undirected_graph<none, int, vertex_list, edge_list> Tmp;
    Tmp f;
    f.remove_vertex(0);
}

