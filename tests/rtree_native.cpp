#include <boost/geometry/geometry.hpp>

#include <boost/geometry/extensions/index/rtree/rtree.hpp>

#include <iostream>

#include <boost/range/algorithm.hpp>
#include <boost/foreach.hpp>

#include <map>

int main()
{
    // Box
    {
        typedef boost::geometry::model::point<float, 2, boost::geometry::cs::cartesian> P;
        typedef boost::geometry::model::box<P> B;

        boost::geometry::index::rtree<B> t(3, 0);
        t.insert(B(P(0, 0), P(1, 1)));
        t.insert(B(P(2, 2), P(3, 3)));
        t.insert(B(P(4, 4), P(5, 5)));
        t.insert(B(P(6, 6), P(7, 7)));
        t.print();

        // error
        t.remove_in(B(P(-1, -1), P(2, 2)));
        // ok
        t.remove_in(B(P(0, 0), P(1, 1)));
        t.print();

        t.remove(B(P(6, 6), P(7, 7)));
        t.print();
    }

    std::cout << "-------------------------------------------------\n";
    std::cout << "-------------------------------------------------\n";

    // Point
    {
        typedef boost::geometry::model::point<float, 2, boost::geometry::cs::cartesian> P;
        typedef boost::geometry::model::box<P> B;

        boost::geometry::index::rtree<P> t(3, 0);
        t.insert(P(0, 0));
        t.insert(P(2, 2));
        t.insert(P(4, 4));
        t.insert(P(6, 6));
        t.print();

        // error
        t.remove_in(B(P(-1, -1), P(1, 1)));
        // ok
        t.remove_in(B(P(0, 0), P(0, 0)));
        t.print();

        t.remove(P(6, 6));
        t.print();
    }

    std::cout << "-------------------------------------------------\n";
    std::cout << "-------------------------------------------------\n";

    // std::pair<Box, int>
    {
        typedef boost::geometry::model::point<float, 2, boost::geometry::cs::cartesian> P;
        typedef boost::geometry::model::box<P> B;
        typedef std::pair<B, int> V;

        boost::geometry::index::rtree<V> t(3, 0);
        t.insert(V(B(P(0, 0), P(1, 1)), 0));
        t.insert(V(B(P(2, 2), P(3, 3)), 1));
        t.insert(V(B(P(4, 4), P(5, 5)), 2));
        t.insert(V(B(P(6, 6), P(7, 7)), 3));
        t.print();

        // error
        t.remove_in(B(P(0, 0), P(2, 1)));
        // ok
        t.remove_in(B(P(0, 0), P(1, 1)));
        t.print();

        t.remove(V(B(P(6, 6), P(7, 7)), 3));
        t.print();
    }

    std::cout << "-------------------------------------------------\n";
    std::cout << "-------------------------------------------------\n";

    // boost::shared_ptr< std::pair<Box, int> >
    {
        typedef boost::geometry::model::point<float, 2, boost::geometry::cs::cartesian> P;
        typedef boost::geometry::model::box<P> B;
        
        typedef boost::shared_ptr< std::pair<B, int> > V;

        V v1( new std::pair<B, int>(B(P(0, 0), P(1, 1)), 0) );
        V v2( new std::pair<B, int>(B(P(2, 2), P(3, 3)), 1) );
        V v3( new std::pair<B, int>(B(P(4, 4), P(5, 5)), 2) );
        V v4( new std::pair<B, int>(B(P(6, 6), P(7, 7)), 3) );

        boost::geometry::index::rtree<V> t(3, 0);
        t.insert(v1);
        t.insert(v2);
        t.insert(v3);
        t.insert(v4);
        t.print();

        // error
        t.remove_in(B(P(0, 0), P(2, 1)));
        // ok
        t.remove_in(B(P(0, 0), P(1, 1)));
        t.print();

        t.remove(v4);
        t.print();
    }

    std::cout << "-------------------------------------------------\n";
    std::cout << "-------------------------------------------------\n";

    // std::map<int, Box>::iterator
    {
        typedef boost::geometry::model::point<float, 2, boost::geometry::cs::cartesian> P;
        typedef boost::geometry::model::box<P> B;
        
        typedef std::map<int, B>::iterator V;

        std::map<int, B> m;
        m.insert(std::pair<int, B>(0, B(P(0, 0), P(1, 1))));
        m.insert(std::pair<int, B>(1, B(P(2, 2), P(3, 3))));
        m.insert(std::pair<int, B>(2, B(P(4, 4), P(5, 5))));
        m.insert(std::pair<int, B>(3, B(P(6, 6), P(7, 7))));

        boost::geometry::index::rtree<V> t(3, 0);
        V vit = m.begin();
        t.insert(vit++);
        t.insert(vit++);
        t.insert(vit++);
        t.insert(vit);
        t.print();

        // error
        t.remove_in(B(P(0, 0), P(2, 1)));
        // ok
        t.remove_in(B(P(0, 0), P(1, 1)));
        t.print();

        t.remove(m.find(3));
        t.print();
    }

    // ERROR!
    // remove_in expects previously inserted box - it should remove all objects inside some bigger box

    std::cin.get();
    return 0;
}
