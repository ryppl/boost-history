// Generic Geometry Library - Star Comb Overlay Comparisons
//
// Copyright Barend Gehrels, 2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include "../common/starcomb.hpp"

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


using namespace geos;
using namespace geos::geom;

GeometryFactory *global_factory;


void add(CoordinateSequence& seq, double x, double y, int)
{
    seq.add(Coordinate(x, y));
}


int main(int argc, char** argv)
{
    bool do_union;
    int testcount, starcount, combcount;
    double factor1, factor2;
    parse(argc, argv, testcount, starcount, combcount, factor1, factor2, do_union);

    geos::geom::PrecisionModel *pm = new geos::geom::PrecisionModel(geos::geom::PrecisionModel::FLOATING);
    geos::geom::GeometryFactory *global_factory = new geos::geom::GeometryFactory(pm, -1);
    delete pm;

    geos::geom::Geometry* star;
    geos::geom::Geometry* comb;

    {
        CoordinateSequence *star_seq = new CoordinateArraySequence();
        make_star(*star_seq, add, starcount, factor1, factor2);
        star = global_factory->createPolygon(global_factory->createLinearRing(star_seq), NULL);

        CoordinateSequence *comb_seq = new CoordinateArraySequence();
        make_comb(*comb_seq, add, combcount);
        comb = global_factory->createPolygon(global_factory->createLinearRing(comb_seq), NULL);
    }

    double area = 0;
    boost::timer t;
    for (int i = 0; i < testcount; i++)
    {
        geos::geom::Geometry *g;
        if (do_union)
        {
            g = star->Union(comb);
        }
        else
        {
            g = star->intersection(comb);
        }
        area += g->getArea();
        global_factory->destroyGeometry(g);
    }

    report("GEOS", area, t);

    global_factory->destroyGeometry(star);
    global_factory->destroyGeometry(comb);

    return 0;
}
