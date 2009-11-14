// Generic Geometry Library - Star Comb Overlay Comparisons
//
// Copyright Barend Gehrels, 2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include "../common/starcomb.hpp"

#include <ggl/ggl.hpp>
#include <ggl/algorithms/union.hpp>
#include <ggl/geometries/geometries.hpp>
#include <ggl/extensions/gis/io/wkt/wkt.hpp>



template <typename Polygon>
void add(Polygon& polygon, double x, double y, int)
{
    typedef typename ggl::point_type<Polygon>::type p;
    ggl::exterior_ring(polygon).push_back(ggl::make<p>(x, y));
}



int main(int argc, char** argv)
{
    try
    {
        bool do_union;
        int testcount, starcount, combcount;
        double factor1, factor2;
        parse(argc, argv, testcount, starcount, combcount, factor1, factor2, do_union);


        typedef ggl::point_xy<double> point_type;
        typedef ggl::polygon<point_type> polygon_type;

        polygon_type star, comb;
        make_star(star, add<polygon_type>, starcount, factor1, factor2);
        make_comb(comb, add<polygon_type>, combcount);

    #ifdef _DEBUG
        std::cout << ggl::wkt(star) << std::endl;
        std::cout << ggl::wkt(comb) << std::endl;
    #endif


        double area = 0;
        boost::timer t;
        for (int i = 0; i < testcount; i++)
        {
            std::vector<polygon_type> v;
            if (do_union)
            {
                ggl::union_inserter<polygon_type>(star, comb, std::back_inserter(v));
            }
            else
            {
                ggl::intersection_inserter<polygon_type>(star, comb, std::back_inserter(v));
            }

            double a = 0.0;
            for (std::vector<polygon_type>::const_iterator pit = v.begin(); pit != v.end(); ++pit)
            {
                a += ggl::area(*pit);
    #ifdef _DEBUG
                std::cout << ggl::wkt(*pit) << std::endl;
    #endif
            }
            area += a;
        }

        report("GGL", area, t);
    }
    catch(std::exception const& e)
    {
        std::cout << "GGL: " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "GGL exception..." << std::endl;
    }

    return 0;
}
