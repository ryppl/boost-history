// Boost sweepline/voronoi_segment_output.hpp header file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_SWEEPLINE_VORONOI_SEGMENT_OUTPUT
#define BOOST_SWEEPLINE_VORONOI_SEGMENT_OUTPUT

namespace boost {
namespace sweepline {
    template <typename T>
    struct point_2d {
    public:
        typedef T coordinate_type;

        point_2d() {}

        point_2d(T x, T y) {
            x_ = x;
            y_ = y;
        }

        bool operator==(const point_2d &point) const {
            return (this->x_ == point.x()) && (this->y_ == point.y());
        }

        bool operator!=(const point_2d &point) const {
            return (this->x_ != point.x()) || (this->y_ != point.y());
        }

        // This comparator actually defines sweepline movement direction.
        bool operator<(const point_2d &point) const {
            // Sweepline sweeps from left to right.
            if (this->x_ != point.x_)
                return this->x_ < point.x_;
            return this->y_ < point.y_;
        }

        bool operator<=(const point_2d &point) const {
            return !(point < (*this));
        }

        bool operator>(const point_2d &point) const {
            return point < (*this);
        }

        bool operator>=(const point_2d &point) const {
            return !((*this) < point);
        }

        coordinate_type x() const {
            return this->x_;
        }

        coordinate_type y() const {
            return this->y_;
        }

        void x(coordinate_type x) {
            x_ = x;
        }

        void y(coordinate_type y) {
            y_ = y;
        }

    private:
        coordinate_type x_;
        coordinate_type y_;
    };

    template <typename T>
    point_2d<T> make_point_2d(T x, T y) {
        return point_2d<T>(x,y);
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // VORONOI OUTPUT TYPES ///////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
	namespace detail {
		template <typename T>
		struct site_event;
	}

    // Bounding rectangle data structure.
    template <typename T>
    struct BRect {
    public:
        typedef T coordinate_type;
        typedef point_2d<T> Point2D;

        coordinate_type x_min;
        coordinate_type y_min;
        coordinate_type x_max;
        coordinate_type y_max;

        BRect() {}

        BRect(const Point2D &p1, const Point2D &p2) {
            x_min = (std::min)(p1.x(), p2.x());
            y_min = (std::min)(p1.y(), p2.y());
            x_max = (std::max)(p1.x(), p2.x());
            y_max = (std::max)(p1.y(), p2.y());
        }

        BRect(coordinate_type x_mn, coordinate_type y_mn,
              coordinate_type x_mx, coordinate_type y_mx) {
             x_min = (std::min)(x_mn, x_mx);
             y_min = (std::min)(y_mn, y_mx);
             x_max = (std::max)(x_mn, x_mx);
             y_max = (std::max)(y_mn, y_mx);
        }

        void update(const Point2D &p) {
            x_min = (std::min)(x_min, p.x());
            y_min = (std::min)(y_min, p.y());
            x_max = (std::max)(x_max, p.x());
            y_max = (std::max)(y_max, p.y());
        }

        bool contains(const Point2D &p) const {
            return p.x() > x_min && p.x() < x_max && p.y() > y_min && p.y() < y_max;
        }

        bool is_valid() const {
            return (x_min < x_max) && (y_min < y_max);
        }
    };

	template <typename T>
	class Helper {
	public:
		typedef T coordinate_type;

		enum kEdgeType {
			SEGMENT = 0,
			RAY = 1,
			LINE = 2,
		};

		// Find edge(segment, ray, line) rectangle intersetion points.
		static bool find_intersections(const point_2d<coordinate_type> &origin,
									   const point_2d<coordinate_type> &direction,
									   kEdgeType edge_type, const BRect<coordinate_type> &brect,
									   std::vector< point_2d<coordinate_type> > &intersections) {
			coordinate_type half = static_cast<coordinate_type>(0.5);

			// Find rectangle center.
			coordinate_type center_x = (brect.x_min + brect.x_max) * half;
			coordinate_type center_y = (brect.y_min + brect.y_max) * half;

			// Find rectangle half-diagonal vector.
			coordinate_type len_x = brect.x_max - center_x;
			coordinate_type len_y = brect.y_max - center_y;
	        
			// Find distance between origin and center of rectangle.
			coordinate_type diff_x = origin.x() - center_x;
			coordinate_type diff_y = origin.y() - center_y;
	        
			// Find direction perpendicular to the current.
			coordinate_type perp_x = direction.y();
			coordinate_type perp_y = -direction.x();

			// Compare projections of distances.
			coordinate_type lexpr = magnitude(perp_x * diff_x + perp_y * diff_y);
			coordinate_type rexpr = magnitude(perp_x * len_x) + magnitude(perp_y * len_y);

			// Intersection check.
			if (lexpr > rexpr)
				return false;

			// Intersection parameters:
			// origin + fT[i] * direction = intersections point, i = 0 .. 1.
			bool fT0_used = false;
			bool fT1_used = false;
			double fT0 = 0.0;
			double fT1 = 0.0;

			// Find intersections with lines going through sides of a bounding rectangle.
			clip_line(+direction.x(), -diff_x - len_x, fT0_used, fT1_used, fT0, fT1);
			clip_line(-direction.x(), +diff_x - len_x, fT0_used, fT1_used, fT0, fT1);
			clip_line(+direction.y(), -diff_y - len_y, fT0_used, fT1_used, fT0, fT1);
			clip_line(-direction.y(), +diff_y - len_y, fT0_used, fT1_used, fT0, fT1);

			if (fT0_used && check_extent(fT0, edge_type))
				intersections.push_back(make_point_2d(origin.x() + fT0 * direction.x(),
													  origin.y() + fT0 * direction.y()));
			if (fT1_used && fT0 != fT1 && check_extent(fT1, edge_type))
				intersections.push_back(make_point_2d(origin.x() + fT1 * direction.x(),
													  origin.y() + fT1 * direction.y()));

			return fT0_used && fT1_used;
		}
	
	private:
		Helper();

		static bool check_extent(double extent, kEdgeType etype) {
			switch (etype) {
				case SEGMENT: return extent >= 0.0 && extent <= 1.0;
				case RAY: return extent >= 0.0;
				case LINE: return true;
			}
			return true;
		}

		static coordinate_type magnitude(coordinate_type value) {
			if (value >= static_cast<coordinate_type>(0))
				return value;
			return -value;
		}

		static bool clip_line(coordinate_type denom,
							  coordinate_type numer,
							  bool &fT0_used, bool &fT1_used,
							  double &fT0, double &fT1) {
			if (denom > static_cast<coordinate_type>(0)) {
				if (fT1_used && numer > denom * fT1)
					return false;
				if (!fT0_used || numer > denom * fT0) {
					fT0_used = true;
					fT0 = numer / denom;
				}
				return true;
			} else if (denom < static_cast<coordinate_type>(0)) {
				if (fT0_used && numer > denom * fT0)
					return false;
				if (!fT1_used || numer > denom * fT1) {
					fT1_used = true;
					fT1 = numer / denom;
				}
				return true;
			}
			return false;
		}
	};

    template <typename T>
    struct voronoi_edge_clipped;
    
	template <typename T>
	struct voronoi_cell_clipped {
		typedef T coordinate_type;
		typedef detail::site_event<T> site_event_type;

		voronoi_edge_clipped<coordinate_type> *incident_edge;
		int num_incident_edges;

		voronoi_cell_clipped(const site_event_type &new_site,
							 voronoi_edge_clipped<coordinate_type>* edge) :
			incident_edge(edge),
			site(new_site),
			num_incident_edges(0) {}

		bool is_segment() const {
			return site.is_segment();
		}

		point_2d<T> get_point0() const {
			return site.get_point0();
		}

		point_2d<T> get_point1() const {
			return site.get_point1();
		}

		private:
			site_event_type site;

	};

	template <typename T>
	struct voronoi_vertex_clipped {
		typedef T coordinate_type;
		typedef point_2d<T> Point2D;

		voronoi_edge_clipped<coordinate_type> *incident_edge;
		Point2D vertex;
		int num_incident_edges;

		voronoi_vertex_clipped(const Point2D &point, voronoi_edge_clipped<coordinate_type>* edge) :
			incident_edge(edge),
			vertex(point),
			num_incident_edges(0) {}
	};

    // Half-edge data structure. Represents voronoi edge.
    // Contains: 1) pointer to cell records;
    //           2) pointers to start/end vertices of half-edge;
    //           3) pointers to previous/next half-edges(CCW);
    //           4) pointers to previous/next half-edges rotated
    //              around start point;
    //           5) pointer to twin half-edge.
    template <typename T>
    struct voronoi_edge_clipped {
        typedef T coordinate_type;
        typedef point_2d<T> Point2D;
		typedef voronoi_cell_clipped<coordinate_type> voronoi_cell_type;
		typedef voronoi_vertex_clipped<coordinate_type> voronoi_vertex_type;
        typedef voronoi_edge_clipped<coordinate_type> voronoi_edge_type;
		typedef detail::site_event<coordinate_type> site_event_type;

        voronoi_cell_type *cell;
        voronoi_vertex_type *start_point;
        voronoi_vertex_type *end_point;
        voronoi_edge_type *prev;
        voronoi_edge_type *next;
        voronoi_edge_type *rot_prev;
        voronoi_edge_type *rot_next;
        voronoi_edge_type *twin;

        voronoi_edge_clipped() :
            cell(NULL),
            start_point(NULL),
            end_point(NULL),
            prev(NULL),
            next(NULL),
            rot_prev(NULL),
            rot_next(NULL),
            twin(NULL) {}

		std::vector<Point2D> get_intermediate_points(BRect<T> &brect) const {
			const voronoi_cell_type *cell1 = cell;
			const voronoi_cell_type *cell2 = twin->cell;
			std::vector<Point2D> edge_points;
			if (!(cell1->is_segment() ^ cell2->is_segment())) {
				if (start_point != NULL && end_point != NULL) {
					edge_points.push_back(start_point->vertex);
					edge_points.push_back(end_point->vertex);
				} else {
					const Point2D &site1 = cell1->get_point0();
					const Point2D &site2 = cell2->get_point0();
					Point2D origin((site1.x() + site2.x()) * static_cast<coordinate_type>(0.5),
								   (site1.y() + site2.y()) * static_cast<coordinate_type>(0.5));
					Point2D direction(site1.y() - site2.y(), site2.x() - site1.x());
					Helper<T>::find_intersections(origin, direction, Helper<T>::LINE,
												  brect, edge_points);
					if (end_point != NULL)
						edge_points[1] = end_point->vertex;
					if (start_point != NULL)
						edge_points[0] = start_point->vertex;
				}
			} else {
				const Point2D &point1 = (cell1->is_segment()) ?
					cell2->get_point0() : cell1->get_point0();
				const Point2D &point2 = (cell1->is_segment()) ?
					cell1->get_point0() : cell2->get_point0();
				const Point2D &point3 = (cell1->is_segment()) ?
					cell1->get_point1() : cell2->get_point1();
				if (start_point != NULL && end_point != NULL) {
					edge_points.push_back(start_point->vertex);
					edge_points.push_back(end_point->vertex);
					fill_intermediate_points(point1, point2, point3, edge_points);
				} else {
					coordinate_type dir_x = (cell1->is_segment() ^ (point1 == point3)) ?
						point3.y() - point2.y() : point2.y() - point3.y();
					coordinate_type dir_y = (cell1->is_segment() ^ (point1 == point3)) ?
						point2.x() - point3.x() : point3.x() - point2.x();
					Point2D direction(dir_x, dir_y);
					Helper<T>::find_intersections(point1, direction, Helper<T>::LINE,
												  brect, edge_points);
					if (end_point != NULL)
						edge_points[1] = end_point->vertex;
					if (start_point != NULL)
						edge_points[0] = start_point->vertex;
				}
			}
			return edge_points;
		}
    };

    template <typename T>
    struct voronoi_output_clipped {
    public:
        typedef T coordinate_type;
        typedef point_2d<T> Point2D;
		typedef detail::site_event<T> site_event_type;

		typedef voronoi_cell_clipped<coordinate_type> voronoi_cell_type;
		typedef std::list<voronoi_cell_type> voronoi_cells_type;
		typedef typename voronoi_cells_type::iterator voronoi_cell_iterator_type;
		typedef typename voronoi_cells_type::const_iterator voronoi_cell_const_iterator_type;

		typedef voronoi_vertex_clipped<coordinate_type> voronoi_vertex_type;
		typedef std::list<voronoi_vertex_type> voronoi_vertices_type;
		typedef typename voronoi_vertices_type::iterator voronoi_vertex_iterator_type;
		typedef typename voronoi_vertices_type::const_iterator voronoi_vertex_const_iterator_type;

        typedef voronoi_edge_clipped<coordinate_type> voronoi_edge_type;
        typedef std::list<voronoi_edge_type> voronoi_edges_type;
        typedef typename voronoi_edges_type::iterator voronoi_edge_iterator_type;
        typedef typename voronoi_edges_type::const_iterator voronoi_edge_const_iterator_type;

		BRect<coordinate_type> bounding_rectangle;
		        
        int num_cell_records;
        int num_vertex_records;
        int num_edge_records;

        voronoi_cells_type cell_records;
        voronoi_vertices_type vertex_records;
        voronoi_edges_type edge_records;

        voronoi_output_clipped() {
            num_cell_records = 0;
            num_vertex_records = 0;
            num_edge_records = 0;
        }

        void reset() {
            cell_records.clear();
            vertex_records.clear();
            edge_records.clear();

            num_cell_records = 0;
            num_vertex_records = 0;
            num_edge_records = 0;
        }
    };

} // sweepline
} // boost

#endif
