// Boost sweepline/voronoi_sweepline.hpp header file

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_SWEEPLINE_VORONOI_SWEEPLINE
#define BOOST_SWEEPLINE_VORONOI_SWEEPLINE

#include <algorithm>
#include <cmath>
#include <cstring>
#include <list>
#include <map>
#include <queue>
#include <stack>
#include <vector>

#ifndef BOOST_POLYGON_USE_LONG_LONG
#include <boost/config.hpp>
#ifdef BOOST_HAS_LONG_LONG
#define BOOST_POLYGON_USE_LONG_LONG
typedef boost::long_long_type polygon_long_long_type;
typedef boost::ulong_long_type polygon_ulong_long_type;
#endif
#endif

#include "voronoi_output.hpp"
//#include "detail/mpz_arithmetic.hpp"
#include "detail/voronoi_formation.hpp"

namespace boost {
namespace sweepline {

    // Public methods to compute voronoi diagram.
    // points - vector of input points.
    // segments - vector of input segments.
    // output - voronoi output data structure to hold voronoi diagram.
    // The assumption is made that input doesn't contain segments that
    // intersect or points lying on the segments. Also coordinates of
    // the points and of the endpoints of the segments should be within
    // signed integer range [-2^31, 2^31-1].
    // Complexity - O(N*logN), memory usage - O(N), where N is the total
    // number of points and segments.

    template <typename T>
    static inline void build_voronoi(const std::vector< point_2d<T> > &points,
                                     voronoi_output<double> &output) {
        std::vector< std::pair< point_2d<T>, point_2d<T> > > segments_empty;
        build_voronoi<T>(points, segments_empty, output);
    }

    template <typename T>
    static inline void build_voronoi(
          const std::vector< std::pair< point_2d<T>, point_2d<T> > > &segments,
          voronoi_output<double> &output) {
        std::vector< point_2d<T> > points_empty;
        build_voronoi<T>(points_empty, segments, output);
    }

    template <typename T>
    static inline void build_voronoi(
          const std::vector< point_2d<T> > &points,
          const std::vector< std::pair< point_2d<T>, point_2d<T> > > &segments,
          voronoi_output<double> &output) {
        detail::voronoi_builder<double> builder(output);
        builder.init(points, segments);
        builder.run_sweepline();
    }

} // sweepline
} // boost

#endif
