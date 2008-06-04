// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_INTERSECTION_LINESTRING_HPP
#define _GEOMETRY_INTERSECTION_LINESTRING_HPP

#include <geometry/geometry.hpp>
#include <geometry/segment.hpp>
#include <geometry/box.hpp>

namespace geometry
{
	namespace strategy
	{
		namespace intersection
		{
			/*!
				\brief Linestring clipping algorithm after Liang Barsky
				\details The Liang-Barsky lien clipping algorithm clips a line with a clipping box. 
				It is slightly adapted in the sense that it returns which points are clipped
				\author Barend Gehrels, and the following recourses
				- A tutorial: http://www.skytopia.com/project/articles/compsci/clipping.html
				- a German applet: http://ls7-www.cs.uni-dortmund.de/students/projectgroups/acit/lineclip.shtml
				\par Template parameters:
				- \a PB point type of clipping box
				- \a PS point type of segments to be clipped
			*/
			template<typename PB, typename PS>
			class liang_barsky
			{
				private :
					inline bool check_edge(const double& p, const double& q, double &t1, double &t2) const
					{
						bool visible = true;

						if(p < 0)
						{
							double r = q / p;
							if(r > t2) visible = false;
							else if(r > t1) t1 = r;
						}
						else if(p > 0)
						{
							double r = q / p;
							if(r < t1) visible = false;
							else if(r < t2) t2 = r;
						}
						else
						{
							if(q < 0) visible = false;
						}

						return visible;
					}

				public :
					bool clip_segment(const box<PB>& b, segment<PS>& s, bool& sp1_clipped, bool& sp2_clipped) const
					{
						typedef typename select_type_traits<typename PB::coordinate_type, typename PS::coordinate_type>::type T;

						double t1 = 0;
						double t2 = 1;

						T dx = s.second.x() - s.first.x();
						T dy = s.second.y() - s.first.y();

						T p1 = -dx;
						T p2 = dx;
						T p3 = -dy;
						T p4 = dy;

						T q1 = s.first.x() - b.min().x();
						T q2 = b.max().x() - s.first.x();
						T q3 = s.first.y() - b.min().y();
						T q4 = b.max().y() - s.first.y();

						if(check_edge(p1, q1, t1, t2)  // left
								&& check_edge(p2, q2, t1, t2) // right
								&& check_edge(p3, q3, t1, t2) // bottom
								&& check_edge(p4, q4, t1, t2) // top
								)
						{
							sp1_clipped = t1 > 0;
							sp2_clipped = t2 < 1;

							// Todo, maybe: round coordinates in case of integer? define some round_traits<> or so?
							// Take boost-round of Fernando
							if (sp2_clipped)
							{
								s.second.x(s.first.x() + t2 * dx);
								s.second.y(s.first.y() + t2 * dy);
							}

							if(sp1_clipped)
							{
								s.first.x(s.first.x() + t1 * dx);
								s.first.y(s.first.y() + t1 * dy);
							}

							return true;
						}

						return false;
					}

					template<typename L, typename O_IT>
					inline void add(L& line_out, O_IT out) const
					{
						if (! line_out.empty())
						{
							*out = line_out;
							out++;
							line_out.clear();
						}
					}
			};
		}
	}


	namespace impl
	{
		namespace intersection
		{


			/*!
				\brief Clips a linestring with a box
				\details A linestring, defined by an iterator pair, is intersected (clipped) by the specified box
				and the resulting linestring, or pieces of linestrings, are sent to the specified output operator.
				\par Template parameters:
				- \a L linestring type, could be "linestring" or std::vector<point>, it should match the output iterator
				- \a B box type
				- \a IT in iterator type 
				- \a O_IT output iterator type
				- \a S strategy, a clipping strategy which should implement the methods "clip_segment" and "add"
				\note The first template parameter should be specified, it cannot be deduced from the parameters and
				there is no convenient way (I see) to construct it by a traits class
			*/
			template <typename L, typename B, typename IT, typename O_IT, typename S>
			O_IT clip_linestring_with_box(const B& b, IT begin, IT end, O_IT out, const S& strategy)
			{
				if (begin == end)
				{
					return (out);
				}

				L line_out;
				
				IT vertex = begin;
				IT previous = vertex++;
				while(vertex != end)
				{
					typename IT::value_type p1 = *previous;
					typename IT::value_type p2 = *vertex;

					// Clip the segment. Five situations:
					// 1. Segment is invisible, finish line if any (shouldn't occur)
					// 2. Segment is completely visible. Add (p1)-p2 to line
					// 3. Point 1 is invisible (clipped), point 2 is visible. Start new line from p1-p2...
					// 4. Point 1 is visible, point 2 is invisible (clipped). End the line with ...p2
					// 5. Point 1 and point 2 are both invisible (clipped). Start/finish an independant line p1-p2
					//
					// This results in:
					// a. if p1 is clipped, start new line
					// b. if segment is partly or completely visible, add the segment
					// c. if p2 is clipped, end the line

					bool c1, c2;
					segment<typename IT::value_type> s(p1, p2);
					if (! strategy.clip_segment(b, s, c1, c2))
					{
						strategy.add(line_out, out);
					}
					else
					{
						// a. If necessary, finish the line and add a start a new one
						if (c1)
						{
							strategy.add(line_out, out);
						}

						// b. Add p1 only if it is the first point, then add p2
						if (line_out.empty())
						{
							line_out.push_back(p1);
						}
						line_out.push_back(p2);

						// c. If c2 is clipped, finish the line
						if (c2)
						{
							strategy.add(line_out, out);
						}
					}
					previous = vertex++;
				}
				// Add last part
				strategy.add(line_out, out);
				return (out);
			}

		} // namespace intersection
	} // namespace impl


	/*!
		\brief Clips a linestring with a box
		\ingroup intersection
		\details A linestring is intersected (clipped) by the specified box
		and the resulting linestring, or pieces of linestrings, are sent to the specified output operator.
		\param b the clipping box
		\param ls the linestring to be clipped
		\param out the output iterator
		\return the output iterator
		\par Template parameters:
		- \a PB point type of box
		- \a PL point type of linestring
		- \a V container type
		- \a A allocator type
		- \a O_IT output iterator
		\note The default clipping strategy, Liang-Barsky, is used. The algorithm is currently only
		implemented for 2D xy points. It could be generic for most ll cases, but not across the 180
		meridian so that issue is still on the todo-list.
		\par Example:
		Example showing clipping of linestring with box
		\dontinclude doxygen_examples.cpp
		\skip example_intersection_linestring1
		\line {
		\until }
	*/
	template<typename PB, typename PL,
			template<typename,typename> class V, template<typename> class A,
			typename O_IT>
	inline O_IT intersection(const box<PB>& b, const linestring<PL, V, A>& ls, O_IT out)
	{
		strategy::intersection::liang_barsky<PB, PL> strategy;
		return (impl::intersection::clip_linestring_with_box<linestring<PL, V, A> > (b, ls.begin(), ls.end(), out, strategy));
	}

} // namespace


#endif //_GEOMETRY_INTERSECTION_LINESTRING_HPP
