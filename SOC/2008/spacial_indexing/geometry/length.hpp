// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_LENGTH_HPP
#define _GEOMETRY_LENGTH_HPP

#include <geometry/geometry.hpp>
#include <geometry/strategies/strategy_traits.hpp>

/*!
\defgroup length length: length calculation algorithms
The length algorithm is implemented for the linestring and the multi_linestring geometry and results
in the length of the linestring. If the points of a linestring have coordinates expressed in kilometers,
the length of the line is expressed in kilometers as well.
*/

namespace geometry
{
	namespace impl
	{
		namespace length
		{

			/*!
				\brief Internal, calculates length of a linestring using iterator pairs and specified strategy
				\note for_each is not used here, then it will stop it working for std::vector<P> because of ::point_type
			*/
			template<typename IT, typename S>
			inline double length_linestring(IT const& begin, IT const& end, S const& strategy)
			{
				double sum = 0.0;
				IT it = begin;
				if (it != end)
				{
					IT previous = it++;
					while(it != end)
					{
						sum += geometry::distance(*previous, *it, strategy);
						previous = it++;
					}
				}
				return sum;
			}
		}
	}

	/*! 
		\brief Calculate length of a linestring
		\ingroup length
		\details The function length returns the length of a linestring, using the default distance-calculation-strategy
		\param ls linestring 
		\return the length
		\par Example:
		Example showing length calculation of linestrings built of xy-points and of latlong-points
		\dontinclude doxygen_examples.cpp
		\skip example_length_linestring
		\line {
		\until }
	 */
	template<typename P, template<typename,typename> class V, template<typename> class A>
	inline double length(const linestring<P, V, A>& ls)
	{
		return impl::length::length_linestring(ls.begin(), ls.end(), 
			typename strategy_traits<P>::point_distance());
	}

	/*! 
		\brief Calculate length of a linestring
		\ingroup length
		\details The function length returns the length of a linestring, using the specified strategy
		to calculate the distance between the points of the linestring
		\param ls linestring 
		\param strategy strategy to be used for distance calculations. 
		\return the length, in units defined by the strategy
		\par Example:
		Example showing length calculation of latlong linestrings using the accurate Vincenty strategy
		for distance calculations
		\dontinclude doxygen_examples.cpp
		\skip example_length_linestring_strategy
		\line {
		\until }
	 */
	template<typename P, template<typename,typename> class V, template<typename> class A, typename S>
	inline double length(const linestring<P, V, A>& ls, S const& strategy)
	{
		return impl::length::length_linestring(ls.begin(), ls.end(), strategy);
	}


	// Iterator versions.
	// They turn out to be exactly the same as in implementation
	// Versions of linestring<P,V,A>::const_iterator do not work here, somehow
	// Specifying abstract iterators works here because there is only one version specified
	// For algorithms with more than one versions, such as intersection, this will not work.


	/*! 
		\brief Calculate length of a series of points, specified by an iterator pair
		\ingroup length
		\details The function length returns the length of a point container, using the default distance-calculation-strategy
		\param begin iterator positioned at first point 
		\param end iterator positioned after last point 
		\return the length
		\par Example:
		Example showing length calculation using iterators
		\dontinclude doxygen_examples.cpp
		\skip example_length_linestring_iterators1
		\line {
		\until }
		Example showing length calculation on a vector
		\dontinclude doxygen_examples.cpp
		\skip example_length_linestring_iterators2
		\line {
		\until }
	 */
	template<typename IT>
	inline double length(IT begin, IT end)
	{
		return impl::length::length_linestring(begin, end, 
			typename strategy_traits<typename IT::value_type>::point_distance());
	}

	/*! 
		\brief Calculate length of a series of points, specified by an iterator pair and a strategy
		\ingroup length
		\details The function length returns the length of a point container between specified iterators, using specified strategy
		\param begin iterator positioned at first point 
		\param end iterator positioned after last point 
		\param strategy strategy to be used for distance calculations. 
		\return the length
		\par Example:
		Example showing length calculation using iterators and the Vincenty strategy
		\dontinclude doxygen_examples.cpp
		\skip example_length_linestring_iterators3
		\line {
		\until }
	 */
	template<typename IT, typename S>
	inline double length(IT begin, IT end, S const& strategy)
	{
		return impl::length::length_linestring(begin, end, strategy);
	}


} // namespace geometry


#endif // _GEOMETRY_LENGTH_HPP
