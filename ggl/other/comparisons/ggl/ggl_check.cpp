// Generic Geometry Library - COMPARISONS
//
// Copyright Barend Gehrels, 2008, 2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)



//#define GGL_DEBUG_INTERSECTION
// #define OUTPUT_POSTGIS
// #define OUTPUT_SQLSERVER
//#define OUTPUT_ORACLE do not use this, does not work yet / is not yet checked in
//#define OUTPUT_WKT



#include "../common/common.hpp"
#include "../common/read_shapefile.hpp"

#include <ggl/ggl.hpp>
#include <ggl/geometries/geometries.hpp>

#if defined(OUTPUT_ORACLE)
#  include <ggl/io/oracle/write_oracle.hpp>
#endif



template <typename T>
T convert(SHPObject* psShape)
{
    const double* x = psShape->padfX;
    const double* y = psShape->padfY;
    T polygon;
    for (int v = 0; v < psShape->nVertices; v++)
    {
        typename ggl::point_type<T>::type p;
        ggl::set<0>(p, x[v]);
        ggl::set<1>(p, y[v]);
        polygon.outer().push_back(p);
    }
    return polygon;
}


#if defined(OUTPUT_POSTGIS) || defined(OUTPUT_SQLSERVER) || defined(OUTPUT_ORACLE)

template <typename Polygon>
void output_for_sql(int k, Polygon const& polygon1, Polygon const& polygon2, bool& first, bool last)
{
#ifdef OVERLAY_UNION
    const std::string operation = "union";
#else
    const std::string operation = "intersection";
#endif



    if (first)
    {

#if defined(OUTPUT_ORACLE)
        std::cout << "create table t_comp(k integer, a mdsys.sdo_geometry, b mdsys.sdo_geometry);"
            << std::endl << std::endl
            << "declare g1 mdsys.sdo_geometry;" << std::endl
            << "declare g2 mdsys.sdo_geometry;" << std::endl
            << "begin" << std::endl;

#else
        std::cout << "drop table if exists t_comp;" << std::endl;
        std::cout << "create table t_comp(k integer, a geometry, b geometry);" << std::endl;
#endif

        first = false;
    }

    #if defined(OUTPUT_POSTGIS)
    std::cout << "insert into t_comp values(" << k << ", "
        << "GeometryFromText('" <<  ggl::wkt(polygon1)
        << "'),GeometryFromText('" << ggl::wkt(polygon2)
        << "'));"
        << std::endl;

    // The query is:
    // select sum(area(intersection(a,b))) from t_comp
    #endif

    #if defined(OUTPUT_SQLSERVER)
    std::cout << "insert into t_comp values(" << k
        << ", geometry::STGeomFromText('" << ggl::wkt(polygon1)
        << "', 0), geometry::STGeomFromText('" << ggl::wkt(polygon2)
        << "', 0));"
        << std::endl;


    // The query is:
    // select sum(a.STIntersection(b).STArea()) as sum_area from t_comp
    // where k <> 1911 (for 101 points)
    // 101: reported 00:00:10 (10 seconds)
    // 1001: reported 00:01:55, area: 161.239301428967 (GGL: 161.2393008999)
    #endif

    #if defined(OUTPUT_ORACLE)
    std::cout
        << " g1 := " << ggl::sdo_oracle(polygon1, 32662) << ";" << std::endl
        << " g2 := " << ggl::sdo_oracle(polygon2, 32662) << ";" << std::endl
        << " execute immediate 'insert into t_comp(k,a,b) values(" << k << ",:gm1,:gm2)' using g1,g2;" << std::endl;


    // The query is:
    // select sum(a.STIntersection(b).STArea()) as sum_area from t_comp
    // where k <> 1911 (for 101 points)
    // 101: reported 00:00:10 (10 seconds)
    // 1001: reported 00:01:55, area: 161.239301428967 (GGL: 161.2393008999)
    #endif

    if (last)
    {
        #if defined(OUTPUT_ORACLE)
        std::cout
            << "end;" << std::endl
            << "/" << std::endl;
        #endif
    }



}
#endif


int main(int argc, char** argv)
{
    compare::version_info("ggl");

    typedef ggl::point_xy<double> POINT;
    typedef ggl::polygon<POINT> POLY;
    typedef ggl::box<POINT> BOX;

    std::vector<POLY> polygons;
    std::vector<BOX> boxes;
    std::vector<BOX> clip_boxes; // for clips
    std::vector<POLY> ellipses; // for intersections/unions
    std::vector<int> ids;
    try
    {
        read_shapefile(compare::filename(argc, argv), compare::fieldname(), polygons, ids, convert<POLY>);
    }
    catch(std::exception const& e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }
    catch(std::string const* s)
    {
        std::cout << s << std::endl;
        return 1;
    }
    catch(...)
    {
        std::cout << "Unknown exception" << std::endl;
        return 1;
    }


    // Create envelopes
    for (std::vector<POLY>::const_iterator it = polygons.begin();
        it != polygons.end();
        ++it)
    {
        BOX b;
        ggl::envelope(*it, b);
        boxes.push_back(b);
    }

    // Create the ellipses for intersections lateron
    if (compare::MEASURE_OVERLAY || compare::MEASURE_CLIP)
    {

        int k = 0;
        for (std::vector<BOX>::const_iterator bit = boxes.begin();
            bit != boxes.end();
            ++bit, ++k)
        {
            double cx = 0.5 * (ggl::get<ggl::max_corner, 0>(*bit) + ggl::get<ggl::min_corner, 0>(*bit));
            double cy = 0.5 * (ggl::get<ggl::max_corner, 1>(*bit) + ggl::get<ggl::min_corner, 1>(*bit));

            double dx = ggl::get<ggl::max_corner, 0>(*bit) - ggl::get<ggl::min_corner, 0>(*bit);
            double dy = ggl::get<ggl::max_corner, 1>(*bit) - ggl::get<ggl::min_corner, 1>(*bit);

            if (compare::MEASURE_OVERLAY)
            {
                double a1 = compare::OVERLAY_ELLIPSE_FACTOR1 * 0.5 * dx;
                double b1 = compare::OVERLAY_ELLIPSE_FACTOR1 * 0.5 * dy;
                double a2 = compare::OVERLAY_ELLIPSE_FACTOR2 * 0.5 * dx;
                double b2 = compare::OVERLAY_ELLIPSE_FACTOR2 * 0.5 * dy;

                POLY ellipse;
                ellipse.outer().reserve(compare::OVERLAY_ELLIPSE_COUNT);
                double angle = 0.0; //45.0 * ggl::math::d2r; //0.0;
                for (int i = 0; i < compare::OVERLAY_ELLIPSE_COUNT - 1; i++, angle += compare::delta)
                {
                    if (i % 2 == 0)
                    {
                        ellipse.outer().push_back(POINT(cx + a1 * sin(angle), cy + b1 * cos(angle)));
                    }
                    else
                    {
                        ellipse.outer().push_back(POINT(cx + a2 * sin(angle), cy + b2 * cos(angle)));
                    }
                }
                ellipse.outer().push_back(ellipse.outer().front());
                ellipses.push_back(ellipse);
            }

            if (compare::MEASURE_CLIP)
            {
                double a = compare::CLIP_FACTOR * 0.5 * dx;
                double b = compare::CLIP_FACTOR * 0.5 * dy;

                BOX box;
                double angle = 225.0 * ggl::math::d2r;
                ggl::set<ggl::min_corner, 0>(box, cx + a * sin(angle));
                ggl::set<ggl::min_corner, 1>(box, cy + b * cos(angle));
                angle = 45.0 * ggl::math::d2r;
                ggl::set<ggl::max_corner, 0>(box, cx + a * sin(angle));
                ggl::set<ggl::max_corner, 1>(box, cy + b * cos(angle));

                clip_boxes.push_back(box);

                /*if (k < 5)
                {
                    std::cout << ggl::wkt(box) << " " << ggl::area(box) << std::endl;
                }*/

            }

        }
    }

    compare::wait();


    if (compare::MEASURE_AREA)
    {
        double area = 0;
        boost::timer t;
        for (int i = 0; i < compare::AREA_COUNT; i++)
        {
            for (std::vector<POLY>::const_iterator it = polygons.begin();
                it != polygons.end();
                ++it)
            {
                area += ggl::area(*it);
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
                POINT centroid;
#if defined(CENTROID_WITH_CATCH)
                try
#endif
                {
                    ggl::centroid(*it, centroid);
                    sum_x += centroid.x();
                    sum_y += centroid.y();
                }
#if defined(CENTROID_WITH_CATCH)
                catch(std::exception const& e)
                {
                }
                catch(...)
                {
                }
#endif
            }
        }
        compare::report_centroid(t, polygons.size(), sum_x, sum_y);
    }

    if (compare::MEASURE_CONVEX_HULL)
    {
        double area = 0.0;
        boost::timer t;
        for (std::vector<POLY>::const_iterator it = polygons.begin(); it != polygons.end(); ++it)
        {
            POLY::ring_type ring;
            //std::cout << ggl::wkt<POLY>(*it) << std::endl;
            ggl::convex_hull(*it, std::back_inserter(ring));
            if (compare::HULL_AREA)
            {
                area += fabs(ggl::area(ring));
                /*POLY p;
                p.outer() = ring;
                std::cout << ggl::wkt<POLY>(p) << " ";
                std::cout << ggl::area(it->outer()) << " -> " << ggl::area(ring)  << std::endl;
                */
            }

        }
        compare::report_hull(t, polygons.size(), area);
    }


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
                if (ggl::intersects(*it))
                {
                    //std::cout << ggl::wkt<POLY>(*it) << std::endl;
                    n++;
                }
            }
        }
        compare::report_intersects(t, polygons.size(), n);
    }


    if (compare::MEASURE_OVERLAY)
    {
        bool first = true;
        double area1 = 0.0, area2 = 0.0;

        boost::timer t;
        for (int i = 0; i < compare::OVERLAY_COUNT; i++)
        {
            int k = 0;
            std::vector<POLY>::const_iterator eit = ellipses.begin();
            for (std::vector<POLY>::const_iterator it = polygons.begin();
                it != polygons.end() && eit != ellipses.end();
                ++it, ++eit, ++k)
            {
                //if (k == 1590)
                {

#if defined(OUTPUT_WKT)
                    if (i == 0)// && k == 264)
                    {
                        std::cout
                            << ggl::wkt(*it) << std::endl
                            << ggl::wkt(*eit) << std::endl;
                    }
#endif

                    if (compare::OVERLAY_AREA)
                    {
                        area1 += ggl::area(*it);
                    }



                    POLY p;
                    std::vector<POLY> v;
                    ggl::intersection<POLY>(*eit, *it, std::back_inserter(v));

                    if (compare::OVERLAY_AREA) // && k != 1911)
                    {
                        double a = 0.0;
                        for (std::vector<POLY>::const_iterator pit = v.begin(); pit != v.end(); ++pit)
                        {
                            a += ggl::area(*pit);
                        }
                        area2 += a;
                        //std::cout << "insert into se_ggl values(" << k << ", " << a << ");" << std::endl;
                    }

#if defined(OUTPUT_WKT)
                    if (i == 0)// && k == 264)
                    {
                        for (std::vector<POLY>::const_iterator pit = v.begin(); pit != v.end(); ++pit)
                        {
                            std::cout << ggl::wkt(*pit) << std::endl;
                        }
                        //return 0;
                    }
#endif


#if defined(OUTPUT_POSTGIS) || defined(OUTPUT_SQLSERVER) || defined(OUTPUT_ORACLE)
                    if (i == 0)
                    {
                        output_for_sql(k, *it, *eit, first, it + 1 == polygons.end());
                    }
#endif
                }
            }

        }
        compare::report_overlay(t, polygons.size(), area1, area2);
    }

    if (compare::MEASURE_CLIP)
    {
        bool first = true;
        double area1 = 0.0, area2 = 0.0;

       boost::timer t;
        for (int i = 0; i < compare::CLIP_COUNT; i++)
        {
            int k = 0;
            std::vector<BOX>::const_iterator bit = clip_boxes.begin();
            for (std::vector<POLY>::const_iterator it = polygons.begin();
                it != polygons.end() && bit != clip_boxes.end();

                ++it, ++bit,  ++k)
            {
                if (compare::CLIP_AREA)
                {
                    area1 += ggl::area(*it);
                }

                POLY p;
                std::vector<POLY> v;
                ggl::intersection<POLY>(*bit, *it, std::back_inserter(v));

                if (compare::CLIP_AREA)
                {
                    double a = 0.0;
                    for (std::vector<POLY>::const_iterator pit = v.begin(); pit != v.end(); ++pit)
                    {
                        a += ggl::area(*pit);
                    }
                    area2 += a;
                }
            }
        }
        compare::report_clip(t, polygons.size(), area1, area2);
    }


    if (compare::MEASURE_SIMPLIFY)
    {
        int count1 = 0, count2 = 0;
        double length1 = 0.0, length2 = 0.0;
        boost::timer t;
        for (std::vector<POLY>::const_iterator it = polygons.begin(); it != polygons.end(); ++it)
        {
            POLY p;
            ggl::simplify(*it, p, compare::SIMPLIFY_DISTANCE);
            count1 += it->outer().size();
            count2 += p.outer().size();
            if (compare::SIMPLIFY_LENGTH)
            {
                length1 += ggl::perimeter(it->outer());
                length2 += ggl::perimeter(p.outer());
            }

        }
        compare::report_simplify(t, polygons.size(), length1, length2, count1, count2);
    }


    if (compare::MEASURE_WITHIN)
    {
        int count = 0;
        boost::timer t;
        for (int e = 0; e < boxes.size(); e++)
        {
            const BOX& b = boxes[e];
            POINT p((b.min_corner().x() + b.max_corner().x()) / 2.0,
                (b.min_corner().y() + b.max_corner().y()) / 2.0);

            //compare::debug_within(e, count);

            std::vector<BOX>::const_iterator bit = boxes.begin();
            int k = 0;
            for (std::vector<POLY>::const_iterator it = polygons.begin();
                it != polygons.end() && bit != boxes.end();
                ++it, ++bit, ++k)
            {
                /***
                if ((e == 85 && k == 85)
                    || e == 99 && k == 99
                    )
                {
                    std::cout << e << "," << k << std::endl;
                    std::cout << std::setprecision(16) << ggl::wkt(p) << std::endl;
                    std::cout << std::setprecision(16) << ggl::wkt(*it) << std::endl;
                }
                ***/
                if (ggl::within(p, *bit) && ggl::within(p, *it))
                {
                    //std::cout << e << " IN " << k << std::endl;
                    count++;
                }
            }
        }
        compare::report_within(t, polygons.size(), count, -1);
    }



    return 0;
}
