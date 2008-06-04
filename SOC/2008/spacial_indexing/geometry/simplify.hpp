// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_SIMPLIFY_HPP
#define _GEOMETRY_SIMPLIFY_HPP

#include <geometry/geometry.hpp>
#include <geometry/distance.hpp>

//#define GL_DEBUG_SIMPLIFY

#ifdef GL_DEBUG_SIMPLIFY
#include <geometry/astext.hpp>
#include <iostream>
#endif

/*!
\defgroup simplify simplify: line/polygon simplification algorithms
The simplify algorithm implements the Douglas Peucker algorithm (or another simplification
algorithm) to simplify linestrings. It is implemented for linestrings, multi_linestrings, 
linear rings, polygons and multi_polygons. Note, however, that simplifying
a valid simple polygon (which never intersects itself) might result in an invalid polygon, 
where the simplified rings intersect themselves or one of the other outer or inner rings.
*/

namespace geometry
{
	namespace strategy
	{
		namespace simplify
		{
			/*!
				\brief Small wrapper around a point, with an extra member "included"
				\details
				- We could implement a complete point as well but that is not necessary
				- We could derive from geometry::point but we need the original later on, including extra members;
					besides that it is not necessary to copy points during the algorithm
				\par Template parameters:
				- \a P the enclosed point type
			*/
			template<typename P>
			struct douglas_peucker_point
			{
				const P& p;
				bool included;

				inline douglas_peucker_point(const P& ap)
					: p(ap)
					, included(false)
				{}

				inline douglas_peucker_point<P> operator=(const douglas_peucker_point<P>& other)
				{
					return douglas_peucker_point<P>(*this);
				}
			};


			/*!
				\brief Implements the simplify algorithm.
				\details The douglas_peucker strategy simplifies a linestring, ring or vector of points
				using the well-known Douglas-Peucker algorithm. For the algorithm, see for example:
				- http://de.wikipedia.org/wiki/Douglas-Peucker-Algorithmus
				- http://www2.dcs.hull.ac.uk/CISRG/projects/Royal-Inst/demos/dp.html

				\note This strategy uses itself a point-segment-distance strategy which can be specified
				\author Barend and Maarten, 1995/1996
				\author Revised using templates/iterators 2008
				\par Template parameters:
				- \a V container with points, should have "value_type"
				- \a O_IT output iterator 
				- \a S point-segment distance strategy to be used

				\par Template concepts for V
				- typename value_type, should define the used point type
			*/
			template<typename V, typename O_IT, typename S>
			class douglas_peucker
			{
				typedef typename V::value_type P;
				typedef douglas_peucker_point<P> DP;
				typedef typename std::vector<DP>::iterator IT;

				inline void consider(IT begin, IT end, double max_dist, int& n, const S& strategy) const
				{
					size_t size = end - begin;
					// size must be at least 3 here: we want to consider a candidate point between begin and end
					if (size <= 2)
					{
#ifdef GL_DEBUG_SIMPLIFY
						if (begin != end)
						{
							std::cout << "ignore between " << begin->p << " and " << (end - 1)->p << " size=" << size << std::endl;
						}
						std::cout << "return because size=" << size << std::endl;
#endif
						return;
					}

					IT last = end - 1;

#ifdef GL_DEBUG_SIMPLIFY
					std::cout << "find between " << begin->p << " and " << last->p << " size=" << size << std::endl;
#endif


					// Find most distance point, compare to the current segment
					geometry::const_segment<P> s(begin->p, last->p);
					double max = -1.0; // any value < 0
					IT candidate;
					for(IT it = begin + 1; it != last; it++)
					{
						typename strategy_traits<P>::point_segment_distance f2;
						distance_result dist = f2(it->p, s);
#ifdef GL_DEBUG_SIMPLIFY
						std::cout << "consider " << it->p << " at " << dist.first << (dist.first > max_dist ? " maybe" : " no") << std::endl;
#endif
						if (dist.first > max)
						{
							max = dist.first;
							candidate = it;
						}
					}

					// If a point is found, set the include flag and handle segments in between recursively
					if (max > max_dist)
					{
#ifdef GL_DEBUG_SIMPLIFY
						std::cout << "use " << candidate->p << std::endl;
#endif

						candidate->included = true;
						n++;

						consider(begin, candidate + 1, max_dist, n, strategy);
						consider(candidate, end, max_dist, n, strategy);
					}
				}


			public :
				typedef typename V::const_iterator CIT;


				/*!
					\brief Call simplification on an iterator pair
				*/
				inline void simplify(CIT begin, CIT end, O_IT out, double max_distance, const S& strategy) const
				{
					// Init the output, a vector of references to all points

					// Note Geometry Algorithms suggest here 
					// http://geometryalgorithms.com/Archive/algorithm_0205/algorithm_0205.htm
					// to "STAGE 1: Vertex Reduction within max_distance of prior vertex cluster"
					// However, that is not correct: a vertex within the specified distance might be
					// excluded here, but might be a better candidate for final inclusion than the point before.

					std::vector<DP> ref_candidates(begin, end);

					// Include first and last point of line, they are always part of the line
					int n = 2;
					ref_candidates.front().included = true;
					ref_candidates.back().included = true;

					// Get points, recursively, including them if they are further away than the specified distance
					consider(ref_candidates.begin(), ref_candidates.end(),
						strategy.squared() ? max_distance * max_distance : max_distance, n, strategy);

					// Copy included elements to the output  (might be changed using copy_if)
					for(typename std::vector<DP>::const_iterator it = ref_candidates.begin();
									it != ref_candidates.end(); it++)
					{
						if (it->included)
						{
							//v_out.push_back(it->p);
							*out = it->p;
							out++;
						}
					}
				}

			};

		}
	}


	namespace impl
	{
		namespace simplify
		{

			template<typename L, typename O_IT, typename S>
			inline void simplify_linestring(const L& l_in, O_IT out, double max_distance, const S& strategy)
			{
				if (l_in.size() <= 2 || max_distance < 0)
				{
					std::copy(l_in.begin(), l_in.end(), out);
					return;
				}
				strategy::simplify::douglas_peucker<L, O_IT, S> simplify_strategy;
				simplify_strategy.simplify(l_in.begin(), l_in.end(), out, max_distance, strategy);
			}

			template<typename R, typename O_IT, typename S>
			inline void simplify_ring(const R& r_in, O_IT out, double max_distance, const S& strategy)
			{
				// Call do_container for a ring
				// Efficient simplification of a ring/polygon is still an "Open Problem"
				// See http://maven.smith.edu/~orourke/TOPP/P24.html#Problem.24
				// The first/last point (the closing point of the ring) should maybe be excluded because it 
				// lies on a line with second/one but last. Here it is never excluded.

				// Note also that, especially if max_distance is too large, the output ring might be self intersecting
				// while the input ring is not, although chances are low in normal polygons

				// Finally the inputring might have 4 points (=correct), the output < 4(=wrong)
				if (r_in.size() <= 4 || max_distance < 0)
				{
					std::copy(r_in.begin(), r_in.end(), out);
					return;
				}

				strategy::simplify::douglas_peucker<R, O_IT, S> simplify_strategy;
				simplify_strategy.simplify(r_in.begin(), r_in.end(), out, max_distance, strategy);
			}

			template<typename Y, typename S>
			inline void simplify_polygon(const Y& poly_in, Y& poly_out, double max_distance, const S& strategy)
			{
				// Note that if there are inner rings, and distance is too large, they might intersect with the
				// outer ring in the output, while it didn't in the input.
				simplify_ring(poly_in.outer(), poly_out.outer(), max_distance, strategy);

				poly_out.inners().resize(poly_in.inners().size());
				typename Y::inner_container_type::const_iterator it_in = poly_in.inners().begin();
				typename Y::inner_container_type::iterator it_out = poly_out.inners().begin();

				for (; it_in != poly_in.inners().end(); it_in++, it_out++)
				{
					simplify_ring(*it_in, std::back_inserter(*it_out), max_distance, strategy);
				}
			}

		} // namespace simplify

	} // namespace impl


	/*! 
		\brief Simplify a (piece of a) linestring using input/output iterator.
		\ingroup simplify
		\details The simplify algorithm removes points, keeping the shape as much as possible.
		This version of simplify is the most basic version, working on an iterator pair, an
		output iterator and two strategies to be specified.
		\param begin iterator pointed at first element to be simplified
		\param end iterator pointed after last element to be simplified
		\param out output iterator, outputs all simplified points
		\param max_distance distance (in units of input coordinates) of a vertex to the linestring to be removed
		\param simplify_strategy strategy to be used for simplification
		\param point_segment_distance_strategy strategy to be used for point-segment-distance calculation
		\par Example:
		The simplify algorithm can be used as following:
		\dontinclude doxygen_examples.cpp
		\skip example_simplify_linestring2
		\line {
		\until }
		or, using a std::vector instead of a linestring, as following:
		\dontinclude doxygen_examples.cpp
		\skip example_simplify_linestring3
		\line {
		\until }
	 */
	template<typename IT, typename O_IT, typename SS, typename DS>
	inline void simplify(IT begin, IT end,
				O_IT out, double max_distance, 
				const SS& simplify_strategy,
				const DS& point_segment_distance_strategy)
	{
		simplify_strategy.simplify(begin, end, out, max_distance, point_segment_distance_strategy);
	}



	/*! 
		\brief Simplify a linestring.
		\ingroup simplify
		\details The simplify algorithm removes points from a linestring, keeping the shape as 
		much as possible. This version of simplify uses the default strategy for point-segment-distance calculation,
		and uses the Douglas Peucker (douglas_peucker) strategy for simplification.
		\param l_in linestring to simplify
		\param l_out simplified linestring
		\param max_distance distance (in units of input coordinates) of a vertex to the linestring to be removed
		\par Example:
		The simplify algorithm can be used as following:
		\dontinclude doxygen_examples.cpp
		\skip example_simplify_linestring1
		\line {
		\until }
	 */
	template<typename P, template<typename,typename> class V, template<typename> class A>
	inline void simplify(const linestring<P, V, A>& l_in, linestring<P, V, A>& l_out, double max_distance)
	{
		impl::simplify::simplify_linestring(l_in, std::back_inserter(l_out), 
			max_distance, typename strategy_traits<P>::point_distance());
	}

	template<typename P,
			template<typename,typename> class VP,
			template<typename,typename> class VR,
			template<typename> class AP,
			template<typename> class AR,
			typename S>
	inline void simplify(const polygon<P, VP, VR, AP, AR>& poly_in,
				polygon<P, VP, VR, AP, AR>& poly_out, double max_distance, 
				const S& point_segment_distance_strategy)
	{
		impl::simplify::simplify_polygon(poly_in, poly_out, max_distance, point_segment_distance_strategy);
	}

	template<typename P,
			template<typename,typename> class VP,
			template<typename,typename> class VR,
			template<typename> class AP,
			template<typename> class AR>
	inline void simplify(const polygon<P, VP, VR, AP, AR>& poly_in,
				polygon<P, VP, VR, AP, AR>& poly_out, double max_distance)
	{
		impl::simplify::simplify_polygon(poly_in, poly_out, max_distance, 
			typename strategy_traits<P>::point_distance());
	}

} // namespace geometry

#endif // _GEOMETRY_SIMPLIFY_HPP
