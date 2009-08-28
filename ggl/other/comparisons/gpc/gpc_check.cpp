

#include "../common/common.hpp"
#include "../common/read_shapefile.hpp"


#include <cstdlib>
#include <cmath>

extern "C"
{
#include "gpc.h"
}

#include <iomanip>



template <typename P>
inline void allocate_polygon(P& polygon, int n)
{
    polygon.hole = (int*) malloc(1 * sizeof(int));
    polygon.contour =  (gpc_vertex_list*) malloc(1 * sizeof(gpc_vertex_list));
    polygon.contour[0].vertex = (gpc_vertex*) malloc(n * sizeof(gpc_vertex));

    polygon.num_contours = 1;
    polygon.hole[0] = 0;
    polygon.contour[0].num_vertices = n;
}


template <typename T>
T convert(SHPObject* psShape)
{
    const double* x = psShape->padfX;
    const double* y = psShape->padfY;

    int n = psShape->nVertices;

    T polygon;

    allocate_polygon(polygon, n);

    // Add it in CLOCKWISE direction
    for (int v = 0; v < n; v++, x++, y++)
    {
        polygon.contour[0].vertex[v].x = *x;
        polygon.contour[0].vertex[v].y = *y;

    }
    return polygon;
}


template <typename P, typename E>
void envelope_for_gpc(P const& polygon, E& envelope)
{
    envelope.first.x = 1e300;
    envelope.first.y = 1e300;
    envelope.second.x = -1e300;
    envelope.second.y = -1e300;
    int n = polygon.contour[0].num_vertices;
    const gpc_vertex* v = polygon.contour[0].vertex;
    for (int i = 0; i < n; i++, v++)
    {
        if (v->x < envelope.first.x) envelope.first.x = v->x;
        if (v->y < envelope.first.y) envelope.first.y = v->y;
        if (v->x > envelope.second.x) envelope.second.x = v->x;
        if (v->y > envelope.second.y) envelope.second.y = v->y;
    }
}

template <typename P>
inline double area_for_gpc(P const& polygon, bool close = false)
{
    double a = 0.0;
    const int n = polygon.num_contours;
    for (int c = 0; c < n; c++)
    {
        int n = polygon.contour[c].num_vertices;
        const gpc_vertex* v = polygon.contour[c].vertex;
        const gpc_vertex* w = v++;
        for (int i = 1; i < n; i++, w = v++)
        {
            a += w->x * v->y - v->x * w->y;
        }
        if (close)
        {
            // add closing point
            v = polygon.contour[c].vertex;
            a += w->x * v->y - v->x * w->y;
        }
    }
    return std::abs(a / 2.0);
}



template <typename B, typename P>
inline void box_as_polygon(B const& box, P& polygon)
{
    int n = 5;

    allocate_polygon(polygon, 5);

    polygon.contour[0].vertex[0].x = box.first.x;
    polygon.contour[0].vertex[0].y = box.first.y;

    polygon.contour[0].vertex[1].x = box.first.x;
    polygon.contour[0].vertex[1].y = box.second.y;

    polygon.contour[0].vertex[2].x = box.second.x;
    polygon.contour[0].vertex[2].y = box.second.y;

    polygon.contour[0].vertex[3].x = box.second.x;
    polygon.contour[0].vertex[3].y = box.first.y;

    polygon.contour[0].vertex[4].x = box.first.x;
    polygon.contour[0].vertex[4].y = box.first.y;
}





int main(int argc, char** argv)
{
    compare::version_info("gpc");

    typedef gpc_vertex POINT;
    typedef gpc_polygon POLY;
    typedef std::pair<POINT, POINT> BOX;

    std::vector<POLY> polygons;
    std::vector<BOX> boxes;
    std::vector<BOX> boxes10; // for clips
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
        envelope_for_gpc(*it, b);
        boxes.push_back(b);

        // Decrease for clipping
        b.first.x += compare::CLIP_DISTANCE;
        b.first.y += compare::CLIP_DISTANCE;
        b.second.x -= compare::CLIP_DISTANCE;
        b.second.y -= compare::CLIP_DISTANCE;

        boxes10.push_back(b);
    }


    // Create the ellipses for intersections lateron
    if (compare::MEASURE_OVERLAY)
    {
        ellipses.resize(boxes.size());

        int i = 0;
        for (std::vector<BOX>::const_iterator bit = boxes.begin();
            bit != boxes.end();
            ++bit, i++)
        {
            double cx = 0.5 * (bit->first.x + bit->second.x);
            double cy = 0.5 * (bit->first.y + bit->second.y);

            double dx = bit->second.x - bit->first.x;
            double dy = bit->second.y - bit->first.y;

            double a1 = compare::OVERLAY_ELLIPSE_FACTOR1 * 0.5 * dx;
            double b1 = compare::OVERLAY_ELLIPSE_FACTOR1 * 0.5 * dy;
            double a2 = compare::OVERLAY_ELLIPSE_FACTOR2 * 0.5 * dx;
            double b2 = compare::OVERLAY_ELLIPSE_FACTOR2 * 0.5 * dy;

            double angle = 0.0;


            POLY& ellipse = ellipses[i];
            allocate_polygon(ellipse, compare::OVERLAY_ELLIPSE_COUNT);

            for (int v = 0; v < compare::OVERLAY_ELLIPSE_COUNT - 1; v++, angle += compare::delta)
            {
                if (v % 2 == 0)
                {
                    ellipse.contour[0].vertex[v].x = cx + a1 * sin(angle);
                    ellipse.contour[0].vertex[v].y = cy + b1 * cos(angle);
                }
                else
                {
                    ellipse.contour[0].vertex[v].x = cx + a2 * sin(angle);
                    ellipse.contour[0].vertex[v].y = cy + b2 * cos(angle);
                }
            }
            // close
            int last = compare::OVERLAY_ELLIPSE_COUNT - 1;
            ellipse.contour[0].vertex[last].x = ellipse.contour[0].vertex[0].x;
            ellipse.contour[0].vertex[last].y = ellipse.contour[0].vertex[0].y;
        }
    }


    compare::wait();


    // OVERLAY
    if (compare::MEASURE_OVERLAY)
    {
        bool first = true;
        double area1 = 0.0, area2 = 0.0;

        std::cout << std::setprecision(16);

        boost::timer t;
        for (int i = 0; i < compare::OVERLAY_COUNT; i++)
        {

            int k = 0;
            //std::vector<BOX>::const_iterator bit = boxes10.begin();
            std::vector<POLY>::iterator eit = ellipses.begin();
            for (std::vector<POLY>::iterator it = polygons.begin();
                it != polygons.end() && eit != ellipses.end();
                ++it, ++eit, ++k)
            {
                if (compare::OVERLAY_AREA)
                {
                    area1 += area_for_gpc(*it);
                }

                POLY& subject = *it;
                POLY& clip = *eit;
                POLY result;

                //box_as_polygon(*bit, clip);
                gpc_polygon_clip(
#if defined(OVERLAY_UNION)
                    GPC_UNION
#elif defined(OVERLAY_SYMDIFF)
                    GPC_XOR
#else
                    GPC_INT
#endif
                    , &subject, &clip, &result);

                if (compare::OVERLAY_AREA)
                {
                    area2 += area_for_gpc(result, true);
                }

                //gpc_free_polygon(&clip);
                gpc_free_polygon(&result);
            }
        }
        compare::report_overlay(t, polygons.size(), area1, area2);
    }


    for (int i = 0; i < polygons.size(); i++)
    {
        gpc_free_polygon(&(polygons[i]));
        gpc_free_polygon(&(ellipses[i]));
    }

    return 0;
}
