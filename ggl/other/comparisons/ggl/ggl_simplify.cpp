
#include "../common/common.hpp"
#include "../common/read_shapefile.hpp"

#include <boost/timer.hpp>
#include <ggl/ggl.hpp>
#include <ggl/algorithms/perimeter.hpp>
#include <ggl/geometries/geometries.hpp>

template <typename T>
T convert(SHPObject* psShape)
{
    const double* x = psShape->padfX;
    const double* y = psShape->padfY;
    T polygon;
    for (int v = 0; v < psShape->nVertices; v++)
    {
        typename T::point_type POINT;
        //POINT p;
        ggl::point_xy<double> p;
        ggl::set<0>(p, x[v]);
        ggl::set<1>(p, y[v]);
        polygon.outer().push_back(p);
    }
    return polygon;
}





int main(int argc, char** argv)
{
    compare::version_info("ggl");

    typedef ggl::point_xy<double> POINT;
    typedef ggl::polygon<POINT> POLY;
    typedef ggl::box<POINT> BOX;

    std::vector<POLY> polygons;
    std::vector<BOX> boxes;
    std::vector<BOX> boxes10; // for intersections
    std::vector<int> ids;
    read_shapefile(compare::filename(argc, argv), compare::fieldname(), polygons, ids, convert<POLY>);

    double d = 0.01;
    bool iterate = true;
    while(iterate)
    {
        int count1 = 0, count2 = 0;
        double length1 = 0.0, length2 = 0.0;
        boost::timer t;
        for (std::vector<POLY>::const_iterator it = polygons.begin(); it != polygons.end(); it++)
        {
            POLY p;
            ggl::simplify(*it, p, d);
            count1 += it->outer().size();
            count2 += p.outer().size();
            length1 += ggl::perimeter(it->outer());
            length2 += ggl::perimeter(p.outer());
        }
        length1 /= 1000.0;
        length2 /= 1000.0;
        double diff = std::abs(length1 - length2);
        double frac = length2 / length1;

        std::cout << "SIMPLIFY " << polygons.size() << std::endl
            << " distance: " << d << std::endl
            << " frac: " << frac << std::endl
            << " vertices: " << count1 << " -> " << count2 << std::endl
            << " perimeter: " << length1 << " -> " << length2 << std::endl
            << " time: "<< t.elapsed() << "s" << std::endl;

        if (frac < 0.99 && d > 0.0001)
        {
            d *= 0.95;
        }
        else
        {
            iterate = false;
        }

    }



    return 0;
}
