#include <boost/geometry/geometry.hpp>

#include <boost/geometry/extensions/index/rtree/rtree.hpp>

#include <iostream>

#include <boost/timer.hpp>
#include <boost/foreach.hpp>

int main()
{
    {
        typedef boost::geometry::model::point<float, 2, boost::geometry::cs::cartesian> P;
        typedef boost::geometry::model::box<P> B;

        boost::geometry::index::rtree<B>::rtree_leaf l;
        boost::geometry::index::rtree<B>::rtree_internal_node n;

        std::cout << "shared ptr size: " << sizeof(boost::shared_ptr<int>) << '\n';
        std::cout << "vector size: " << sizeof(std::vector<int>) << '\n';
        std::cout << "internal node size: " << sizeof(n) << '\n';
        std::cout << "leaf size: " << sizeof(l) << '\n';        
    }

    {
        typedef boost::geometry::model::point<float, 2, boost::geometry::cs::cartesian> P;
        typedef boost::geometry::model::box<P> B;

        // randomize boxes
        const size_t n = 10000;
        std::vector<B> v(n);
        for ( size_t i = 0 ; i < n ; ++i )
        {
            float x = ( rand() % 10000 ) / 1000.0f;
            float y = ( rand() % 10000 ) / 1000.0f;
            float w = ( rand() % 10000 ) / 100000.0f;
            float h = ( rand() % 10000 ) / 100000.0f;
            v[i] = B(P(x - w, y - h),P(x + w, y + h));
        }

        boost::geometry::index::rtree<B> t(5, 1);

        std::cout << "inserting time test...\n";

        boost::timer tim;

        BOOST_FOREACH(B &b, v)
        {
            t.insert(b);
        }

        std::cout << "time: " << tim.elapsed() << "s\n";
        std::cout << "removing time test...\n";

        tim.restart();

        BOOST_FOREACH(B &b, v)
        {
            t.remove(b);
        }

        std::cout << "time: " << tim.elapsed() << "s\n";
    }

    return 0;
}
