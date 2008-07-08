
#include <iostream>
#include <string>
#include <list>

#include <boost/next_prior.hpp>
#include <boost/graphs/placeholder.hpp>

using namespace std;
using namespace boost;

struct Vertex
{
    Vertex(string n, int w)
        : name(n), weight(w)
    { }

    string name;
    int weight;
};

inline ostream& operator<<(ostream& os, Vertex const& v)
{ return os << v.name << " " << v.weight; }

int main()
{
    typedef list<Vertex> List;
    typedef List::iterator Iterator;

    typedef list<int>::iterator Dummy;
    typedef placeholder<sizeof(Dummy)> Reference;

    List verts;
    verts.push_back(Vertex("a", 1));
    verts.push_back(Vertex("b", 2));

    Reference x(verts.begin());
    Reference y(prior(verts.end()));

    cout << *x.get<Iterator>() << endl;
    cout << *y.get<Iterator>() << endl;

    return 0;
}
