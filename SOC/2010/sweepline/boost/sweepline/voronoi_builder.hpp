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

    template <typename Point2D, typename EventQueue, typename BeachLine>
    class voronoi_builder {
    public:
        voronoi_builder() {}
        
        ~voronoi_builder() {
            if (event_queue_) {
                delete event_queue_;
                event_queue_ = NULL;
            }

            if (beach_line_) {
                delete beach_line_;
                beach_line_ = NULL;
            }
        }

        void init(const std::vector<Point2D> &sites) {
            event_queue_->init(sites);
        }

        void reset() {
            event_queue_->reset();
            beach_line_->reset();
        }

        void build_voronoi_diagram() {
            while (!event_queue_->empty())
                make_one_iteration();
        }

    private:
        void make_one_iteration() {
            //Not implemented yet
        }

        EventQueue* event_queue_;
        BeachLine* beach_line_;
    };

} // sweepline
} // boost

#endif