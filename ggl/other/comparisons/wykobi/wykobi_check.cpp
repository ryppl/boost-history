#include <iostream>
#include <string>

#include "../common/common.hpp"
#include "../common/read_shapefile.hpp"

#include <boost/timer.hpp>

#include <wykobi.hpp>
#include <wykobi_algorithm.hpp>



template <typename T>
T convert(SHPObject* psShape)
{
    const double* x = psShape->padfX;
    const double* y = psShape->padfY;
    T polygon;
    for (int v = 0; v < psShape->nVertices; v++)
    {
        typedef typename T::PointType POINT;
        POINT p;
        p[0] = x[v];
        p[1] = y[v];
        polygon.push_back(p);
    }
    return polygon;
}



int main(int argc, char** argv)
{
    compare::version_info("wykobi");

    typedef wykobi::point2d<double> POINT;
    typedef wykobi::polygon<double, 2> POLY;
    typedef wykobi::rectangle<double> BOX;

    std::vector<POLY> polygons;
    std::vector<BOX> boxes;
    std::vector<BOX> clip_boxes;
    std::vector<int> ids;

    read_shapefile(compare::filename(argc, argv), compare::fieldname(), polygons, ids, convert<POLY>);

    // Create envelopes
    for (std::vector<POLY>::const_iterator it = polygons.begin();
        it != polygons.end();
        it++)
    {
        BOX b = wykobi::aabb(*it);
        boxes.push_back(b);
    }


    // Create the smaller boxes for clips lateron
    if (compare::MEASURE_CLIP)
    {

        int k = 0;
        for (std::vector<BOX>::const_iterator bit = boxes.begin();
            bit != boxes.end();
            ++bit, ++k)
        {
            BOX const& bx = *bit;
            POINT c;
            c.x = (bx[0].x + bx[1].x) / 2.0;
            c.y = (bx[0].y + bx[1].y) / 2.0;


            double a = compare::CLIP_FACTOR * 0.5 * (bx[1].x - bx[0].x);
            double b = compare::CLIP_FACTOR * 0.5 * (bx[1].y - bx[0].y);

            BOX box;

            double angle = 225.0 * wykobi::PIDiv180;
            box[0].x = c.x + a * sin(angle);
            box[0].y = c.y + b * cos(angle);

            angle = 45.0 * wykobi::PIDiv180 ;
            box[1].x = c.x + a * sin(angle);
            box[1].y = c.y + b * cos(angle);

            clip_boxes.push_back(box);
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
                it++)
            {
                area += wykobi::area(*it);
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
                POINT centroid = wykobi::centroid(*it);
                sum_x += centroid.x;
                sum_y += centroid.y;
            }
        }
        compare::report_centroid(t, polygons.size(), sum_x, sum_y);
    }

    if (compare::MEASURE_CLIP)
    {
        bool first = true;
        double area1 = 0.0, area2 = 0.0;

        std::cout << std::setprecision(16);

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
                    area1 += wykobi::area(*it);
                }


                POLY out;
                wykobi::algorithm::sutherland_hodgman_polygon_clipper<POINT>
                    clipper(*bit, *it, out);
                if (compare::CLIP_AREA)
                {
                    area2 += wykobi::area(out);
                }
            }
        }
        compare::report_clip(t, polygons.size(), area1, area2);
    }


    if (compare::MEASURE_CONVEX_HULL)
    {
        double area = 0.0;
        boost::timer t;
        for (std::vector<POLY>::const_iterator it = polygons.begin(); it != polygons.end(); it++)
        {

            POLY out;
            wykobi::algorithm::convex_hull_graham_scan<POINT> graham(it->begin(), it->end(), std::back_inserter(out));
            if (compare::HULL_AREA)
            {
                area += wykobi::area(out);
            }
        }
        compare::report_hull(t, polygons.size(), area);
    }

    if (compare::MEASURE_WITHIN)
    {
        int count = 0;
        boost::timer t;
        for (int e = 0; e < boxes.size(); e++)
        {
            BOX const& b = boxes[e];
            POINT p;
            p.x = (b[0].x + b[1].x) / 2.0;
            p.y = (b[0].y + b[1].y) / 2.0;

            //compare::debug_within(e, count);

            std::vector<BOX>::const_iterator bit = boxes.begin();
            for (std::vector<POLY>::const_iterator it = polygons.begin();
                it != polygons.end() && bit != boxes.end();
                it++, bit++)
            {
                if (wykobi::point_in_rectangle(p, *bit)
                    && wykobi::point_in_polygon(p, *it))
                    //&& wykobi::point_in_polygon_winding_number(p, *it))
                {
                    count++;
                }
            }
        }
        compare::report_within(t, polygons.size(), count, -1);
    }


    return 0;
}
