// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_ASWKT_HPP
#define _GEOMETRY_ASWKT_HPP

#include <geometry/geometry.hpp>
#include <iostream>
#include <string>

/*!
\defgroup wkt wkt: Well-Known Text algorithms
The as_wkt class streams the specified geometry as \ref OGC Well Known Text (\ref WKT). It is defined for OGC geometries.
It is therefore not defined for box and circle.
\note The implementation is independant from point type, point_xy and point_ll are supported, 
as well as points with more than two coordinates.

*/


namespace geometry
{
	namespace impl
	{
		namespace wkt
		{
			namespace impl
			{
				template <typename CH, typename TR, typename P, int I, int Count>
				struct dump_coordinate
				{
					static void dump(std::basic_ostream<CH,TR>& os, const P& p)
					{
						os << (I > 0 ? " " : "") << p.template get<I>();
						dump_coordinate<CH, TR, P, I+1, Count>::dump(os, p);
					}
				};

				template <typename CH, typename TR, typename P, int Count>
				struct dump_coordinate<CH, TR, P, Count, Count>
				{
					static void dump(std::basic_ostream<CH, TR>&, const P&)
					{}
				};
			}

			/*!
				\brief Manipulator helper class to stream points as \ref WKT
			*/
			template <typename P>
			class stream_point
			{
				public :
					inline stream_point(const P& p) : m_point(p) {}

					template <typename CH, typename TR>
					inline void stream(std::basic_ostream<CH,TR>& os)
					{
						os << "POINT(";
						impl::dump_coordinate<CH, TR, P, 0, P::coordinate_count>::dump(os, m_point);
						os << ")";
					}

				private :
					P m_point;
			};


			/*!
				\brief Manipulator helper class to stream containers as WKT
			*/
			template <typename V> 
			class stream_container
			{
				public :
					inline stream_container(const V& container, const std::string& prefix, const std::string& postfix) 
						: m_container(container) 
						, m_prefix(prefix)
						, m_postfix(postfix)
					{}

					template <typename CH, typename TR>
					inline void stream(std::basic_ostream<CH, TR>& os)
					{
						bool first = true;
						os << m_prefix;
						for (typename V::const_iterator it = m_container.begin(); it != m_container.end(); it++)
						{
							if (! first)
							{
								os << ",";
							}
							first = false;

							typedef typename V::point_type P;
							impl::dump_coordinate<CH, TR, P, 0, P::coordinate_count>::dump(os, *it);
						}
						os << m_postfix;
					}

				private :
					V m_container;
					std::string m_prefix;
					std::string m_postfix;

			};

			/*!
				\brief Manipulator helper class to stream linestring as WKT
			*/
			template <typename L> 
			class stream_linestring : public stream_container<L>
			{
				public :
					inline stream_linestring(const L& ls) : stream_container<L>(ls, "LINESTRING(", ")") {}
			};


			/*!
				\brief Manipulator helper class to stream polygon as WKT
			*/
			template <typename P> 
			class stream_polygon 
			{
				public :
					inline stream_polygon(const P poly) : m_poly(poly) {}

					template <typename CH, typename TR>
					inline void stream(std::basic_ostream<CH, TR>& os)
					{
						os << "POLYGON(";

						stream_container<typename P::ring_type> wkt(m_poly.outer(), "(", ")");
						wkt.stream(os);

						for (typename P::inner_container_type::const_iterator it = m_poly.inners().begin();
										it != m_poly.inners().end(); it++)
						{
							os << ",";
							stream_container<typename P::ring_type> wkt(*it, "(", ")");
							wkt.stream(os);
						}

						os << ")";
					}

				private :
					P m_poly;
			};


		} // namespace wkt
	} // namespace impl


	/*!
		\brief Traits class to select wkt-implementations
	*/
	template <typename G>
	struct wkt_traits
	{
		// Todo: define stream_type as not_implemented
	};


	// specializations for point, linestring, polygon
	template <typename T, size_t D>
	struct wkt_traits<geometry::point<T, D> >
	{
		typedef impl::wkt::stream_point<geometry::point<T, D> > stream_type;
	};

	// Alas it must be specialized for each derived type
	template <typename T>
	struct wkt_traits<geometry::point_xy<T> >
	{
		typedef impl::wkt::stream_point<geometry::point_xy<T> > stream_type;
	};

	template <typename P, template<typename,typename> class V, template<typename> class A>
	struct wkt_traits<geometry::linestring<P, V, A> >
	{
		typedef impl::wkt::stream_linestring<geometry::linestring<P, V, A> > stream_type;
	};

	template<typename P,
			template<typename,typename> class VP,
			template<typename,typename> class VR,
			template<typename> class AP,
			template<typename> class AR>
	struct wkt_traits<geometry::polygon<P, VP, VR, AP, AR> >
	{
		typedef impl::wkt::stream_polygon<geometry::polygon<P, VP, VR, AP, AR> > stream_type;
	};


	/*!
		\brief Generic geometry template manipulator class, takes corresponding output class from traits class
		\ingroup wkt
		\details Stream manipulator, streams geometry classes as \ref WKT streams
		\par Example:
		Small example showing how to use the as_wkt class
		\dontinclude doxygen_examples.cpp
		\skip example_as_wkt_point
		\line {
		\until }
		\note the template parameter must be specified. If that is inconvient, users might use streamwkt
		which streams geometries as manipulators.
	*/
	template <typename G>
	class as_wkt
	{
		public :
			as_wkt(const G& g) : m_geometry(g) {}

			template <typename CH, typename TR>
			inline friend std::basic_ostream<CH,TR>& operator<<(std::basic_ostream<CH,TR>& os, const as_wkt& m)
			{ 
				typename wkt_traits<G>::stream_type w(m.m_geometry);
				w.stream(os);
				os.flush();
				return os;
			}

		private :
			G m_geometry;
	};

}

#endif // _GEOMETRY_ASWKT_HPP
