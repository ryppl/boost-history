// Copyright Barend Gehrels, 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <vector>
#include <sstream>
#include <iomanip>
#include <geos/geom/PrecisionModel.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/Point.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/GeometryCollection.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/CoordinateArraySequence.h>
#include <geos/geom/IntersectionMatrix.h>

#include <geos/util/GeometricShapeFactory.h>
#include <geos/util/GEOSException.h>
#include <geos/util/IllegalArgumentException.h>
#include <geos/opLinemerge.h>
#include <geos/opPolygonize.h>

#include <geos/simplify/DouglasPeuckerSimplifier.h>

#include <geos/io/WKTWriter.h>



#include "../common/common.hpp"
#include "../common/read_shapefile.hpp"

using namespace geos;
using namespace geos::geom;

GeometryFactory *global_factory;


template <typename T>
T convert(SHPObject* psShape)
{
    const double* x = psShape->padfX;
    const double* y = psShape->padfY;

    // We will use a coordinate list to build the linearring
    CoordinateSequence *cl = new CoordinateArraySequence();

    for (int v = 0; v < psShape->nVertices; v++)
    {
        cl->add(Coordinate(x[v], y[v]));
    }
    LinearRing *lr = global_factory->createLinearRing(cl);
    return global_factory->createPolygon(lr, NULL);
}






void write_envelope(const Envelope* e)
{
    std::cout << e->getMinX() << "," << e->getMinY()
        << "," << e->getMaxX() << "," << e->getMaxY() << std::endl;
}

int main(int argc, char** argv)
{
    try
    {
        compare::version_info("geos");

        PrecisionModel *pm = new PrecisionModel(geos::geom::PrecisionModel::FLOATING);
        global_factory = new GeometryFactory(pm, -1);
        delete pm;

        geos::io::WKTWriter writer; // for checking things


        std::vector<Geometry*> polygons;
        std::vector<Geometry*> ellipses;
        std::vector<int> ids;
        read_shapefile(compare::filename(argc, argv), compare::fieldname(), polygons, ids, convert<Geometry*>);


        // Create envelopes
        std::vector<const Envelope*> envelopes;
        std::vector<Geometry*> boxes; // decreased, for intersection
        {
            boost::timer t;
            for (std::vector<Geometry*>::const_iterator it = polygons.begin();
                it != polygons.end(); ++it)
            {
                Geometry* poly = *it;
                const Envelope* e = poly->getEnvelopeInternal();

                Envelope* b = new Envelope(*e);
                envelopes.push_back(e);

                b->expandBy(- compare::CLIP_DISTANCE);

                Geometry* g = global_factory->toGeometry(b);
                boxes.push_back(g);
            }
            //std::cout << "ENVELOPE " << polygons.size() << " in " << t.elapsed() << "s" << std::endl;
        }

        // Create the ellipses for intersections lateron
        if (compare::MEASURE_OVERLAY)
        {
            for (std::vector<const Envelope*>::const_iterator eit = envelopes.begin();
                    eit != envelopes.end();
                    ++eit)
            {
                double factor = 0.9;
                double cx = 0.5 * ((*eit)->getMinX() + (*eit)->getMaxX());
                double cy = 0.5 * ((*eit)->getMinY() + (*eit)->getMaxY());

                double dx = (*eit)->getMaxX() - (*eit)->getMinX();
                double dy = (*eit)->getMaxY() - (*eit)->getMinY();

                double a1 = compare::OVERLAY_ELLIPSE_FACTOR1 * 0.5 * dx;
                double b1 = compare::OVERLAY_ELLIPSE_FACTOR1 * 0.5 * dy;
                double a2 = compare::OVERLAY_ELLIPSE_FACTOR2 * 0.5 * dx;
                double b2 = compare::OVERLAY_ELLIPSE_FACTOR2 * 0.5 * dy;

                double angle = 0.0;

                CoordinateSequence *cl = new CoordinateArraySequence();

                for (int v = 0; v < compare::OVERLAY_ELLIPSE_COUNT - 1; v++, angle += compare::delta)
                {
                    if (v % 2 == 0)
                    {
                        cl->add(Coordinate(cx + a1 * sin(angle), cy + b1 * cos(angle)));
                    }
                    else
                    {
                        cl->add(Coordinate(cx + a2 * sin(angle), cy + b2 * cos(angle)));
                    }
                }
                // Close it (when we use the calculation we get a "non closed" exception)
                cl->add(cl->front());

                LinearRing *lr = global_factory->createLinearRing(cl);
                ellipses.push_back(global_factory->createPolygon(lr, NULL));
            }
        }


        compare::wait();


        if (compare::MEASURE_AREA)
        {
            double area = 0;
            boost::timer t;
            for (int i = 0; i < compare::AREA_COUNT; i++)
            {
                for (std::vector<Geometry*>::const_iterator it = polygons.begin();
                    it != polygons.end(); ++it)
                {
                    Geometry* poly = *it;
                    area += poly->getArea();
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
                for (std::vector<Geometry*>::const_iterator it = polygons.begin();
                    it != polygons.end();
                    ++it)
                {
                    Geometry* centroid = (*it)->getCentroid();
                    Point* c = (Point*) centroid;
                    sum_x += c->getX();
                    sum_y += c->getY();
                    global_factory->destroyGeometry(centroid);
                }
            }
            compare::report_centroid(t, polygons.size(), sum_x, sum_y);
        }


        if (compare::MEASURE_CONVEX_HULL)
        {
            double area = 0;
            boost::timer t;
            for (std::vector<Geometry*>::const_iterator it = polygons.begin();
                it != polygons.end(); ++it)
            {
                Geometry* hull = (*it)->convexHull();
                area += hull->getArea();
                global_factory->destroyGeometry(hull);
            }
            compare::report_hull(t, polygons.size(), area);
        }


        if (compare::MEASURE_OVERLAY)
        {
            bool first = true;
            bool first_exception = true;
            int exception_count = 0;
            double area1 = 0, area2 = 0;
            std::cout << std::setprecision(16);
            std::string exceptions;

            boost::timer t;
            for (int i = 0; i < compare::OVERLAY_COUNT; i++)
            {
                int k = 0;
                std::vector<Geometry*>::const_iterator eit = ellipses.begin();
                for (std::vector<Geometry*>::const_iterator it = polygons.begin();
                        it != polygons.end() && eit != ellipses.end();
                        ++it, ++eit, ++k)
                {
                    //if (k == 768)
                    {
                        bool has_exception = true;
                        Geometry* poly = *it;
                        Geometry* ellipse = *eit;

                        /***
                        POLYGON 768 has a problem when factor=1 (assertion failed), I'll report this.
                        std::cout << k << ": select ST_Area(intersection"
                            << "(GeometryFromText('"
                            <<  writer.write(poly)
                            << "'),GeometryFromText('" << writer.write(ellipse)
                            << "')));"
                            << std::endl;
                        ***/

                        Geometry *g = NULL;
                        try
                        {
                            g = poly->intersection(ellipse);

                            if (compare::OVERLAY_AREA)
                            {
                                area1 += poly->getArea();
                                area2 += g->getArea();
                                //std::cout << k << ": " << g->getArea() << std::endl;
                            }

                            has_exception = false;
                        }
                        catch(const std::exception& e)
                        {
                            exceptions += e.what();
                            exceptions += "\n";
                            exception_count++;

                            if (first_exception)
                            {
                                std::cout << e.what() << std::endl;


                                first_exception = false;
                            }
                        }
                        catch(...)
                        {
                            exception_count++;
                        }
                        /*if (first && ! has_exception)
                        {
                            first = false;
                            std::cout << "Intersected: " << k << " n0: " << poly->getNumPoints()
                                << " -> " << g->getNumGeometries() << "("
                                << g->getNumPoints()
                                << ")" << std::endl;
                            first = false;
                        }*/

                        global_factory->destroyGeometry(g);
                    }
                }
            }
            compare::report_overlay(t, polygons.size(), area1, area2);
            if (exception_count > 0)
            {
                std::cout << exception_count << " exceptions:" << std::endl;
                    //<< exceptions << std::endl;
            }
        }


        if (compare::MEASURE_SIMPLIFY)
        {
            int count1 = 0, count2 = 0;
            double length1 = 0.0, length2 = 0.0;
            boost::timer t;
            for (std::vector<Geometry*>::const_iterator it = polygons.begin();
                it != polygons.end();
                ++it)
            {
                Geometry* poly = *it;
                count1 += poly->getNumPoints();
                std::auto_ptr<Geometry> simplified = geos::simplify::DouglasPeuckerSimplifier::simplify(poly, compare::SIMPLIFY_DISTANCE);
                count2 += simplified->getNumPoints();

                if (compare::SIMPLIFY_LENGTH)
                {
                    length1 += ((Polygon *) poly)->getLength();
                    length2 += ((Polygon *) simplified.get())->getLength();
                }

            }
            compare::report_simplify(t, polygons.size(), length1, length2, count1, count2);
        }

        if (compare::MEASURE_TOUCH)
        {
            int count = 0;
            int count_non_disjoint = 0;
            boost::timer t;
            int e1 = 0;
            for (std::vector<Geometry*>::const_iterator it1 = polygons.begin();
                it1 != polygons.end(); ++it1, e1++)
            {
                int e2 = 0;
                for (std::vector<Geometry*>::const_iterator it2 = polygons.begin();
                    it2 != polygons.end(); ++it2, e2++)
                {
                    Envelope env1 = *envelopes[e1];
                    Envelope env2 = *envelopes[e2];
                    env1.expandBy(2.0);
                    env2.expandBy(2.0);
                    if (env1.intersects(env2))
                    {
                        count_non_disjoint++;
                        //std::string r = (*it1)->relate(*it2)->toString();
                        if (! (*it1)->disjoint(*it2))
                        {
                            /*
                            std::cout << ids[e1] << " - " << ids[e2]
                                //<< " " << r
                                << std::endl;
                            */
                            count++;
                        }
                    }
                }
            }
            compare::report_touch(t, polygons.size(), count, count_non_disjoint);
        }


        if (compare::MEASURE_WITHIN)
        {
            int count = 0;
            boost::timer t;
            for (int e = 0; e < envelopes.size(); e++)
            {
                Coordinate c;
                envelopes[e]->centre(c);

                Point *p = global_factory->createPoint(c);

                int k = 0;
                std::vector<const Envelope*>::const_iterator bit = envelopes.begin();
                for (std::vector<Geometry*>::const_iterator it = polygons.begin();
                    it != polygons.end() && bit != envelopes.end();
                    ++it, ++bit, k++)
                {
                    /***
                    if ((e == 85 && k == 85)
                        || e == 99 && k == 99
                        )
                    {
                        std::cout << e << "," << k << std::endl;
                        write_envelope(envelopes[e]);
                        std::cout << p->getX() << "," << p->getY() << std::endl;
                    }
                    ***/

                    // First check bbox (like all other tests) (though GEOS does this also internally)
                    // (perf.difference: ca 1%)
                    if ((*bit)->contains(c))
                    {
                        Geometry* poly = *it;
                        if (p->within(poly))
                        {
                            //std::cout << e << " IN " << k << std::endl;
                            count++;
                        }
                    }
                }
                global_factory->destroyGeometry(p);
            }
            compare::report_within(t, polygons.size(), count, -1);
        }


        for (int i = 0; i < polygons.size(); i++)
        {
            global_factory->destroyGeometry(polygons[i]);
        }
        for (int i = 0; i < ellipses.size(); i++)
        {
            global_factory->destroyGeometry(ellipses[i]);
        }
        for (int i = 0; i < boxes.size(); i++)
        {
            global_factory->destroyGeometry(boxes[i]);
        }
    }
    catch(std::exception const& e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}

