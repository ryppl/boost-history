// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef _GEOMETRY_ARITHMETIC_HPP
#define _GEOMETRY_ARITHMETIC_HPP


#include <functional>
#include <boost/call_traits.hpp>
#include <geometry/for_each_coordinate.hpp>


namespace geometry
{
	namespace impl
	{
		template <typename P>
		struct param
		{
			typedef typename boost::call_traits
			                 <typename point_traits<P>::coordinate_type>
			                 ::param_type type;
		};


		template <typename C, template <typename> class Function>
		struct value_operation
		{
			C m_value;
			value_operation(C value): m_value(value) {}

			template <typename P, int I>
			void operator()(P& point) const
			{ get<I>(point) = Function<C>()(get<I>(point), m_value); }
		};

		template <typename PS, template <typename> class Function>
		struct point_operation
		{
			typedef typename point_traits<PS>::coordinate_type T;
			const PS& m_source_point;
			point_operation(const PS& point): m_source_point(point) {}

			template <typename PD, int I>
			void operator()(PD& dest_point) const
			{ 
				get<I>(dest_point) = Function<T>()(get<I>(dest_point), get<I>(m_source_point)); 
			}
		};

	} // namespace impl


	template <typename P>
	void add_value(P& p, typename impl::param<P>::type value)
	{ for_each_coordinate(p, impl::value_operation<typename point_traits<P>::coordinate_type, std::plus>(value)); }

	template <typename P1, typename P2>
	void add_point(P1& p1, const P2& p2)
	{ for_each_coordinate(p1, impl::point_operation<P2, std::plus>(p2)); }



	template <typename P>
	void subtract_value(P& p, typename impl::param<P>::type value)
	{ for_each_coordinate(p, impl::value_operation<typename point_traits<P>::coordinate_type, std::minus>(value)); }

	template <typename P1, typename P2>
	void subtract_point(P1& p1, const P2& p2)
	{ for_each_coordinate(p1, impl::point_operation<P2, std::minus>(p2)); }



	template <typename P>
	void multiply_value(P& p, typename impl::param<P>::type value)
	{ for_each_coordinate(p, impl::value_operation<typename point_traits<P>::coordinate_type, std::multiplies>(value)); }

	template <typename P1, typename P2>
	void multiply_point(P1& p1, const P2& p2)
	{ for_each_coordinate(p1, impl::point_operation<P2, std::multiplies>(p2)); }



	template <typename P>
	void divide_value(P& p, typename impl::param<P>::type value)
	{ for_each_coordinate(p, impl::value_operation<typename point_traits<P>::coordinate_type, std::divides>(value)); }

	template <typename P1, typename P2>
	void divide_point(P1& p1, const P2& p2)
	{ for_each_coordinate(p1, impl::point_operation<P2, std::divides>(p2)); }


} // namespace geometry


#endif
