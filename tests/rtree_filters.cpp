#include <boost/geometry/geometry.hpp>
#include <boost/geometry/extensions/index/rtree/rtree.hpp>

#include <boost/geometry/extensions/index/rtree/filters.hpp>
#include <boost/geometry/extensions/index/filters/nearest_filter.hpp>

#include <iostream>

#include <boost/range/algorithm.hpp>
#include <boost/foreach.hpp>

template <typename R>
void print_range(R const& r)
{
    BOOST_FOREACH(typename boost::iterator_value<typename R::const_iterator>::type const& b, r)
    {
    	float min_x = b.min_corner().template get<0>();
    	float min_y = b.min_corner().template get<1>();
    	float max_x = b.max_corner().template get<0>();
    	float max_y = b.max_corner().template get<1>();
        std::cout << "(" << min_x << ", " << min_y << ")";
        std::cout << 'x';
        std::cout << "(" << max_x << ", " << max_y << ")";
        std::cout << '\n';
    }
    std::cout << std::endl;
}

int main()
{
    typedef boost::geometry::model::point<float, 2, boost::geometry::cs::cartesian> P;
    typedef boost::geometry::model::box<P> B;

    {
        boost::geometry::index::rtree<B> t(3, 0);
        t.insert(B(P(0, 0), P(1, 1)));
        t.insert(B(P(2, 2), P(3, 3)));
        t.insert(B(P(4, 4), P(5, 5)));
        t.insert(B(P(6, 6), P(7, 7)));
        t.print();

        namespace f = boost::geometry::index::filters;
        print_range(t | f::spatially_filtered(B(P(2.5f, 2.5f), P(4.5f, 4.5f))));
        print_range(t | f::nearest_filtered(P(3.5f, 3.5f), 1.0f));
    }

    std::cin.get();
    return 0;
}
