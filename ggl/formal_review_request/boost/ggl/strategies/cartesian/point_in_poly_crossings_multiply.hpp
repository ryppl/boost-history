// Generic Geometry Library
//
// Copyright Barend Gehrels 2009, Geodan, Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_STRATEGY_CARTESIAN_POINT_IN_POLY_CROSSINGS_MULTIPLY_HPP
#define GGL_STRATEGY_CARTESIAN_POINT_IN_POLY_CROSSINGS_MULTIPLY_HPP


#include <ggl/core/coordinate_type.hpp>
#include <ggl/util/select_calculation_type.hpp>


namespace ggl

{
namespace strategy { namespace within {

/*!
    \brief Within detection using cross counting,
    \see http://tog.acm.org/resources/GraphicsGems/gemsiv/ptpoly_haines/ptinpoly.c
    \note Does NOT work correctly for point ON border
 */

template
<
    typename Point,
    typename PointOfSegment = Point,
    typename CalculationType = void
>
class crossings_multiply
{
    typedef typename select_calculation_type
        <
            Point,
            PointOfSegment,
            CalculationType
        >::type calculation_type;

    class flags
    {
        bool inside_flag;
        bool first;
        bool yflag0;

    public :

        friend class crossings_multiply;

        inline flags()
            : inside_flag(false)
            , first(true)
            , yflag0(false)
        {}
    };

public :

    typedef Point point_type;
    typedef PointOfSegment segment_point_type;
    typedef flags state_type;

    static inline bool apply(Point const& point,
            PointOfSegment const& seg1, PointOfSegment const& seg2,
            flags& state)
    {
        calculation_type const tx = get<0>(point);
        calculation_type const ty = get<1>(point);
        calculation_type const x0 = get<0>(seg1);
        calculation_type const y0 = get<1>(seg1);
        calculation_type const x1 = get<0>(seg2);
        calculation_type const y1 = get<1>(seg2);

        if (state.first)
        {
            state.first = false;
            state.yflag0 = y0 >= ty;
        }


        bool yflag1 = y1 >= ty;
        if (state.yflag0 != yflag1)
        {
            if ( ((y1-ty) * (x0-x1) >= (x1-tx) * (y0-y1)) == yflag1 )
            {
                state.inside_flag = ! state.inside_flag;
            }
        }
        state.yflag0 = yflag1;
        return true;
    }

    static inline bool result(flags const& state)
    {
        return state.inside_flag;
    }
};



}} // namespace strategy::within


} // namespace ggl


#ifdef ORIGINAL_CODE

See http://tog.acm.org/resources/GraphicsGems/gemsiv/ptpoly_haines/ptinpoly.c

/* ======= Crossings Multiply algorithm =================================== */

/*
 * This version is usually somewhat faster than the original published in
 * Graphics Gems IV; by turning the division for testing the X axis crossing
 * into a tricky multiplication test this part of the test became faster,
 * which had the additional effect of making the test for "both to left or
 * both to right" a bit slower for triangles than simply computing the
 * intersection each time.  The main increase is in triangle testing speed,
 * which was about 15% faster; all other polygon complexities were pretty much
 * the same as before.  On machines where division is very expensive (not the
 * case on the HP 9000 series on which I tested) this test should be much
 * faster overall than the old code.  Your mileage may (in fact, will) vary,
 * depending on the machine and the test data, but in general I believe this
 * code is both shorter and faster.  This test was inspired by unpublished
 * Graphics Gems submitted by Joseph Samosky and Mark Haigh-Hutchinson.
 * Related work by Samosky is in:
 *
 * Samosky, Joseph, "SectionView: A system for interactively specifying and
 * visualizing sections through three-dimensional medical image data",
 * M.S. Thesis, Department of Electrical Engineering and Computer Science,
 * Massachusetts Institute of Technology, 1993.
 *
 */

/* Shoot a test ray along +X axis.  The strategy is to compare vertex Y values
 * to the testing point's Y and quickly discard edges which are entirely to one
 * side of the test ray.  Note that CONVEX and WINDING code can be added as
 * for the CrossingsTest() code; it is left out here for clarity.
 *
 * Input 2D polygon _pgon_ with _numverts_ number of vertices and test point
 * _point_, returns 1 if inside, 0 if outside.
 */
int CrossingsMultiplyTest( pgon, numverts, point )
double  pgon[][2] ;
int numverts ;
double  point[2] ;
{
register int    j, yflag0, yflag1, inside_flag ;
register double ty, tx, *vtx0, *vtx1 ;

    tx = point[X] ;
    ty = point[Y] ;

    vtx0 = pgon[numverts-1] ;
    /* get test bit for above/below X axis */
    yflag0 = ( vtx0[Y] >= ty ) ;
    vtx1 = pgon[0] ;

    inside_flag = 0 ;
    for ( j = numverts+1 ; --j ; ) {

    yflag1 = ( vtx1[Y] >= ty ) ;
    /* Check if endpoints straddle (are on opposite sides) of X axis
     * (i.e. the Y's differ); if so, +X ray could intersect this edge.
     * The old test also checked whether the endpoints are both to the
     * right or to the left of the test point.  However, given the faster
     * intersection point computation used below, this test was found to
     * be a break-even proposition for most polygons and a loser for
     * triangles (where 50% or more of the edges which survive this test
     * will cross quadrants and so have to have the X intersection computed
     * anyway).  I credit Joseph Samosky with inspiring me to try dropping
     * the "both left or both right" part of my code.
     */
    if ( yflag0 != yflag1 ) {
        /* Check intersection of pgon segment with +X ray.
         * Note if >= point's X; if so, the ray hits it.
         * The division operation is avoided for the ">=" test by checking
         * the sign of the first vertex wrto the test point; idea inspired
         * by Joseph Samosky's and Mark Haigh-Hutchinson's different
         * polygon inclusion tests.
         */
        if ( ((vtx1[Y]-ty) * (vtx0[X]-vtx1[X]) >=
            (vtx1[X]-tx) * (vtx0[Y]-vtx1[Y])) == yflag1 ) {
        inside_flag = !inside_flag ;
        }
    }

    /* Move to the next pair of vertices, retaining info as possible. */
    yflag0 = yflag1 ;
    vtx0 = vtx1 ;
    vtx1 += 2 ;
    }

    return( inside_flag ) ;
}
#endif


#endif // GGL_STRATEGY_CARTESIAN_POINT_IN_POLY_CROSSINGS_MULTIPLY_HPP
