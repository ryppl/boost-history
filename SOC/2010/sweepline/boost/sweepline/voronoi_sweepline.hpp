// Boost sweepline/voronoi_segment_sweepline.hpp header file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_SWEEPLINE_VORONOI_SEGMENT_SWEEPLINE
#define BOOST_SWEEPLINE_VORONOI_SEGMENT_SWEEPLINE

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

#include "voronoi_segment_output.hpp"

#include "detail/voronoi_segment_formation.hpp"

#include "voronoi_segment_builder.hpp"

#endif
