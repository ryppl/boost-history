
#include "../common/common.hpp"
#include "../common/read_shapefile.hpp"

#include <iostream>

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


typedef K::Point_2 POINT;
typedef CGAL::Polygon_2<K> POLY;
typedef CGAL::Bbox_2 BOX;

typedef CGAL::Polygon_with_holes_2<K> HOLEY_POLY; // necessary for intersections


template <typename T>
T convert(SHPObject* psShape)
{
    const double* x = psShape->padfX;
    const double* y = psShape->padfY;

    POLY poly;

    // for intersections:
    // 1: polygon must NOT be closed otherwise: "Cannot contruct a degenerate segment"
            /*CGAL-intersections-degenerate:

            terminate called after throwing an instance of 'CGAL::Precondition_exception'
              what():  CGAL ERROR: precondition violation!
            Expr: ! is_degen
            File: ../../../../../CGAL-3.4/include/CGAL/Arr_segment_traits_2.h
            Line: 120
            Explanation: Cannot contruct a degenerate segment.
            Aborted*/

    // 2: polygon must be counter-clockwise otherwise: "The polygon has the wrong orientation"
    // 3: still get an error, now:
            /*terminate called after throwing an instance of 'CGAL::Precondition_exception'
              what():  CGAL ERROR: precondition violation!
            Expr: comp_f(object, parentP->object) != SMALLER
            File: ../../../../../CGAL-3.4/include/CGAL/Multiset.h
            Line: 2128
            Aborted
            */


    // 4: when going to quotient/etc, we come the most far, getting for one polygon:
            /*
            CGAL warning: check violation!
            Expression : is_strictly_simple
            File       : ../../../../../CGAL-3.4/include/CGAL/Boolean_set_operations_2/Gps_polygon_validation.h
            Line       : 174
            Explanation: The polygon is not strictly simple.
            Refer to the bug-reporting instructions at http://www.cgal.org/bug_report.html
            terminate called after throwing an instance of 'CGAL::Precondition_exception'
              what():  CGAL ERROR: precondition violation!
            Expr: m_traits->is_valid_2_object()(pgn)
            File: ../../../../../CGAL-3.4/include/CGAL/General_polygon_set_2.h
            Line: 200
            */

    for (int v = psShape->nVertices - 1; v > 0; v--)
    {
        // CGAL's intersection does not like degenerate segments. So sort them out here.
        if (! (v > 0
            && std::abs(x[v - 1] - x[v]) < 1e-10
            && std::abs(y[v - 1] - y[v]) < 1e-10))
        {
            poly.push_back(POINT(x[v], y[v]));
        }
    }

    return poly;
}




int main(int argc, char** argv)
{
    compare::version_info("cgal");
    std::vector<POLY> polygons;
    std::vector<POLY> ellipses;
    std::vector<BOX> boxes;
    //std::vector<BOX> clip_boxes;
    std::vector<int> ids;



    read_shapefile(compare::filename(argc, argv), compare::fieldname(), polygons, ids, convert<POLY>);


    // Create envelopes
    for (std::vector<POLY>::const_iterator it = polygons.begin();
        it != polygons.end();
        ++it)
    {
        BOX box = CGAL::bbox_2(it->vertices_begin(), it->vertices_end(), K());
        boxes.push_back(box);
    }

    // Create the ellipses for intersections lateron
    if (compare::MEASURE_OVERLAY)
    {
        for (std::vector<BOX>::const_iterator bit = boxes.begin();
            bit != boxes.end();
            ++bit)
        {
            double cx = 0.5 * (bit->xmin() + bit->xmax());
            double cy = 0.5 * (bit->ymin() + bit->ymax());

            double dx = bit->xmax() - bit->xmin();
            double dy = bit->ymax() - bit->ymin();

            double a1 = compare::OVERLAY_ELLIPSE_FACTOR1 * 0.5 * dx;
            double b1 = compare::OVERLAY_ELLIPSE_FACTOR1 * 0.5 * dy;
            double a2 = compare::OVERLAY_ELLIPSE_FACTOR2 * 0.5 * dx;
            double b2 = compare::OVERLAY_ELLIPSE_FACTOR2 * 0.5 * dy;

            double angle = 0.0;

            POLY ellipse;

            // Create COUNTER-clockwise ellipse (therefore the minus in angle), WITHOUT last point
            // (therefore the -1)
            for (int v = 0; v < compare::OVERLAY_ELLIPSE_COUNT - 1; v++, angle -= compare::delta)
            {
                if (v % 2 == 0)
                {
                    ellipse.push_back(POINT(cx + a1 * sin(angle), cy + b1 * cos(angle)));
                }
                else
                {
                    ellipse.push_back(POINT(cx + a2 * sin(angle), cy + b2 * cos(angle)));
                }
            }

            ellipses.push_back(ellipse);
        }
    }


    /***
    if (compare::MEASURE_CLIP)
    {
        for (std::vector<BOX>::const_iterator bit = boxes.begin();
            bit != boxes.end();
            ++bit)
        {
            double cx = 0.5 * (bit->xmin() + bit->xmax());
            double cy = 0.5 * (bit->ymin() + bit->ymax());

            double a = compare::OVERLAY_ELLIPSE_FACTOR * 0.5 * (bit->xmax() - bit->xmin());
            double b = compare::OVERLAY_ELLIPSE_FACTOR * 0.5 * (bit->ymax() - bit->ymin());

            double angle = 45.0 * boost::math::constants::pi<double>() / 180.0;

                BOX box;
                double angle = 225.0 * ggl::math::d2r;
                ggl::set<ggl::min_corner, 0>(box, cx + a * sin(angle));
                ggl::set<ggl::min_corner, 1>(box, cy + b * cos(angle));
                angle = 45.0 * ggl::math::d2r;
                ggl::set<ggl::max_corner, 0>(box, cx + a * sin(angle));
                ggl::set<ggl::max_corner, 1>(box, cy + b * cos(angle));

                .push_back(box);


            POLY clip;

            // Create COUNTER-clockwise ellipse (therefore the minus in angle), WITHOUT last point
            // (therefore the -1)
            for (int v = 0; v < 4; v++, angle -= compare::delta)
            {
                clip.push_back(POINT(cx + a * sin(angle), cy + b * cos(angle)));
            }

            BOX box = CGAL::bbox_2(clip.vertices_begin(), clip.vertices_end(), K());
            clip_boxes.push_back(box);
        }
    }
    ***/


    compare::wait();

    if (compare::MEASURE_AREA)
    {
        double area = 0;
        boost::timer t;
        for (int i = 0; i < compare::AREA_COUNT; i++)
        {
            for (std::vector<POLY>::const_iterator it = polygons.begin();
                it != polygons.end();
                it++)
            {
                area += CGAL::to_double(it->area());
            }
        }
        compare::report_area(t, polygons.size(), area);
    }

    if (compare::MEASURE_CENTROID)
    {
        double sum_x = 0, sum_y = 0;
        boost::timer t;
        for (int i = 0; i < compare::CENTROID_COUNT; i++)
        {
            for (std::vector<POLY>::const_iterator it = polygons.begin();
                it != polygons.end();
                ++it)
            {
                POINT centroid = CGAL::centroid(it->vertices_begin(), it->vertices_end(), K());
                sum_x += CGAL::to_double(centroid.x());
                sum_y += CGAL::to_double(centroid.y());
            }
        }
        compare::report_centroid(t, polygons.size(), sum_x, sum_y);
    }


    if (compare::MEASURE_CONVEX_HULL)
    {
        double area = 0.0;
        boost::timer t;
        for (std::vector<POLY>::const_iterator it = polygons.begin(); it != polygons.end(); it++)
        {
            POLY hull;
            CGAL::ch_graham_andrew(it->vertices_begin(), it->vertices_end(), std::back_inserter(hull));
            if (compare::HULL_AREA)
            {
                area += CGAL::to_double(hull.area());
            }

        }
        compare::report_hull(t, polygons.size(), area);
    }


    /***
    if (compare::MEASURE_CLIP)
    {
        bool first = true;
        double area1 = 0, area2 = 0;

        boost::timer t;

        std::vector<POLY>::const_iterator cit = clip_boxes.begin();
        for (std::vector<POLY>::iterator it = polygons.begin();
            it != polygons.end() && cit != clip_boxes.end();
            ++it, ++cit)
        {
            area1 += CGAL::to_double(it->area());

            std::list<HOLEY_POLY> pv;
            try
            {
                CGAL::intersection(*it, *cit, std::back_inserter(pv));
                double a = 0;
                for (std::list<HOLEY_POLY>::const_iterator pit = pv.begin(); pit != pv.end(); ++pit)
                {
                    a += CGAL::to_double(pit->outer_boundary().area());
                }
                area2 += a;
                //std::cout << " " << a << std::endl;
            }
            catch(std::exception const& e)
            {
            }
            catch(...)
            {
            }

        }
        compare::report_clip(t, polygons.size(), area1, area2);
    }
    ***/

    if (compare::MEASURE_INTERSECTS)
    {
        int n = 0;

        boost::timer t;
        for (int i = 0; i < 1; i++)
        {
            for (std::vector<POLY>::const_iterator it = polygons.begin();
                it != polygons.end();
                ++it)
            {
                if (! CGAL::is_simple_2(it->vertices_begin(), it->vertices_end(), K()))
                {
                    n++;
                }
            }
        }
        compare::report_intersects(t, polygons.size(), n);
    }


    #ifndef SKIP_OVERLAY
    if (compare::MEASURE_OVERLAY)
    {
        double area1 = 0, area2 = 0;

        boost::timer t;

        for (int i = 0; i < compare::OVERLAY_COUNT; i++)
        {
            int k = 0;
            std::vector<POLY>::const_iterator eit = ellipses.begin();

            //std::vector<int>::const_iterator idit = ids.begin();

            for (std::vector<POLY>::iterator it = polygons.begin();
                it != polygons.end() && eit != ellipses.end();
                ++it, ++eit, ++k) //, ++idit)
            {
                area1 += CGAL::to_double(it->area());

                std::list<HOLEY_POLY> pv;
                try
                {
                    //std::cout << " " << *idit;
                    CGAL::intersection(*it, *eit, std::back_inserter(pv));
                    double a = 0;
                    for (std::list<HOLEY_POLY>::const_iterator pit = pv.begin(); pit != pv.end(); ++pit)
                    {
                        a += CGAL::to_double(pit->outer_boundary().area());
                    }
                    area2 += a;
                }
                catch(std::exception const& e)
                {
                }
                catch(...)
                {
                }

            }
        }
        compare::report_overlay(t, polygons.size(), area1, area2);
    }
    #endif


    if (compare::MEASURE_WITHIN)
    {
        int count = 0;
        int count_boundary = 0;
        boost::timer t;
        for (int e = 0; e < boxes.size(); e++)
        {
            const BOX& b = boxes[e];
            POINT p((b.xmin() + b.xmax()) / 2.0, (b.ymin() + b.ymax()) / 2.0);

            //compare::debug_within(e, count);

            std::vector<BOX>::const_iterator bit = boxes.begin();
            int k = 0;
            for (std::vector<POLY>::const_iterator it = polygons.begin();
                it != polygons.end() && bit != boxes.end();
                it++, bit++, k++)
            {
                if (p.x() > bit->xmin() && p.x() < bit->xmax() && p.y() > bit->ymin() && p.y() < bit->ymax())
                {
                    switch(CGAL::bounded_side_2(it->vertices_begin(), it->vertices_end(), p, K()))
                    {
                        case CGAL::ON_BOUNDED_SIDE :
                            count++;
                            //std::cout << e << " IN " << k << std::endl;
                            break;
                        case CGAL::ON_BOUNDARY:
                            count_boundary++;
                            break;
                        case CGAL::ON_UNBOUNDED_SIDE:
                            break;
                     }
                }
            }
        }
        compare::report_within(t, polygons.size(), count, count_boundary);
    }

    return 0;
}
