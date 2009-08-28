#include <TeVersion.h>
#include <TeDatabase.h>
#include <TeGeometryAlgorithms.h>
#include <TeOverlay.h>

#include "TeException.h"

#include "../common/common.hpp"
#include "../common/read_shapefile.hpp"


template <typename T>
T convert(SHPObject* psShape)
{
    const double* x = psShape->padfX;
    const double* y = psShape->padfY;

    TeLinearRing ring;
    for (int v = 0; v < psShape->nVertices; v++)
    {
        TeCoord2D pt (x[v], y[v]);
        ring.add (pt);
    }
    TePolygon p;
    p.add(ring);
    return p;
}


template <typename T>
T convert_line(SHPObject* psShape)
{
    const double* x = psShape->padfX;
    const double* y = psShape->padfY;

    TeLine2D line;
    for (int v = 0; v < psShape->nVertices; v++)
    {
        TeCoord2D pt (x[v], y[v]);
        line.add (pt);
    }
    return line;
}


int main(int argc, char** argv)
{
    compare::version_info("terralib");

    std::vector<TeLine2D> lines;
    std::vector<TePolygon> polygons;
    std::vector<TeBox> boxes;
    std::vector<int> ids;

    std::vector<TePolygonSet> ellipses;
    std::vector<TePolygonSet> polygonsets;

    read_shapefile(compare::filename(argc, argv), compare::fieldname(), polygons, ids, convert<TePolygon>);


    // Read also polygons as (for simplify) (note that thse could be extracted from polygon rings
    // However, performance is not measured so we take it easy here.
    read_shapefile(compare::filename(argc, argv), compare::fieldname(), lines, ids, convert_line<TeLine2D>);

    // Create envelops
    for (std::vector<TePolygon>::const_iterator it = polygons.begin();
            it != polygons.end(); it++)
    {
        TeBox box = it->box();
        boxes.push_back(box);
    }

    // Create polygon sets
    for (std::vector<TePolygon>::const_iterator it = polygons.begin();
            it != polygons.end(); it++)
    {
        TePolygonSet set;
        set.add(*it);
        polygonsets.push_back(set);
    }

    // Create the ellipses for intersections lateron
    if (compare::MEASURE_OVERLAY)
    {
        for (std::vector<TeBox>::const_iterator bit = boxes.begin();
                bit != boxes.end();
                ++bit)
        {
            double factor = 0.9;
            double cx = 0.5 * (bit->x1() + bit->x2());
            double cy = 0.5 * (bit->y1() + bit->y2());

            double dx = bit->x2() - bit->x1();
            double dy = bit->y2() - bit->y1();

            double a1 = compare::OVERLAY_ELLIPSE_FACTOR1 * 0.5 * dx;
            double b1 = compare::OVERLAY_ELLIPSE_FACTOR1 * 0.5 * dy;
            double a2 = compare::OVERLAY_ELLIPSE_FACTOR2 * 0.5 * dx;
            double b2 = compare::OVERLAY_ELLIPSE_FACTOR2 * 0.5 * dy;

            double angle = 0.0;

            TeLinearRing ring;

            for (int v = 0; v < compare::OVERLAY_ELLIPSE_COUNT - 1; v++, angle += compare::delta)
            {
                if (v % 2 == 0)
                {
                    ring.add(TeCoord2D(cx + a1 * sin(angle), cy + b1 * cos(angle)));
                }
                else
                {
                    ring.add(TeCoord2D(cx + a2 * sin(angle), cy + b2 * cos(angle)));
                }
            }
            ring.add(ring.first());

            TePolygon poly;
            TePolygonSet ellipse;
            poly.add(ring);
            ellipse.add(poly);
            ellipses.push_back(ellipse);
        }
    }

    compare::wait();


    if (compare::MEASURE_AREA)
    {
        double area = 0;
        boost::timer t;
        for (int i = 0; i < compare::AREA_COUNT; i++)
        {
            for (std::vector<TePolygon>::const_iterator it = polygons.begin();
                    it != polygons.end(); ++it)
            {
                area += TeGeometryArea(*it);
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
            for (std::vector<TePolygon>::const_iterator it = polygons.begin();
                it != polygons.end();
                ++it)
            {
                TeCoord2D centroid = TeFindCentroid(*it);
                sum_x += centroid.x();
                sum_y += centroid.y();
            }
        }
        compare::report_centroid(t, polygons.size(), sum_x, sum_y);
    }


    if (compare::MEASURE_CONVEX_HULL)
    {
        double area = 0;
        boost::timer t;
        for (std::vector<TePolygon>::const_iterator it = polygons.begin();
                it != polygons.end(); ++it)
        {
            TePolygon hull = TeConvexHull(*it);
            if (compare::HULL_AREA)
            {
                area += TeGeometryArea(hull);
            }
        }
        compare::report_hull(t, polygons.size(), area);
    }


    if (compare::MEASURE_OVERLAY)
    {
        bool first = true;
        double area1 = 0.0, area2 = 0.0;

        std::cout << std::setprecision(16);

        boost::timer t;
        for (int i = 0; i < compare::OVERLAY_COUNT; i++)
        {
            int k = 0;
            std::vector<TePolygonSet>::const_iterator eit = ellipses.begin();
            for (std::vector<TePolygonSet>::const_iterator it = polygonsets.begin();
                it != polygonsets.end() && eit != ellipses.end();
                ++it, ++eit, ++k)
            {
                if (compare::OVERLAY_AREA)
                {
                    area1 += TeGeometryArea(*it);
                }

                TePolygonSet v;
                TeOVERLAY::TeIntersection(*it, *eit, v);
                if (compare::OVERLAY_AREA)
                {
                    double a = 0.0;
                    for (int i = 0; i < v.size(); i++)
                    {
                        a += TeGeometryArea(v[i]);
                    }
                    area2 += a;
                }
            }
        }
        compare::report_overlay(t, polygons.size(), area1, area2);
    }


    if (compare::MEASURE_SIMPLIFY)
    {
        // Modifies the line! But OK, rest is done on polygon
        int count1 = 0, count2 = 0;
        double length1 = 0.0, length2 = 0.0;
        boost::timer t;
        for (std::vector<TeLine2D>::iterator it = lines.begin(); it != lines.end(); ++it)
        {
            count1 += it->size();
            if (compare::SIMPLIFY_LENGTH)
            {
                length1 += TeLength(*it);
            }

            TeLineSimplify(*it, compare::SIMPLIFY_DISTANCE, compare::SIMPLIFY_DISTANCE * 100);

            count2 += it->size();
            if (compare::SIMPLIFY_LENGTH)
            {
                length2 += TeLength(*it);
            }
        }
        compare::report_simplify(t, polygons.size(), length1, length2, count1, count2);
    }


    if (compare::MEASURE_WITHIN)
    {
        int count = 0;
        boost::timer t;
        for (unsigned int e = 0; e < boxes.size(); e++)
        {
            TeCoord2D c = boxes[e].center();
            TePoint p(c);

            std::vector<TeBox>::const_iterator bit = boxes.begin();
            int k = 0;
            for (std::vector<TePolygon>::const_iterator it = polygons.begin();
                it != polygons.end() && bit != boxes.end();
                ++it, ++bit, ++k)
            {
                if (TeWithin(c, *bit) && TeWithin(c, *it))
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

