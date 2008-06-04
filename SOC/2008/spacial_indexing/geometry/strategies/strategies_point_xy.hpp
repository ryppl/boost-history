// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_STRATEGIES_POINT_XY_HPP
#define _GEOMETRY_STRATEGIES_POINT_XY_HPP

#include <utility>

#include <geometry/geometry.hpp>
#include <geometry/segment.hpp>
#include <geometry/grow.hpp>
#include <geometry/arithmetic.hpp>
#include <geometry/assign.hpp>
#include <geometry/copy.hpp>
#include <geometry/dot_product.hpp>
#include <geometry/concepts/point_traits.hpp>
#include <geometry/util/promotion_traits.hpp>
#include <geometry/util/return_types.hpp>
#include <geometry/util/side.hpp>

namespace geometry
{
	namespace strategy
	{
		namespace distance
		{
			namespace impl
			{
				template <typename P1, typename P2, size_t I, typename T>
				struct compute_pythagoras
				{
					static T result(const P1& p1, const P2& p2)
					{
						T d = get<I-1>(p2) - get<I-1>(p1);
						return d*d + compute_pythagoras<P1, P2, I-1, T>::result(p1, p2);
					}
				};

				template <typename P1, typename P2, typename T>
				struct compute_pythagoras<P1, P2, 0, T>
				{
					static T result(const P1&, const P2&)
					{
						return 0;
					}
				};
			}

			/*!
				\brief Strategy functor for distance point to point: pythagoras
				\par Template parameters:
				- \a P1 first point type
				- \a P2 optional, second point type, defaults to first point type
				\par Concepts for P1 and P2:
				- specialized point_traits class
			*/
			template <typename P1, typename P2 = P1>
			struct pythagoras
			{
				static inline bool squared() { return true; }

				inline distance_result operator()(const P1& p1, const P2& p2) const
				{
					BOOST_STATIC_ASSERT(point_traits<P1>::coordinate_count == point_traits<P2>::coordinate_count);

					// Calculate distance using Pythagoras
					// (Leave comment above for Doxygen)
					typedef typename select_type_traits<
						             typename point_traits<P1>::coordinate_type,
						             typename point_traits<P2>::coordinate_type
					                 >::type T;
					return distance_result(impl::compute_pythagoras<P1, P2, point_traits<P1>::coordinate_count, T>::result(p1, p2), true);
				}
			};


			/*!
				\brief Strategy functor for distance point to segment
				\details Calculates distance using projected-point method, and (optionally) Pythagoras
				\author Adapted from: http://geometryalgorithms.com/Archive/algorithm_0102/algorithm_0102.htm
				\par Template parameters:
				- \a P point type
				- \a PS point type of segments, optional, defaults to point type
				- \a S strategy, optional, defaults to pythagoras
				\par Concepts for P and PS:
				- specialized point_traits class
				\par Concepts for S:
				- distance_result operator(P,P)
			*/
			template <typename P, typename PS = P, typename S = pythagoras<P, PS> >
			struct xy_point_segment
			{
				typedef S distance_strategy_type;

				static inline bool squared() { return S::squared(); }

				inline distance_result operator()(const P& p, const const_segment<PS>& s) const
				{
					BOOST_STATIC_ASSERT(point_traits<P>::coordinate_count == point_traits<PS>::coordinate_count);

					S distance_strategy;

					/* Algorithm
					POINT v(x2 - x1, y2 - y1);
					POINT w(px - x1, py - y1);
					c1 = w . v
					c2 = v . v
					b = c1 / c2
					RETURN POINT(x1 + b * vx, y1 + b * vy);
					*/

					PS v, w;
					copy_coordinates(s.second, v);
					copy_coordinates(p, w);
					subtract_point(v, s.first);
					subtract_point(w, s.first);

					typedef typename point_traits<PS>::coordinate_type T;

					T c1 = dot_product(w, v);
					if (c1 <= 0)
					{
						return distance_strategy(p, s.first);
					}
					T c2 = dot_product(v, v);
					if (c2 <= c1)
					{
						return distance_strategy(p, s.second);
					}

					// Even in case of char's, we have to turn to a point<double/float> 
					// because of the division.
					double b = c1 / c2;

					PS projected;
					copy_coordinates(s.first, projected);
					multiply_value(v, b);
					add_point(projected, v);

					return distance_strategy(p, projected);

				}
			};

		} // namespace distance


		namespace area
		{

			/*!
				\brief Strategy functor for area calculation on point_xy points
				\details Calculates area using well-known triangulation algorithm
				\par Template parameters:
				- \a PS point type of segments
				\par Concepts for PS:
				- specialized point_traits class
			*/
			template<typename PS>
			class by_triangles
			{
				private :
					struct summation
					{
						typedef typename point_traits<PS>::coordinate_type T;
						T sum;
						inline summation() : sum(T()) {}
						inline double area() const { return 0.5 * double(sum); }
					};

				public :
					typedef summation state_type;

					inline bool operator()(const const_segment<PS>& s, state_type& state) const
					{
						// SUM += x2 * y1 - x1 * y2;

						// Currently only 2D areas are supported
						BOOST_STATIC_ASSERT(point_traits<PS>::coordinate_count == 2);
						state.sum += get<0>(s.second) * get<1>(s.first) - get<0>(s.first) * get<1>(s.second);
						return true;
					}

			};

		} // namespace area


		namespace within
		{
			/*!
				\brief Within detection using cross counting

				\author adapted from Randolph Franklin algorithm
				\author Barend and Maarten, 1995
				\author Revised for templatized library, Barend Gehrels, 2007
				\return true if point is in ring, works for closed rings in both directions
				\note Does NOT work correctly for point ON border
			 */

			template<typename P, typename PS = P>
			struct franklin
			{
				private :
					/*! subclass to keep state */
					struct crossings
					{
						P p;
						bool crosses;
						explicit crossings(const P& ap)
							: p(ap)
							, crosses(false)
						{}
						bool within() const
						{
							return crosses;
						}
					};

				public :

					typedef crossings state_type;

					inline bool operator()(const const_segment<PS>& s, state_type& state) const
					{
						/* Algorithm:
						if (
							( (y2 <= py && py < y1) 
								|| (y1 <= py && py < y2) )
							&& (px < (x1 - x2) 
									* (py - y2) 
										/ (y1 - y2) + x2)
							)
								crosses = ! crosses
						*/
		

						if (
							((get<1>(s.second) <= get<1>(state.p) && get<1>(state.p) < get<1>(s.first)) 
								|| (get<1>(s.first) <= get<1>(state.p) && get<1>(state.p) < get<1>(s.second)))
							&& (get<0>(state.p) < (get<0>(s.first) - get<0>(s.second)) 
								* (get<1>(state.p) - get<1>(s.second))
										/ (get<1>(s.first) - get<1>(s.second)) + get<0>(s.second))
							)
						{
							state.crosses = ! state.crosses;
						}
						return true;
					}
			};


			/*!
				\brief Within detection using winding rule
				\par Template parameters:
				- \a P point type of point to examine
				- \a PS point type of segments, defaults to P
				\par Concepts for P and PS:
				- specialized point_traits class
				\author The implementation is inspired by terralib http://www.terralib.org (LGPL)
				and http://geometryalgorithms.com/Archive/algorithm_0103/algorithm_0103.htm
				\note Added the horizontal case, for "completely within"
				\note Is tested for points ON border, more tests have to be done.
				\note More efficient (less comparisons and no divison) than the cross count algorithm
			 */
			template<typename P, typename PS = P>
			class winding
			{
				private :
					/*! subclass to keep state */
					struct windings
					{
						int count;
						bool touches;
						P p;
						explicit windings(const P& ap)
							: p(ap)
							, count(0)
							, touches(false)
						{}
						bool within() const
						{
							return ! touches && count != 0;
						}
					};

				public :

					typedef windings state_type;

					inline bool operator()(const const_segment<PS>& s, state_type& state) const
					{
						bool up = false;
						bool down = false;

						if (equals(get<1>(s.first), get<1>(state.p)) && equals(get<1>(s.first), get<1>(s.second)))
						{
							// Horizontal case
							if (get<0>(s.first) <= get<0>(state.p) && get<0>(s.second) > get<0>(state.p))
							{
								up = true; // "up" means from left to right here
							}
							else if (get<0>(s.second) < get<0>(state.p) && get<0>(s.first) >= get<0>(state.p))
							{
								down = true; // from right to left
							}
						}
						else if (get<1>(s.first) <= get<1>(state.p) && get<1>(s.second) > get<1>(state.p))
						{
							up = true;
						}
						else if (get<1>(s.second) < get<1>(state.p) && get<1>(s.first) >= get<1>(state.p))
						{
							down = true;
						}

						if (up || down)
						{
							typedef typename point_traits<P>::coordinate_type T;
							T side = point_side(s, state.p);
							if (equals<T>(side, 0))
							{
								state.touches = true;
								return false;
							}
							else if (up && side > 0)
							{
								state.count++;
							}
							else if (down && side < 0)
							{
								state.count--;
							}
						}
						return true;
					}

			};

		} // namespace within


		namespace centroid
		{
			/*!
				\brief Centroid calculation
				\details Geolib original version,
				\par Template parameters and concepts: see bashein_detmer
				\author Barend and Maarten, 1995/1996
				\author Revised for templatized library, Barend Gehrels, 2007
				\note The results are slightly different from Bashein/Detmer, so probably slightly wrong.
			*/

			template<typename PC, typename PS = PC>
			class geolib1995
			{
				private  :
					typedef typename point_traits<PS>::coordinate_type T;

					/*! subclass to keep state */
					struct sums
					{
						 PC sum_ms, sum_m;

						sums()
						{
							assign_point(sum_m, T());
							assign_point(sum_ms, T());
						}
						void centroid(PC& point)
						{
							point = sum_ms;

							if (get<0>(sum_m) != 0 && get<1>(sum_m) != 0)
							{
								multiply_value(point, 0.5);
								divide_point(point, sum_m);
							}
							else
							{
								// exception?
							}
						}
					};

				public :
					typedef sums state_type;
					inline bool operator()(const const_segment<PS>& s, state_type& state) const
					{
						/* Algorithm:
						For each segment:
						begin
							dx = x2 - x1;
							dy = y2 - y1;
							sx = x2 + x1;
							sy = y2 + y1;
							mx = dx * sy;
							my = sx * dy;

							sum_mx += mx;
							sum_my += my;
							sum_msx += mx * sx;
							sum_msy += my * sy;
						end;
						return POINT(0.5 * sum_msx / sum_mx, 0.5 * sum_msy / sum_my);
						*/

						PS diff = s.second, sum = s.second;
						subtract_point(diff, s.first);
						add_point(sum, s.first);

						// We might create an arithmatic operation for this.
						PS m;
						get<0>(m) = get<0>(diff) * get<1>(sum);
						get<1>(m) = get<0>(sum) * get<1>(diff);

						add_point(state.sum_m, m);
						multiply_point(m, sum);
						add_point(state.sum_ms, m);

						return true;
					}

			};


			/*!
				\brief Centroid calculation using algorith Bashein / Detmer
				\details Calculates centroid using triangulation method published by Bashein / Detmer
				\par Template parameters:
				- \a PC point type of centroid to calculate
				- \a PS point type of segments, defaults to PC
				\par Concepts for PC and PS:
				- specialized point_traits class
				\author Adapted from  "Centroid of a Polygon" by Gerard Bashein and Paul R. Detmer<em>,
				in "Graphics Gems IV", Academic Press, 1994</em>
				\par Research notes
				The algorithm gives the same results as Oracle and PostgreSQL but differs from MySQL
				(tried 5.0.21 / 5.0.45 / 5.0.51a / 5.1.23).

				Without holes:
				- this: POINT(4.06923 1.65056)
				- geolib: POINT(4.07254 1.66819)
				- MySQL: 'POINT(3.6636363636364 1.6272727272727)'
				- PostgreSQL: "POINT(4.06923363095238 1.65055803571429)"
				- Oracle: 4.06923363095238, 1.65055803571429

				Statements:
				- \b MySQL/PostgreSQL: select AsText(Centroid(GeomFromText('POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2
								,3.7 1.6,3.4 2,4.1 3,5.3 2.6,5.4 1.2,4.9 0.8,2.9 0.7,2 1.3))')))
				- \b Oracle: select sdo_geom.sdo_centroid(sdo_geometry(2003, null, null,
						sdo_elem_info_array(1, 1003, 1), sdo_ordinate_array(2,1.3,2.4,1.7,2.8,1.8
						,3.4,1.2,3.7,1.6,3.4,2,4.1,3,5.3,2.6,5.4,1.2,4.9,0.8,2.9,0.7,2,1.3))
						, mdsys.sdo_dim_array(mdsys.sdo_dim_element('x',0,10,.00000005)
						,mdsys.sdo_dim_element('y',0,10,.00000005)))
						from dual

				With holes:
				- this: POINT(4.04663 1.6349)
				- geolib: POINT(4.04675 1.65735)
				- MySQL: 'POINT(3.6090580503834 1.607573932092)'
				- PostgresSQL: "POINT(4.0466265060241 1.63489959839357)"
				- Oracle: 4.0466265060241, 1.63489959839357

				Statements:
				- \b MySQL/PostgreSQL: select AsText(Centroid(GeomFromText('POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2
						,3.7 1.6,3.4 2,4.1 3,5.3 2.6,5.4 1.2,4.9 0.8,2.9 0.7,2 1.3)
						,(4 2,4.2 1.4,4.8 1.9,4.4 2.2,4 2))')));
				- \b Oracle: select sdo_geom.sdo_centroid(sdo_geometry(2003, null, null
						, sdo_elem_info_array(1, 1003, 1, 25, 2003, 1)
						, sdo_ordinate_array(2,1.3,2.4,1.7,2.8,1.8,3.4,1.2,3.7,1.6,3.4,2,4.1,3,5.3
						,2.6,5.4,1.2,4.9,0.8,2.9,0.7,2,1.3,4,2, 4.2,1.4, 4.8,1.9, 4.4,2.2, 4,2))
						, mdsys.sdo_dim_array(mdsys.sdo_dim_element('x',0,10,.00000005)
						,mdsys.sdo_dim_element('y',0,10,.00000005)))
						from dual
			 */
			template<typename PC, typename PS = PC>
			class bashein_detmer
			{
				private :
					typedef typename point_traits<PS>::coordinate_type T;

					/*! subclass to keep state */
					struct sums
					{
						T sum_a2;
						PC sum;
						sums()
							: sum_a2(T()) 
						{
							assign_value(sum, T());
						}

						void centroid(PC& point)
						{
							typedef typename point_traits<PC>::coordinate_type TPC;
							point = sum;
							if (sum_a2 != 0)
							{
								divide_value(point, double(3.0 * sum_a2));
							}
							else
							{
								// exception?
							}
						}

					};

				public :
					typedef sums state_type;

					inline bool operator()(const const_segment<PS>& s, state_type& state) const
					{
						/* Algorithm:
						For each segment:
						begin
							ai = x1 * y2 - x2 * y1;
							sum_a2 += ai;
							sum_x += ai * (x1 + x2);
							sum_y += ai * (y1 + y2);
						end
						return POINT(sum_x / (3 * sum_a2), sum_y / (3 * sum_a2) )
						*/

						T ai = get<0>(s.first) * get<1>(s.second) - get<0>(s.second) * get<1>(s.first);
						state.sum_a2 += ai;

						PC p;
						copy_coordinates(s.first, p);
						add_point(p, s.second);
						multiply_value(p, ai);
						add_point(state.sum, p);
						
						return true;
					}

			};
		} // namespace centroid


		namespace envelope
		{
			// envelope calculation strategy for xy-points
			template <typename PB, typename P = PB>
			struct grow_xy
			{
				struct state
				{
					box<PB> b;
					state() : b(init_inverse) {}
					void envelope(box<PB>& mbr)
					{
						mbr.min() = b.min();
						mbr.max() = b.max();
					}
				};

				typedef state state_type;

				void operator()(const P& p, state_type& s) const
				{
					impl::grow::grow_p(s.b, p);
				}
			};
		} // namespace envelope


	} // namespace strategy


} // namespace geometry


#endif // _GEOMETRY_STRATEGIES_POINT_XY_HPP
