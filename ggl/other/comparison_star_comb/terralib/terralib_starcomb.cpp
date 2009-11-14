// Generic Geometry Library - Star Comb Overlay Comparisons
//
// Copyright Barend Gehrels, 2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include "../common/starcomb.hpp"

#include <TeVersion.h>
#include <TeDatabase.h>
#include <TeGeometryAlgorithms.h>
#include <TeOverlay.h>

#include "TeException.h"


void add(TeLinearRing& ring, double x, double y, int)
{
    ring.add(TeCoord2D(x, y));
}


int main(int argc, char** argv)
{
    bool do_union;
    int testcount, starcount, combcount;
    double factor1, factor2;
    parse(argc, argv, testcount, starcount, combcount, factor1, factor2, do_union);

    TePolygonSet star, comb;
    {
        TeLinearRing star_ring, comb_ring;

        make_star(star_ring, add, starcount, factor1, factor2);
        make_comb(comb_ring, add, combcount);

        TePolygon star_poly, comb_poly;
        star_poly.add(star_ring);
        comb_poly.add(comb_ring);

        star.add(star_poly);
        comb.add(comb_poly);
    }


    double area = 0;
    boost::timer t;
    for (int i = 0; i < testcount; i++)
    {
        TePolygonSet v;
        if (do_union)
        {
            TeOVERLAY::TeUnion(star, comb, v);
        }
        else
        {
            TeOVERLAY::TeIntersection(star, comb, v);
        }

        for (int i = 0; i < v.size(); i++)
        {
            area += TeGeometryArea(v[i]);
        }
    }

    report("terralib", area, t);

    return 0;
}
