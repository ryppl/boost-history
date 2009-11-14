// Generic Geometry Library - Star Comb Overlay Comparisons
//
// Copyright Barend Gehrels, 2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include "../common/starcomb.hpp"

#include <CGAL/basic.h>


#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Cartesian.h>

#include <CGAL/Polygon_2.h>

#include <CGAL/ch_graham_andrew.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/intersections.h>
#include <CGAL/centroid.h>


#ifdef CGAL_USE_GMP
   #include <CGAL/Gmpq.h>
#endif

#include <CGAL/MP_Float.h>
#include <CGAL/Quotient.h>



// Define the kernel.
// NOTE: this is also very important for perfomance
// Besides that, intersections might crash in some situations for some kernels...

//typedef CGAL::Cartesian<float> K; // -> nearly all polygons are invalid for intersection
typedef CGAL::Exact_predicates_inexact_constructions_kernel K; // fastest, + only K that compiles for MinGW
//typedef CGAL::Cartesian<double> K; // slower
//typedef CGAL::Exact_predicates_exact_constructions_kernel K;
//typedef CGAL::Cartesian<CGAL::Gmpq> K;
//typedef CGAL::Cartesian<CGAL::Quotient<CGAL::MP_Float> > K; // slowest



template <typename Polygon, typename Point>
void add(Polygon& polygon, double x, double y, int)
{
    polygon.push_back(Point(x, y));
}


int main(int argc, char** argv)
{
    try
    {
        bool do_union;
        int testcount, starcount, combcount;
        double factor1, factor2;
        parse(argc, argv, testcount, starcount, combcount, factor1, factor2, do_union);

        typedef K::Point_2 point_type;
        typedef CGAL::Polygon_2<K> polygon_type;
        typedef CGAL::Polygon_with_holes_2<K> holey_polygon_type;

        polygon_type star, comb;
        make_star(star, add<polygon_type, point_type>,
                starcount, factor1, factor2, 0.0, false, -1.0);
        make_comb(comb, add<polygon_type, point_type>,
                combcount, false, false);

        double area = 0;
        boost::timer t;
        for (int i = 0; i < testcount; i++)
        {
            if (do_union)
            {
                holey_polygon_type result;
                CGAL::join(star, comb, result);
                area += CGAL::to_double(result.outer_boundary().area());

                for (holey_polygon_type::Hole_const_iterator hit
                    = result.holes_begin();
                    hit != result.holes_end();
                    ++hit)
                {
                    area += CGAL::to_double(hit->area());
                }
            }
            else
            {
                std::list<holey_polygon_type> result;
                CGAL::intersection(star, comb, std::back_inserter(result));

                for (std::list<holey_polygon_type>::const_iterator it
                    = result.begin();
                    it != result.end();
                    ++it)
                {
                    area += CGAL::to_double(it->outer_boundary().area());
                }
            }
        }
        report("CGAL", area, t);
    }
    catch(std::exception const& e)
    {
        std::cout << "CGAL: " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "CGAL exception..." << std::endl;
    }

    return 0;
}
