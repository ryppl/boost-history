// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_CENTROID_HPP
#define _GEOMETRY_CENTROID_HPP

#include <geometry/geometry.hpp>
#include <geometry/box.hpp>
#include <geometry/loop.hpp>
#include <geometry/copy.hpp>

#include <geometry/strategies/strategy_traits.hpp>

/*!
\defgroup centroid centroid: centroid calculation algorithms
The centroid of a surface geometry returns a point which is the centroid the geometry.
*/

namespace geometry
{
	// Move elsewhere?
    class centroid_exception : public std::exception
    {
		public:
			centroid_exception()  {}
			virtual const char *what() const throw() 
			{
				return "centroid calculation exception";
			}
    };

	namespace impl
	{


		namespace centroid
		{

			/*!
				\brief Generic function which checks if enough points are present
			*/
			template<typename P, typename R>
			inline bool ring_ok(const R& ring, P& c)
			{
				if (ring.size() == 1)
				{
					// Take over the first point in a "coordinate neutral way"
					copy_coordinates(ring.front(), c);
					return false;
				}
				else if (ring.size() <= 0)
				{
					throw centroid_exception();
				}
				return true;
			}


			/*!
				\brief Calculate the centroid of a ring.
			*/
			template<typename P, typename R, typename S>
			inline void centroid_ring(const R& ring, P& c, const S& strategy)
			{
				if (ring_ok(ring, c))
				{
					typename S::state_type state;
					loop(ring, strategy, state);
					state.centroid(c);
				}
			}

			/*!
				\brief Centroid of a polygon.
				\note Because outer ring is clockwise, inners are counter clockwise,
				triangle approach is OK and works for polygons with rings.
			*/
			template<typename P, typename Y, typename S>
			inline void centroid_polygon(const Y& poly, P& c, const S& strategy)
			{
				if (ring_ok(poly.outer(), c))
				{
					typename S::state_type state;
					loop(poly.outer(), strategy, state);
					for (typename Y::inner_container_type::const_iterator it = poly.inners().begin(); it != poly.inners().end(); it++)
					{
						loop(*it, strategy, state);
					}
					state.centroid(c);
				}
			}


			/*!
				\brief Calculate centroid (==center) of a box
				\todo Implement strategy
			*/
			template<typename P, typename B>
			inline void centroid_box(const B& box, P& c)
			{
				c.x((box.min().x() + box.max().x()) / 2);
				c.y((box.min().y() + box.max().y()) / 2);
			}

		} // namespace centroid
	} // namespace impl


	template<typename PC, typename PB>
	inline void centroid(const box<PB>& b, PC& c)
	{
		impl::centroid::centroid_box<PC>(b, c);
	}

	/*! 
		\brief Calculate centroid of polygon
		\ingroup centroid
		\details The function centroid calculates the centroid of a polygon using the default strategy.
		The polygon should be closed and orientated clockwise, holes, if any, must be orientated
		counter clockwise
		\param poly a closed polygon
		\param c reference to point which will contain the centroid
		\exception centroid_exception if calculation is not successful, e.g. because polygon didn't contain points
		\note It does not RETURN the centroid. The type of the centroid-point might be different 
		from the type of the polygon-points. Therefore it is a reference, this avoids inconvenient calls
		as center = centroid<geometry::point_xy<double> >(poly);
		\par Example:
		Example showing centroid calculation 
		\dontinclude doxygen_examples.cpp
		\skip example_centroid_polygon
		\line {
		\until }
	 */
	template<typename PC, typename PP,
			template<typename,typename> class VP,
			template<typename,typename> class VR,
			template<typename> class AP,
			template<typename> class AR>
	inline void centroid(const polygon<PP, VP, VR, AP, AR>& poly, PC& c)
	{
		impl::centroid::centroid_polygon(poly, c, 
			typename strategy_traits<PC, PP>::centroid());
	}


	/*! 
		\brief Calculate centroid of polygon
		\ingroup centroid
		\details The function centroid calculates the centroid of a polygon. This version takes a specified 
		strategy for centroid calculation. The polygon should be closed and orientated clockwise, holes, if 
		any, must be orientated counter clockwise
		\param poly a closed polygon
		\param c reference to point which will contain the centroid
		\param strategy Calculation strategy for centroid
		\exception centroid_exception if calculation is not successful, e.g. because polygon didn't contain points
	 */
	template<typename PC, typename PP,
			template<typename,typename> class VP,
			template<typename,typename> class VR,
			template<typename> class AP,
			template<typename> class AR,
			typename S>
	inline void centroid(const polygon<PP, VP, VR, AP, AR>& poly, PC& c, const S& strategy)
	{
		impl::centroid::centroid_polygon(poly, c, strategy);
	}

} // namespace geometry


#endif // _GEOMETRY_CENTROID_HPP
