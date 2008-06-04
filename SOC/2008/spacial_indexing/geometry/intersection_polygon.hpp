// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_INTERSECTION_POLYGON_HPP
#define _GEOMETRY_INTERSECTION_POLYGON_HPP

#include <geometry/geometry.hpp>
#include <geometry/box.hpp>
#include <geometry/segment.hpp>
#include <geometry/area.hpp>
#include <geometry/distance.hpp>
#include <geometry/within.hpp>

#include <geometry/intersection_segment.hpp>

#include <exception>

namespace geometry
{

	//------------------------------------------------------------------------------------------------------------------------
	// Weiler-Atherton approach to clip a polygon within a rectangle
	// Can be relatively easily extended to clip polygon-polygon
	// (Somewhat combined with "segment-code" idea of Cohen-Sutherland to see which segment needs clipping)

	// Keep bitwise.
	// See, e.g., http://www.sunshine2k.de/stuff/Java/CohenSutherland/CohenSutherland.html
	const char cohen_sutherland_top    = 1; // 0001
	const char cohen_sutherland_bottom = 2; // 0010
	const char cohen_sutherland_right  = 4; // 0100
	const char cohen_sutherland_left   = 8; // 1000


	// Extend point type with "tag" information
	// Weiler-Atherton:
	// "The new vertices are tagged to indicate they are intersection vertices."  --> intersection_id > 0
	// and
	// "Repeat steps "b" to "e" until the starting point has been reached." --> is_visited
	template<typename P>
	struct weiler_atherton_point : public P
	{
		public :
			inline weiler_atherton_point()
				: P(init_zero)
				, outside(false)
				, ring_index(-1)
				, intersection_id(-1)
				, is_visited(false)
			{}

			// Constructor with point
			inline weiler_atherton_point(const P& p)
				: P(p)
				, outside(false)
				, ring_index(-1)
				, intersection_id(-1)
				, vertex_index(-1)
				, square_distance(0)
				, is_visited(false)
			{}

			// Constructor with coordinates
			inline weiler_atherton_point(const typename P::coordinate_type& x,
							const typename P::coordinate_type& y)
				: P(x, y)
				, outside(false)
				, ring_index(-1)
				, intersection_id(-1)
				, vertex_index(-1)
				, square_distance(0)
				, is_visited(false)
			{}

			// Constructor with point, Cohen-Sutherland code and linear_ring-index
			inline weiler_atherton_point(const P& p, bool os, int r)
				: P(p)
				, outside(os)
				, ring_index(r)
				, intersection_id(-1)
				, vertex_index(-1)
				, square_distance(0)
				, is_visited(false)
			{}

			inline weiler_atherton_point(const P& p, int si, double d)
				: P(p)
				, outside(false)
				, ring_index(-1)
				, intersection_id(-1)
				, vertex_index(si)
				, square_distance(d)
				, is_visited(false)
			{}


			// Operator to sort on "start vertex", then on distance
			inline bool operator<(const weiler_atherton_point& other) const
			{
				if (other.vertex_index == this->vertex_index)
				{
					return this->square_distance < other.square_distance;
				}
				return this->vertex_index < other.vertex_index;
			}

			int ring_index;
			int vertex_index;
			bool outside;

			bool is_visited;

			// For intersections:
			int intersection_id; // 1-based intersection ID
			double square_distance; // distance to corresponding startpoing of segment
	};


	// Small structure to keep two indices and an ID, public
	struct _intersection_indices
	{
		inline _intersection_indices(int an_id)
			: id(an_id)
			, subject_index(-1)
			, clip_index(-1)
		{}

		int id;
		int subject_index;
		int clip_index;
	};
	typedef std::vector<_intersection_indices> _ii_vector;




	template<typename P>
	inline std::ostream& operator<< (std::ostream &s, const weiler_atherton_point<P>& p)
	{
		s << "(" << p.x() << ", " << p.y() << " S=" << p.vertex_index;

		if (p.square_distance >= 0)
		{
			s << " D=" << sqrt(p.square_distance);
		}
		if (p.outside)
		{
			s << " outside";
		}
		if (p.intersection_id > 0)
		{
			s << " I=" << p.intersection_id;
		}
		if (p.is_visited)
		{
			s << " visited";
		}
		return s << ")";
	}


	template<typename B, typename P>
	inline void _cs_clip_code(const B& b, const P& p,
		char& code, bool& is_inside,
		char& code_and, char& code_or, bool& has_inside)
	{
		code = 0;

		// Note: border has to be included because of boundary cases
		// Therefore we need a second variable to see if it is inside (including on boundary)

		// Compare left/right.
		if (p.x() <= b.min().x()) code = cohen_sutherland_left;
		else if (p.x() >= b.max().x()) code = cohen_sutherland_right;

		// Compare top/bottom
		if (p.y() <= b.min().y()) code |= cohen_sutherland_bottom;
		else if (p.y() >= b.max().y()) code |= cohen_sutherland_top;

		code_and &= code;
		code_or  |= code;

		// Check if point is lying inside clip, or on boundary
		is_inside = within_point_in_box(p, b);
		if (is_inside)
		{
			has_inside = true;
		}
	}



	// Small utility to advance an iterator, considering the container circular
	template<typename I>
	void circular_next(I& it, const I& begin, const I& end)
	{
		it++;
		if (it == end)
		{
			it = begin;
		}
	}

	// Traverse to an unvisited intersection point on the subject polygon.
	template<typename W>
	void traverse_to_intersection(W& subject, W& clip, _ii_vector& indices, typename W::iterator& it)
	{
		for (_ii_vector::iterator iit = indices.begin(); iit != indices.end(); iit++)
		{
			typename W::iterator next = subject.begin() + iit->subject_index;
			// Check both for being visited - we don't know which one was taken
			if (! next->is_visited && ! clip[iit->clip_index].is_visited)
			{
				it = next;
				std::cout << "Start with " << *it << std::endl;
				return;
			}
		}
	}

	template<typename W>
	void traverse_to_after_id(W& subject, W& clip,
				_ii_vector& indices, bool on_subject, int id, typename W::iterator& it)
	{
		for (_ii_vector::iterator iit = indices.begin(); iit != indices.end(); iit++)
		{
			if (iit->id == id)
			{
				if (on_subject)
				{
					it = subject.begin() + iit->subject_index;
					circular_next(it, subject.begin(), subject.end());
				}
				else
				{
					it = clip.begin() + iit->clip_index;
					circular_next(it, clip.begin(), clip.end());
				}
				return;
			}
		}
		throw std::exception("internal error - intersection ID not found");
	}

	template<typename W>
	void traverse_next(W& subject, W& clip,
				_ii_vector& indices, bool& on_subject, typename W::iterator& it)
	{
		// 1. Go to next point
		typename W::iterator next = it;
		if (on_subject)
		{
			circular_next(next, subject.begin(), subject.end());
		}
		else
		{
			circular_next(next, clip.begin(), clip.end());
		}

		// 2. If current was not an intersection point,
		//    or next is an intersection point, we take this next point (but only on subject)
		//    or we are on the subject polygon and next is inside the clip
		if (it->intersection_id <= 0
			|| (on_subject && next->intersection_id > 0)
			|| (on_subject && ! next->outside))
		{
			it = next;

			// TEMP
			if (it->intersection_id <= 0) std::cout << "just take next " << *it << std::endl;
			else if (on_subject && next->intersection_id > 0) std::cout << "next intersection on subject " << *it << std::endl;
			else if (on_subject && ! next->outside) std::cout << "next inside " << *it << std::endl;
			else std::cout << "other " << *it << std::endl;
			// END TEMP
		}
		else
		{
			// we are on subject or on clip, on intersection, next is not an intersection,
			// navigate to the corresponding ID on the other polygon
			traverse_to_after_id(subject, clip, indices, !on_subject, it->intersection_id, next);

			// It may never be outside polygon
			if (!on_subject && next->outside)
			{
				std::cout << "return because of " << *next << std::endl;
				return;
			}

			on_subject = ! on_subject;
			it = next;

			// TEMP
			std::cout << "traverse to " << (on_subject ? "subject " : "clip ") << *it << std::endl;
			// END TEMP
		}
	}


	template<typename S1, typename S2, typename WAP>
	void clip_ring_weiler_atherton_intersect_segment(
				const S1& subject, int subject_index, std::vector<WAP>& subject_points,
				const S2& clip, int clip_index, std::vector<WAP>& clip_points,
				int& id, _ii_vector& indices)
	{
		std::vector<WAP> ip;
		double ra, rb;
		intersection_result r = intersection_segment(subject, clip, ra, rb, ip);

		// If there are two intersection points (== overlap), discard:
		// 1) it can be in opposite direction, they share their border there
		//    but do not intersect -> return
		// 2) it can be in same direction, it is OK but the intersection points are calculated
		//    before/after overlap (see figure)

		if (ip.size() != 1)
		{
			return;
		}

		std::cout << "CHECK: " << subject.first << "-" << subject.second
				<< " x " << clip.first << "-" << clip.second << std::endl;


		// If segment of subject intersects clip, and intersection point is on the clip,
		// we examine further
		if (r == is_intersect)
		{
			r = geometry::intersection_connect_result(r, ra, rb);
			if (r != is_intersect)
			{
				std::cout << "CONNECT: " << subject.first << "-" << subject.second
					<< " x " << clip.first << "-" << clip.second << std::endl;

				switch(r)
				{
					// If it connects at at one end, always consider the subject-point:
					// only continue if it is RIGHT of the clip
					case is_intersect_connect_s1p1 :
						if (point_side(clip, subject.second) >= 0) return;
						break;
					case is_intersect_connect_s1p2 :
						if (point_side(clip, subject.second) >= 0) return;
						break;
					case is_intersect_connect_s2p1 :
					case is_intersect_connect_s2p2 :
						// Consider both points, at least one must be right of the clip
						if (point_side(clip, subject.first) >= 0 && point_side(clip, subject.second) >= 0)
						{
							return;
						}
						break;
				}
			}

		}

		// Add intersection points, if any
		for (typename std::vector<WAP>::iterator it = ip.begin(); it != ip.end(); it++)
		{
			// Tag the point(s) with unique id
			it->intersection_id = ++id;
			// Add it to intersection
			indices.push_back(_intersection_indices(id));
			std::cout << "INTERSECTION: " << id << " " << *it << std::endl;

			// Assign index, calc distance and store for addition to subject and to clip lateron
			it->vertex_index = subject_index;
			it->square_distance = square_distance(subject.first, *it);
			subject_points.push_back(*it);

			it->vertex_index = clip_index;
			it->square_distance = square_distance(clip.first, *it);
			clip_points.push_back(*it);
		}
	}


	template<typename B, typename R, typename WAP, typename WAR>
	int clip_ring_weiler_atherton_intersect(const B& b, const R& in, int ring_index,
			WAR& subject,  std::vector<WAP>& clip, // todo, change to WAR
			int& id, _ii_vector& indices)
	{
		// Some book keeping variables
		char code_and = cohen_sutherland_left
				| cohen_sutherland_right
				| cohen_sutherland_top
				| cohen_sutherland_bottom;
		char code_or = 0;
		bool has_inside = false;

		// Define points upperleft and lowerright. Lowerleft is min, upperright is max
		typedef typename R::point_type P;
		P upper_left(b.min().x(), b.max().y());
		P lower_right(b.max().x(), b.min().y());

		char code_previous = 0;
		//R::const_iterator last = in.end() - 1;

		typename R::const_iterator it = in.begin();
		typename R::const_iterator prev = it++;
		while(it != in.end())
		{
			bool is_inside;
			if (subject.empty())
			{
				// Add first point of out-polygon
				_cs_clip_code(b, *prev, code_previous, is_inside, code_and, code_or, has_inside);
				subject.push_back(WAP(*prev, !is_inside, ring_index));
			}
			const_segment<P> ss(*prev, *it);

			char code_current;
			_cs_clip_code(b, *it, code_current, is_inside, code_and, code_or, has_inside);

			std::cout << "SGMNT: " << ss.first << "-" << ss.second << " cs " << int(code_previous) << "," << int(code_current) << std::endl;


			// If both segments are not in same Cohen-Suth. segment, check for intersections
			// Todo: check better, check if they are lying on same side
			if (code_previous != code_current)
			{
				// Intersections are first stored in a vector, then sorted, then added, see Weiler-Atherton:
				// "If care is taken in placement of intersections where the subject and clip polygon
				//  contours are identical in the x-y plane, no degenerate polygons will be produced by
				//  the clipping process."
				std::vector<WAP> ips;

				// Clip all four sides of box
				clip_ring_weiler_atherton_intersect_segment(ss, -1, ips,
								const_segment<P>(b.min(), upper_left), 0, clip, id, indices);
				clip_ring_weiler_atherton_intersect_segment(ss, -1, ips,
								const_segment<P>(upper_left, b.max()), 1, clip, id, indices);
				clip_ring_weiler_atherton_intersect_segment(ss, -1, ips,
								const_segment<P>(b.max(), lower_right), 2, clip, id, indices);
				clip_ring_weiler_atherton_intersect_segment(ss, -1, ips,
								const_segment<P>(lower_right, b.min()), 3, clip, id, indices);

				// Add all found intersection points to subject polygon, after sorting
				// on distance from first point. There might be up to 4 intersection points
				// in rectangular clips, and much more on polygon clips. However, often there are zero or one
				// intersections, sorting is not a big issue here
				std::sort(ips.begin(), ips.end());
				for (typename std::vector<WAP>::const_iterator pit = ips.begin(); pit != ips.end(); pit++)
				{
					indices[pit->intersection_id - 1].subject_index = subject.size();
					subject.push_back(*pit);
				}
			}

			// After intersections, add endpoint of segment to subject vector
			subject.push_back(WAP(*it, !is_inside, ring_index));

			code_previous = code_current;
			prev = it++;
			//circular_next(last, in.begin(), in.end());
		}

		// If all points are inside, output is original, quit
		if (code_or == 0)
		{
			return 1;
		}

		// If all points are on the same side, there is no intersection possible -> output is zero and quit
		if (code_and & cohen_sutherland_left == cohen_sutherland_left
			|| code_and & cohen_sutherland_right == cohen_sutherland_right
			|| code_and & cohen_sutherland_top == cohen_sutherland_top
			|| code_and & cohen_sutherland_bottom == cohen_sutherland_bottom)
		{
			return 0;
		}

		// Special case: if no points are inside, the clip box might be inside the polygon
		// -> output clip box and quit
		if (! has_inside)
		{
			// Take any point of clip and check if it is within the linear_ring
			if (within_point_in_ring(b.min(), in))
			{
				return 2;
			}
			// If clip is not in the linear_ring, there no intersection
			return 0;
		}

		// TODO

		// Some points are inside, some are outside, clipping should go on
		return 3;
	}


	template<typename B, typename Y>
	void clip_poly_weiler_atherton(const B& b, const Y& in, std::vector<Y>& outlist)
	{
		typedef weiler_atherton_point<typename Y::point_type> wap;
		typedef linear_ring<wap> wapoly;

		// Weiler-Atherton:
		// "A link is established between each pair of new vertices, permitting travel between two
		//  polygons wherever they intersect on the x-y plane."
		// We keep this link in an index-vector
		_ii_vector indices;

		int id = 0;

		// 1: create a copy of the polygon and, at the same time, add all points of "subject" and intersections with clip
		//    The copy is called, after Weiler-Atherton, the "subject"
		//    Check the Cohen-Suth.segment
		//    - to avoid unnecessary intersections (in a BOX, two consecutive inside points doesn't need to be checked)
		//    - to choose the path, lateron
		wapoly subject, clip;

		// For outerring and all rings
		int r = clip_ring_weiler_atherton_intersect(b, in, -1, subject, clip, id, indices);


		// If there are clip intersection points, build up the clip polyon. Add all corner points,
		// then sort on segment-index and distance, then the clip is OK
		clip.push_back(wap(b.min(), 0, 0.0));
		clip.push_back(wap(typename Y::point_type(b.min().x(), b.max().y()), 1, 0.0));
		clip.push_back(wap(b.max(), 2, 0.0));
		clip.push_back(wap(typename Y::point_type(b.max().x(), b.min().y()), 3, 0.0));
		clip.push_back(wap(b.min(), 4, 0.0));

		std::sort(clip.begin(), clip.end());

		// Update the id's of clip intersection points, now we have it
		for (int j = 0; j < clip.size(); j++)
		{
			if (clip[j].intersection_id > 0)
			{
				indices[clip[j].intersection_id - 1].clip_index = j;
			}
		}

		// CHECK
		for (_ii_vector::const_iterator it = indices.begin(); it != indices.end(); it++)
		{
			std::cout
				<< "subj:" << it->subject_index << " " << subject[it->subject_index] << std::endl
				<< "clip:" << it->clip_index << " " << clip[it->clip_index] << std::endl;
		}

		std::cout
			<< "WA: " << std::endl
			<< "in: " << in << std::endl
			<< "subject " << subject << std::endl
			<< "clip " << clip << std::endl;
		// END CHECK


		// 4. build output polygon or polygons, start with an intersected point
		// "4. The actual clipping is now performed
		//     a) An intersection vertex is removed from the first intersection list to be used as a
		//        starting point. If the list is exhausted, the clipping is complete; Go to step 5.
		Y out;
		typename wapoly::iterator it = subject.begin();
		traverse_to_intersection(subject, clip, indices, it);
		bool on_subject = true;
		while(! it->is_visited)
		{
			// Add the point, but only if it differs from previous point
			typename Y::point_type p(it->x(), it->y());
			if (out.empty() || ! (out.back() == p))
			{
				out.push_back(p);
			}

			it->is_visited = true;

			//std::cout << (on_subject ? "subj" : "clip") << *it << std::endl;

			traverse_next(subject, clip, indices, on_subject, it);

			if (it->is_visited)
			{
				// Close (if not closed)
				if (! out.empty() && ! (out.front() == out.back()))
				{
					out.push_back(out.front());
				}


				outlist.push_back(out);
				std::cout << "out: " << out << std::endl;
				out.clear();

				// Go to first unvisited intersection point, if any
				// Else the iterator will result is_visited and it will stop
				traverse_to_intersection(subject, clip, indices, it);
				on_subject = true;
			}
		}
	}



	template<typename B, typename R, typename O>
	void clip_y(const B& b, const R& ring_in, O out)
	{
		typename R::const_iterator vertex = ring_in.begin();
		typename R::const_iterator previous = vertex++;
		R ring_out;
		while(vertex != ring_in.end())
		{

		}
	}

} // namespace


#endif //_GEOMETRY_INTERSECTION_POLYGON_HPP
