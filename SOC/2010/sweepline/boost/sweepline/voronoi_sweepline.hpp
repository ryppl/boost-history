// Boost sweepline/voronoi_sweepline.hpp header file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_SWEEPLINE_VORONOI_SWEEPLINE
#define BOOST_SWEEPLINE_VORONOI_SWEEPLINE

#include <algorithm>
#include <cmath>
#include <cstring>
#include <list>
#include <map>
#include <queue>
#include <vector>

#ifdef USE_MULTI_PRECISION_LIBRARY 
    #pragma warning( disable : 4800 )
    #include "gmpxx.h"
#endif

#include "voronoi_output.hpp"
#include "detail/voronoi_formation.hpp"

namespace boost {
namespace sweepline {

    template <typename T>
    static void build_voronoi(const std::vector< point_2d<T> > &points,
                              voronoi_output<double> &output) {
        std::vector< std::pair< point_2d<T>, point_2d<T> > > segments_empty;
        build_voronoi<T>(points, segments_empty, output);
    }

    template <typename T>
    static void build_voronoi(const std::vector< std::pair< point_2d<T>, point_2d<T> > > &segments,
                              voronoi_output<double> &output) {
        std::vector< point_2d<T> > points_empty;
        build_voronoi<T>(points_empty, segments, output);
    }

    template <typename T>
    static void build_voronoi(const std::vector< point_2d<T> > &points,
                              const std::vector< std::pair< point_2d<T>, point_2d<T> > > &segments,
                              voronoi_output<double> &output) {
        detail::voronoi_builder<double> builder(output);
        builder.init(points, segments);
        builder.run_sweepline();
    }

} // sweepline
} // boost

#endif
