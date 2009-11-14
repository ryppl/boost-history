// Generic Geometry Library - Star Comb Overlay Comparisons
//
// Copyright Barend Gehrels, 2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "../common/starcomb.hpp"

#include <boost/polygon/polygon.hpp>


double integer_factor = 2000.0;

template <typename Points, typename Point>
void add(Points& points, double x, double y, int)
{
    points.push_back(Point(x * integer_factor, y * integer_factor));
}


int main(int argc, char** argv)
{
    try
    {
        bool do_union;
        int testcount, starcount, combcount;
        double factor1, factor2;
        int parsed = 1 + parse(argc, argv, testcount, starcount, combcount, factor1, factor2, do_union);

        // Optional extra parameter specifying conversion double->integer factor
        if (argc > parsed)
        {
            integer_factor = atof(argv[parsed]);
        }

        double squared_factor = integer_factor * integer_factor;

        typedef boost::polygon::polygon_set_data<int> polygon_type;

        polygon_type star, comb;
        {
            typedef boost::polygon::point_data<int> point_type;
            typedef boost::polygon::polygon_data<int> ring_type;

            // fill points
            typedef std::vector<point_type> points;

            std::vector<point_type> star_points, comb_points;

            make_star(star_points, add<points, point_type>,
                        starcount, factor1, factor2);
            make_comb(comb_points, add<points, point_type>,
                        combcount);

            // set to polygon
            ring_type star_ring, comb_ring;
            star_ring.set(star_points.begin(), star_points.end());
            comb_ring.set(comb_points.begin(), comb_points.end());

            // insert to polygon-set
            star.insert(star_ring);
            comb.insert(comb_ring);
        }


        double area = 0;
        boost::timer t;
        for (int i = 0; i < testcount; i++)
        {
            polygon_type overlay;
            if (do_union)
            {
                overlay = boost::polygon::polygon_set_view
                    <polygon_type, polygon_type, 0>(star, comb);
            }
            else
            {
                overlay = boost::polygon::polygon_set_view
                    <polygon_type, polygon_type, 1>(star, comb);
            }

            // Assemble them in a list, somehow necessary
            typedef std::list<boost::polygon::polygon_with_holes_data<int> > list;
            list polys;
            overlay.get(polys);

            // The check, somehow necessary to finish the intersection process
            double a = 0.0;
            for (list::const_iterator it = polys.begin(); it != polys.end(); ++it)
            {
                a += boost::polygon::area(*it);
            }
            area += a / squared_factor;
        }
        report("B.P.", area, t);
    }
    catch(std::exception const& e)
    {
        std::cout << "B.P.: " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "B.P. exception..." << std::endl;
    }

    return 0;
}
