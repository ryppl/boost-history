// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_LOOP_HPP
#define _GEOMETRY_LOOP_HPP

#include <geometry/segment.hpp>



namespace geometry
{



	/*!
		\brief Loops through segments of a container and call specified functor for all segments.
		\ingroup loop
		\details for_each like implementation to:
		- walk over const segments of a linestring/polygon
		- be able to break out the loop (if the functor returns false)
		- have a const functor and keep state in separate state-object
		- we still keep the "functor" here so it might be a function or an object, at this place
		- in most algorithms the typename F::state_type is used; in those places it must be an object

		\param v container (vector,list,deque) containing points
		\param functor functor which is called at each const segment
		\param state state, specified separately from the strategy functor
		\return false if the functor returns false, otherwise true
		\par Template parameters:
		- \a V container type, for example a vector, linestring, linear_ring
		- \a F functor type, class or function, not modified by the algorithm
		- \a S state type, might be modified
		\par Concepts
		- \a V
			- const_iterator begin() 
			- const_iterator end()
			- value_type
		- \a F
			- <em>if it is a function functor</em>: bool \b function (const const_segment&, state&)
			- <em>if it is a class functor</em>: bool operator()(const const_segment&, state&) const
		- \a S
			- no specific requirements here, requirments given by F
		\note Some algorithms from the Geometry Library, for example within, centroid,
		use this method.
		\par Examples:
		First example, using a class functor
		\dontinclude doxygen_examples.cpp
		\skip example_loop1
		\line {
		\until //:\\
		Second example, using a function functor and latlong coordinates
		\dontinclude doxygen_examples.cpp
		\skip example_loop2
		\line {
		\until //:\\
	*/
	template<typename V, typename F, typename S>
	inline bool loop(const V& v, const F& functor, S& state)
	{
		typename V::const_iterator it = v.begin();
		if (it != v.end())
		{
			typename V::const_iterator previous = it++;
			while(it != v.end())
			{
				const_segment<typename V::value_type> s(*previous, *it);
				if (! functor(s, state))
				{
					return false;
				}
				previous = it++;
			}
		}
		return true;
	}

} // namespace geometry


#endif // _GEOMETRY_LOOP_HPP
