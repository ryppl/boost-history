// Generic Geometry Library - COMPARISONS
//
// Copyright Barend Gehrels, 2008, 2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)




#include "../common/common.hpp"
#include "../common/read_shapefile.hpp"

#include <boost/polygon/polygon.hpp>




// using "double" for polygons with GTL-Booleans
// - causes a crash in MinGW [[still AUG 25]]
// - causes a segmentation fault in Linux Ubuntu (gcc 4.3.2)
// - causes a crash in MSVC2008 (An unhandled win32 exception occured in gtl_check2008.exe [7544].) [[still AUG 25]]

//#define BOOLEANS_DOUBLE

#ifdef BOOLEANS_DOUBLE
typedef double type_for_booleans;
const int GTL_INTEGER_FACTOR = 1.0;
#else
typedef int type_for_booleans;
const int GTL_INTEGER_FACTOR = compare::INTEGER_FACTOR;
#endif

const int GTL_INTEGER_FACTOR_SQR = GTL_INTEGER_FACTOR * GTL_INTEGER_FACTOR;



// "Factor" is necessary to round coordinates
template <typename P, typename IP>
void convert_polygon(P const& double_polygon, IP& integer_polygon)
{
    typedef boost::polygon::point_data<type_for_booleans> point_type;
    std::vector<point_type> points;

    typename boost::polygon::polygon_traits<P>::iterator_type it = double_polygon.begin();
    int n = double_polygon.size();
    for(unsigned int i = 0; i < n; i++, ++it)
    {
         points.push_back(point_type(
                GTL_INTEGER_FACTOR * it->x(),
                GTL_INTEGER_FACTOR * it->y()));
    }

    boost::polygon::polygon_data<type_for_booleans> polygon;
    polygon.set(points.begin(), points.end());
    integer_polygon.insert(polygon);
}



template <typename T>
inline T convert(SHPObject* psShape)
{
    const double* x = psShape->padfX;
    const double* y = psShape->padfY;

    std::vector<boost::polygon::point_data<double> > points;

    for (int v = 0; v < psShape->nVertices; v++)
    {
        boost::polygon::point_data<double> pt(x[v], y[v]);
        points.push_back(pt);
    }

    boost::polygon::polygon_data<double> polygon;
    polygon.set(points.begin(), points.end());

    return polygon;
}



int main(int argc, char** argv)
{
    try
    {
        compare::version_info("gtl");

        typedef boost::polygon::polygon_data<double> POLY;
        typedef boost::polygon::rectangle_data<double> BOX;
        typedef boost::polygon::point_data<double> POINT;

        std::vector<POLY> polygons;
        std::vector<POLY> ellipses;
        std::vector<BOX> boxes;
        std::vector<int> ids;

        read_shapefile(compare::filename(argc, argv), compare::fieldname(), polygons, ids, convert<POLY>);


        // Create envelopes
        for (std::vector<POLY>::const_iterator it = polygons.begin();
            it != polygons.end();
            ++it)
        {
            BOX b;
            boost::polygon::extents(b, *it);
            boxes.push_back(b);
        }


        // Create the ellipses for intersections lateron
        if (compare::MEASURE_OVERLAY)
        {
            for (std::vector<BOX>::const_iterator bit = boxes.begin();
                bit != boxes.end();
                ++bit)
            {
                double x1 = bit->get(boost::polygon::HORIZONTAL).low();
                double y1 = bit->get(boost::polygon::VERTICAL).low();
                double x2 = bit->get(boost::polygon::HORIZONTAL).high();
                double y2 = bit->get(boost::polygon::VERTICAL).high();

                double cx = 0.5 * (x1 + x2);
                double cy = 0.5 * (y1 + y2);

                double a1 = compare::OVERLAY_ELLIPSE_FACTOR1 * 0.5 * (x2 - x1);
                double b1 = compare::OVERLAY_ELLIPSE_FACTOR1 * 0.5 * (y2 - y1);
                double a2 = compare::OVERLAY_ELLIPSE_FACTOR2 * 0.5 * (x2 - x1);
                double b2 = compare::OVERLAY_ELLIPSE_FACTOR2 * 0.5 * (y2 - y1);

                double angle = 0.0;

                std::vector<boost::polygon::point_data<double> > points;

                for (int v = 0; v < compare::OVERLAY_ELLIPSE_COUNT - 1; v++, angle += compare::delta)
                {
                    if (v % 2 == 0)
                    {
                        points.push_back(boost::polygon::point_data<double>(cx + a1 * sin(angle), cy + b1 * cos(angle)));
                    }
                    else
                    {
                        points.push_back(boost::polygon::point_data<double>(cx + a2 * sin(angle), cy + b2 * cos(angle)));
                    }
                }
                points.push_back(points.front());

                POLY ellipse;
                ellipse.set(points.begin(), points.end());
                ellipses.push_back(ellipse);
            }
        }


        // Create integer-versions of the polygons
        // This is done BEFORE to not influence the performance test
        // by converting inside measurements.
        typedef boost::polygon::polygon_set_data<type_for_booleans> INT_POLY;
        std::vector<INT_POLY> integer_polygons;
        std::vector<INT_POLY> integer_ellipses;

        for (std::vector<POLY>::const_iterator it = polygons.begin();
            it != polygons.end();
            ++it)
        {
            INT_POLY ip;
            convert_polygon(*it, ip);
            integer_polygons.push_back(ip);

            // To check if conversion is OK
            //std::cout << boost::polygon::area(*it) << " " <<  (boost::polygon::area(ip) / GTL_INTEGER_FACTOR_SQR) << std::endl;
        }

        for (std::vector<POLY>::const_iterator it = ellipses.begin();
            it != ellipses.end();
            ++it)
        {
            INT_POLY ip;
            convert_polygon(*it, ip);
            integer_ellipses.push_back(ip);
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
                    area += boost::polygon::area(*it);
                }
            }
            compare::report_area(t, polygons.size(), area);
        }

        if (compare::MEASURE_OVERLAY)
        {
            double area1 = 0.0, area2 = 0.0;

            boost::timer t;
            for (int i = 0; i < compare::OVERLAY_COUNT; i++)
            {
                int k = 0;
                std::vector<INT_POLY>::const_iterator eit = integer_ellipses.begin();
                for (std::vector<INT_POLY>::const_iterator it = integer_polygons.begin();
                    it != integer_polygons.end() && eit != integer_ellipses.end();
                    ++it, ++eit, ++k)
                {
                    // * means intersection, + means union, ^ means XOR

#if defined(OVERLAY_UNION)
                    INT_POLY overlay = (*it) + (*eit);
#elif defined(OVERLAY_SYMDIFF)
                    INT_POLY overlay = (*it) ^ (*eit);
#else
                    //INT_POLY overlay = (*it) * (*eit); // does not compile anymore,
                    // so se do it like this:
                    INT_POLY overlay = boost::polygon::polygon_set_view<INT_POLY, INT_POLY, 1>(*it, *eit);
#endif
                    BOX b;
                    boost::polygon::extents(b, overlay);
                    if (compare::OVERLAY_AREA)
                    {
                        area1 += boost::polygon::area(*it) / GTL_INTEGER_FACTOR_SQR;
                        //area2 += boost::polygon::area(overlay) / GTL_INTEGER_FACTOR_SQR;
                        {
                            typedef std::list<boost::polygon::polygon_with_holes_data<type_for_booleans> > list;
                            double a = 0.0;
                            list polys;
                            overlay.get(polys);
                            for (list::const_iterator it = polys.begin(); it != polys.end(); ++it)
                            {
                                a += boost::polygon::area(*it);
                            }
                            area2 += a / GTL_INTEGER_FACTOR_SQR;



                            //std::cout << "insert into se_gtl values(" << k << ", " << (a / GTL_INTEGER_FACTOR_SQR) << ");" << std::endl;

                            //    CREATING QUERY
                            // select a.*,b.*,abs(a.area-b.area) as diff from se_ggl a left join se b on a.k=b.k order by diff desc
                        }
                    }
                }
            }
            compare::report_overlay(t, polygons.size(), area1, area2);
        }


        if (compare::MEASURE_WITHIN)
        {
            int count = 0;
            boost::timer t;
            for (int e = 0; e < boxes.size(); e++)
            {
                const BOX& b = boxes[e];
                POINT p;
                boost::polygon::center(p, b);

                //compare::debug_within(e, count);

                std::vector<BOX>::const_iterator bit = boxes.begin();
                for (std::vector<POLY>::const_iterator it = polygons.begin();
                    it != polygons.end() && bit != boxes.end();
                    ++it, ++bit)
                {
                    if (p.x() > bit->get(boost::polygon::HORIZONTAL).low()
                        && p.x() < bit->get(boost::polygon::HORIZONTAL).high()
                        && p.y() > bit->get(boost::polygon::VERTICAL).low()
                        && p.y() < bit->get(boost::polygon::VERTICAL).high())
                    {
                        if (boost::polygon::contains(*it, p))
                        {
                            count++;
                        }
                    }
                }
            }
            compare::report_within(t, polygons.size(), count, -1);
        }
    }
    catch(std::exception const& e)
    {
        std::cout << e.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "Exception encountered" << std::endl;
    }

    return 0;
}
