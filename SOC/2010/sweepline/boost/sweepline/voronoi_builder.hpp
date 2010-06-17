// Boost sweepline/voronoi_builder.hpp header file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_SWEEPLINE_VORONOI_BUILDER
#define BOOST_SWEEPLINE_VORONOI_BUILDER

#include <vector>

namespace boost {
namespace sweepline {

    template <typename Point2D, typename BeachLine>
    class voronoi_builder {
    public:
        voronoi_builder() {}

        void init(const std::vector<Point2D> &sites) {
            beach_line_.init(sites, output_);
        }

        void reset() {
            beach_line_.reset();
        }

        void build_voronoi_diagram() {
            beach_line_.run_sweepline();
        }

    private:
        BeachLine beach_line_;
    };

} // sweepline
} // boost

#endif