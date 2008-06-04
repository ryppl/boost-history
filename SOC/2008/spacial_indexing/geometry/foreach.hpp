// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_FOREACH_HPP
#define _GEOMETRY_FOREACH_HPP

/*!
\defgroup loop loop: Loops and for-each functionality
There are several algorithms provided which walk through the points or segments
of linestrings and polygons. They are called for_each_point, for_each_segment, after
the standard library, and \b loop which is more adapted and of which the functor
could break out if necessary.
Of the for_each algorithms there is a \b const and a non-const version provided.
*/


#include <geometry/geometry.hpp>
#include <geometry/segment.hpp>

#include <algorithm>

namespace geometry
{
	namespace impl
	{
		namespace for_each
		{

			// -----------------------------------------------------------------------------------------------
			// Versions for visiting points
			// -----------------------------------------------------------------------------------------------
			template<typename P, typename F>
			inline F each_point_point(P& p, F f)
			{
				f(p);
				return (f);
			}

			template<typename V, typename F>
			inline F each_point_container(V& v, F f)
			{
				return (std::for_each(v.begin(), v.end(), f));
			}

			template<typename L, typename F>
			inline F each_point_linestring(L& ls, F f)
			{
				return (each_point_container(ls, f));
			}

			template<typename Y, typename F>
			inline F each_point_polygon(Y& poly, F f)
			{
				f = each_point_container(poly.outer(), f);
				for (typename Y::inner_container_type::iterator it = poly.inners().begin();
						it != poly.inners().end(); it++)
				{
					f = each_point_container(*it, f);
				}
				return (f);
			}


			// -----------------------------------------------------------------------------------------------
			// Const versions for visiting points
			// -----------------------------------------------------------------------------------------------
			template<typename P, typename F>
			inline F each_point_point(const P& p, F f)
			{
				f(p);
				return (f);
			}

			template<typename V, typename F>
			inline F each_point_container(const V& v, F f)
			{
				return (each(v.begin(), v.end(), f));
			}

			template<typename L, typename F>
			inline F each_point_linestring(const L& ls, F f)
			{
				return (each_point_container(ls, f));
			}

			template<typename Y, typename F>
			inline F each_point_polygon(const Y& poly, F f)
			{
				f = each_point_container(poly.outer(), f);
				for (typename Y::inner_container_type::const_iterator it = poly.inners().begin();
						it != poly.inners().end(); it++)
				{
					f = each_point_container(*it, f);
				}
				return (f);
			}


			// -----------------------------------------------------------------------------------------------
			// Versions for visiting segments.
			// -----------------------------------------------------------------------------------------------
			template<typename V, typename F>
			inline F each_segment_container(V& v, F f)
			{
				typename V::iterator it = v.begin();
				typename V::iterator previous = it++;
				while(it != v.end())
				{
					segment<typename V::point_type> s(*previous, *it);
					f(s);
					previous = it++;
				}

				return (f);
			}

			template<typename L, typename F>
			inline F each_segment_linestring(L& ls, F f)
			{
				return (each_segment_container(ls, f));
			}

			template<typename Y, typename F>
			inline F each_segment_polygon(Y& poly, F f)
			{
				f = each_segment_container(poly.outer(), f);
				for (typename Y::inner_container_type::iterator it = poly.inners().begin();
						it != poly.inners().end(); it++)
				{
					f = each_segment_container(*it, f);
				}
				return (f);
			}


			// -----------------------------------------------------------------------------------------------
			// Const versions for visiting segments.
			// -----------------------------------------------------------------------------------------------
			template<typename V, typename F>
			inline F each_segment_container(const V& v, F f)
			{
				typename V::const_iterator it = v.begin();
				if (it != v.end())
				{
					typename V::const_iterator previous = it++;
					while(it != v.end())
					{
						const_segment<typename V::point_type> s(*previous, *it);
						f(s);
						previous = it++;
					}
				}

				return (f);
			}

			template<typename L, typename F>
			inline F each_segment_linestring(const L& ls, F f)
			{
				return (each_segment_container(ls, f));
			}

			template<typename Y, typename F>
			inline F each_segment_polygon(const Y& poly, F f)
			{
				f = each_segment_container(poly.outer(), f);
				for (typename Y::inner_container_type::const_iterator it = poly.inners().begin();
						it != poly.inners().end(); it++)
				{
					f = each_segment_container(*it, f);
				}
				return (f);
			}

		} // namespace for_each

	} // namespace impl


	/*!
		\brief Calls functor for point
		\ingroup loop
		\details Calls the functor for the specified point. Not useful on itself, provided
		for consistance with other geometry classes.
	*/
	template<typename P, typename F>
	inline F for_each_point(P& p, F f)
	{
		return (impl::for_each::each_point_point(p, f));
	}

	/*!
		\brief Calls functor for linestring
		\ingroup loop
		\details Calls the functor all points of the specified linestring
	*/
	template<typename P,
			template<typename,typename> class V, template<typename> class A,
			typename F>
	inline F for_each_point(linestring<P, V, A>& ls, F f)
	{
		return (impl::for_each::each_point_container(ls, f));
	}

	/*!
		\brief Calls functor for polygon
		\ingroup loop
		\details Calls the functor all points of the specified polygon
	*/
	template<typename P,
			template<typename,typename> class VP,
			template<typename,typename> class VR,
			template<typename> class AP,
			template<typename> class AR,
			typename F>
	inline F for_each_point(polygon<P, VP, VR, AP, AR>& poly, F f)
	{
		return (impl::for_each::each_point_polygon(poly, f));
	}


	/*!
		\brief Calls functor for point
		\ingroup loop
		\details Calls the functor the specified \b const point.
	*/
	template<typename P, typename F>
	inline F for_each_point(const P& p, F f)
	{
		return (impl::for_each::each_point_point(p, f));
	}

	/*!
		\brief Calls functor for linestring
		\ingroup loop
		\details Calls the functor all points of the specified \b const linestring
	*/
	template<typename P,
			template<typename,typename> class V, template<typename> class A,
			typename F>
	inline F for_each_point(const linestring<P, V, A>& ls, F f)
	{
		return (impl::for_each::each_point_container(ls, f));
	}

	/*!
		\brief Calls functor for polygon
		\ingroup loop
		\details Calls the functor all points of the specified \b const polygon
	*/
	template<typename P,
			template<typename,typename> class VP,
			template<typename,typename> class VR,
			template<typename> class AP,
			template<typename> class AR,
			typename F>
	inline F for_each_point(const polygon<P, VP, VR, AP, AR>& poly, F f)
	{
		return (impl::for_each::each_point_polygon(poly, f));
	}


	/*!
		\brief Calls functor for linestring segments
		\ingroup loop
		\details Calls the functor all segments of the specified linestring
	*/
	template<typename P,
			template<typename,typename> class V, template<typename> class A,
			typename F>
	inline F for_each_segment(linestring<P, V, A>& ls, F f)
	{
		return (impl::for_each::each_segment_container(ls, f));
	}

	/*!
		\brief Calls functor for polygon segments
		\ingroup loop
		\details Calls the functor all segments of the specified polygon
	*/
	template<typename P,
			template<typename,typename> class VP,
			template<typename,typename> class VR,
			template<typename> class AP,
			template<typename> class AR,
			typename F>
	inline F for_each_segment(polygon<P, VP, VR, AP, AR>& poly, F f)
	{
		return (impl::for_each::each_segment_polygon(poly, f));
	}



	/*!
		\brief Calls functor for linestring segments
		\ingroup loop
		\details Calls the functor all segments of the specified \b const linestring
	*/
	template<typename P,
			template<typename,typename> class V, template<typename> class A,
			typename F>
	inline F for_each_segment(const linestring<P, V, A>& ls, F f)
	{
		return (impl::for_each::each_segment_container(ls, f));
	}

	/*!
		\brief Calls functor for polygon segments
		\ingroup loop
		\details Calls the functor all segments of the specified \b const polygon
	*/
	template<typename P,
			template<typename,typename> class VP,
			template<typename,typename> class VR,
			template<typename> class AP,
			template<typename> class AR,
			typename F>
	inline F for_each_segment(const polygon<P, VP, VR, AP, AR>& poly, F f)
	{
		return (impl::for_each::each_segment_polygon(poly, f));
	}


}

#endif // _GEOMETRY_FOREACH_HPP
